#pragma once

#include <interpreterBase/robotModel/robotParts/rangeSensor.h>
#include "nxtInputDevice.h"
#include "communication/i2cCommunicator.h"

namespace nxtKitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class SonarSensor : public interpreterBase::robotModel::robotParts::RangeSensor
{
	Q_OBJECT
	Q_CLASSINFO("friendlyName", tr("Sonar sensor"))

public:
	SonarSensor(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port
			, utils::robotCommunication::RobotCommunicator &robotCommunicator);

	void read() override;
	void doConfiguration() override;

private slots:
	void sensorSpecificProcessResponse(QByteArray const &reading);

private:
	void setMode(enums::sonarMode::SonarModeEnum mode);
	void writeRegister(enums::sonarRegisters::SonarRegistersEnum reg, int value);
	void sendCommand(QByteArray const &command, int responseSize);

	NxtInputDevice mImplementation;
	communication::I2CCommunicator mI2C;
};

}
}
}
}
