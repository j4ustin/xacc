/*******************************************************************************
 * Copyright (c) 2017 UT-Battelle, LLC.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompanies this
 * distribution. The Eclipse Public License is available at
 * http://www.eclipse.org/legal/epl-v10.html and the Eclipse Distribution License
 * is available at https://eclipse.org/org/documents/edl-v10.php
 *
 * Contributors:
 *   Alexander J. McCaskey - initial API and implementation
 *******************************************************************************/
#ifndef QUANTUM_GATE_IR_X_HPP_
#define QUANTUM_GATE_IR_X_HPP_

#include "GateInstruction.hpp"

namespace xacc {
namespace quantum {

/**
 *
 */
class X : public virtual GateInstruction {
public:
	X(std::vector<int> qbit);

	X(int qbit);

//	virtual void accept(std::shared_ptr<InstructionVisitor> visitor);

	DEFINE_VISITABLE()

};


}
}

#endif
