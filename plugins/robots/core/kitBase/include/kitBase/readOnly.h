/* Copyright 2007-2015 QReal Research Group
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

#include <QtCore/QFlags>

namespace kitBase {

/// Flags that determine allowed interaction of 2d model widget contents with user.
/// Enum is "plain enum" because it is used as flags, enum class is not interpreted as int.
/// @todo It is implementation details of 2d model engine, shall not be in "include" folder.
namespace ReadOnly {
enum ReadOnlyEnum {
	/// No read-only parts.
	None = 0

	/// World model is read-only, so walls, colored lines and areas etc. can not be added, moved or deleted.
	, World = 1

	/// Robot sensor positions and directions are read-only, and user is not allowed to add or remove sensors.
	, Sensors = 2

	/// Robot starting position and direction is read-only.
	, RobotPosition = 4

	/// Robot model configuration parameters such as wheel to motor port bindings are read-only.
	, RobotSetup = 8

	/// Simulation parameters like used physics engine and simulation speed are read-only.
	, SimulationSettings = 16
};

}

Q_DECLARE_FLAGS(ReadOnlyFlags, ReadOnly::ReadOnlyEnum)
Q_DECLARE_OPERATORS_FOR_FLAGS(ReadOnlyFlags)

}
