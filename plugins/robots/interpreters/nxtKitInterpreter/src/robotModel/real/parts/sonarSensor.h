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
	SonarSensor(const interpreterBase::robotModel::DeviceInfo &info
			, const interpreterBase::robotModel::PortInfo &port
			, utils::robotCommunication::RobotCommunicator &robotCommunicator);

	void read() override;
	void doConfiguration() override;

private slots:
	void sensorSpecificProcessResponse(const QByteArray &reading);

private:
	void setMode(enums::sonarMode::SonarModeEnum mode);
	void writeRegister(enums::sonarRegisters::SonarRegistersEnum reg, int value);
	void sendCommand(const QByteArray &command, int responseSize);

	NxtInputDevice mImplementation;
	communication::I2CCommunicator mI2C;
};

}
}
}
}
