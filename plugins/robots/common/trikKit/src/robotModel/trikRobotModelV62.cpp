/* Copyright 2015 CyberTech Labs Ltd.
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

#include "trikKit/robotModel/trikRobotModelV62.h"

#include <kitBase/robotModel/robotParts/encoderSensor.h>

using namespace trik::robotModel;
using namespace kitBase::robotModel;

TrikRobotModelV62::TrikRobotModelV62(const QString &kitId, const QString &robotId)
	: TrikRobotModelBase(kitId, robotId)
{
	addAllowedConnection(PortInfo("S1", output, {}), { servoMotorInfo() });
	addAllowedConnection(PortInfo("S2", output, {}), { servoMotorInfo() });
	addAllowedConnection(PortInfo("S3", output, {}), { servoMotorInfo() });
	addAllowedConnection(PortInfo("S4", output, {}), { servoMotorInfo() });
	addAllowedConnection(PortInfo("S5", output, {}), { servoMotorInfo() });
	addAllowedConnection(PortInfo("S6", output, {}), { servoMotorInfo() });

	addAllowedConnection(PortInfo("E1", input, { "M1", "Е1" }, "encoder1"), { encoderInfo() });
	addAllowedConnection(PortInfo("E2", input, { "M2", "Е2" }, "encoder2"), { encoderInfo() });
	addAllowedConnection(PortInfo("E3", input, { "M3", "Е3" }, "encoder3"), { encoderInfo() });
	addAllowedConnection(PortInfo("E4", input, { "M4", "Е4" }, "encoder4"), { encoderInfo() });
}
