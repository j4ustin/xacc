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
#include "X.hpp"

namespace xacc {
namespace quantum {

X::X(std::vector<int> qbit) :
		GateInstruction("X", qbit) {
}

X::X(int qbit) :
		X(std::vector<int> { qbit }) {
}

RegisterGateInstruction<X> XTEMP("X");

}
}

