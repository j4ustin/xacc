/***********************************************************************************
 * Copyright (c) 2016, UT-Battelle
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of the xacc nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Contributors:
 *   Initial API and implementation - Alex McCaskey
 *
 **********************************************************************************/
#include "RigettiAccelerator.hpp"
#include "AsioNetworkingTool.hpp"
#include "RuntimeOptions.hpp"
#include <boost/algorithm/string.hpp>

namespace xacc {
namespace quantum {

std::shared_ptr<AcceleratorBuffer> RigettiAccelerator::createBuffer(
		const std::string& varId) {
	auto buffer = std::make_shared<AcceleratorBuffer>(varId);
	storeBuffer(varId, buffer);
	return buffer;
}

std::shared_ptr<AcceleratorBuffer> RigettiAccelerator::createBuffer(
		const std::string& varId, const int size) {
	if (!isValidBufferSize(size)) {
		XACCError("Invalid buffer size.");
	}
	auto buffer = std::make_shared<AcceleratorBuffer>(varId, size);
	storeBuffer(varId, buffer);
	return buffer;
}

bool RigettiAccelerator::isValidBufferSize(const int NBits) {
	return NBits > 0;
}

void RigettiAccelerator::execute(std::shared_ptr<AcceleratorBuffer> buffer,
		const std::shared_ptr<xacc::Function> kernel) {

	auto options = RuntimeOptions::instance();
	std::string type = "multishot";
	std::string jsonStr = "", apiKey = "";
	std::string trials = "10";
	std::map<std::string, std::string> headers;

	auto visitor = std::make_shared<QuilVisitor>();

	// Our QIR is really a tree structure
	// so create a pre-order tree traversal
	// InstructionIterator to walk it
	InstructionIterator it(kernel);
	while (it.hasNext()) {
		// Get the next node in the tree
		auto nextInst = it.next();
		if (nextInst->isEnabled()) nextInst->accept(visitor);
	}

	if (!options->exists("api-key")) {
		XACCError("Cannot execute kernel on Rigetti chip without API Key.");
	}

	apiKey = (*options)["api-key"];

	// Set the execution type if the user provided it
	if (options->exists("type")) {
		type = (*options)["type"];
	}

	// Set the trials number if user provided it
	if (options->exists("trials")) {
		trials = (*options)["trials"];
	}

	if (type == "ping") {
		jsonStr += "{ \"type\" : \"ping\" }";
	} else if (type == "version") {
		jsonStr += "{ \"type\" : \"version\" }";
	} else {
		auto quilStr = visitor->getQuilString();
		boost::replace_all(quilStr, "\n", "\\n");

		// Create the Json String
		jsonStr += "{ \"type\" : \"" + type + "\", \"addresses\" : "
				+ visitor->getClassicalAddresses()
				+ ", \"quil-instructions\" : \"" + quilStr
				+ (type == "wavefunction" ? "" : "\", \"trials\" : " + trials)
				+ " }";

	}

	headers.insert(std::make_pair("Content-type", "application/json"));
	headers.insert(std::make_pair("Accept", "application/octet-stream"));
	headers.insert(std::make_pair("x-api-key", apiKey));


	XACCInfo("Rigetti Json Payload = " + jsonStr);
	auto httpClient = std::make_shared<
			fire::util::AsioNetworkingTool<SimpleWeb::HTTPS>>(
			"api.rigetti.com", false);
	bool success = httpClient->post("/qvm", jsonStr, headers);

	if (!success) {
		XACCError("Error in HTTPS Post.\n" + httpClient->getLastStatusCode() + "\n" + httpClient->getLastRequestMessage());
	}

	XACCInfo("\n\tSuccessful HTTP Post to Rigetti.\n\t" + httpClient->getLastRequestMessage());
}

}
}

