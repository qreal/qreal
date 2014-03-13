#pragma once

#include <interpreterBase/robotModel/robotParts/colorSensor.h>
#include "nxtInputDevice.h"

namespace nxtKitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class ColorSensor : public interpreterBase::robotModel::robotParts::ColorSensor
{
	Q_OBJECT

public:
	ColorSensor(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port
			, utils::robotCommunication::RobotCommunicator &robotCommunicator
			, enums::lowLevelSensorType::SensorTypeEnum lowLevelType);

	void read() override;
	void doConfiguration() override;

private slots:
	void sensorSpecificProcessResponse(QByteArray const &reading);

private:
	NxtInputDevice mImplementation;
	enums::lowLevelSensorType::SensorTypeEnum mLowLevelType;
};

}
}
}
}
