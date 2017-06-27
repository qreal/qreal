/* Copyright 2017 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#include <QtCore/QString>

namespace pioneer {

/// Names for robot models supported by Pioneer plugins.
namespace modelNames {
const QString realCopter = "PioneerRealCopter";
const QString simulator = "PioneerSimulator";
}

/// Names for used global settings.
namespace settings {

/// Path to a Lua compiler that is used to produce binaries for real copter.
const QString realCopterLuaPath = "realCopterLuaPath";

/// Path to a Lua compiler that is used to produce binaries for a simulator.
const QString simulatorLuaPath = "simulatorLuaPath";

/// IP address of a Pioneer base station.
const QString pioneerBaseStationIP = "pioneerBaseStationIP";

/// Path to a Python interpreter.
const QString pioneerPythonPath = "pioneerPythonPath";

}

}
