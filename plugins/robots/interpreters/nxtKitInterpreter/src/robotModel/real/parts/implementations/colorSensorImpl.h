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

#include <kitBase/robotModel/robotParts/colorSensor.h>

#include "src/robotModel/real/parts/nxtInputDevice.h"

namespace nxt {
namespace robotModel {
namespace real {
namespace parts {

class ColorSensorImpl : public QObject
{
	Q_OBJECT

public:
	ColorSensorImpl(const kitBase::robotModel::PortInfo &port
			, utils::robotCommunication::RobotCommunicator &robotCommunicator
			, enums::lowLevelSensorType::SensorTypeEnum lowLevelType);

	void read();
	void doConfiguration();

signals:
	void newData(int reading);
	void configurationCompleted(bool success);
	void failure();

private slots:
	void sensorSpecificProcessResponse(const QByteArray &reading);

private:
	NxtInputDevice mImplementation;
	enums::lowLevelSensorType::SensorTypeEnum mLowLevelType;
};

}
}
}
}
