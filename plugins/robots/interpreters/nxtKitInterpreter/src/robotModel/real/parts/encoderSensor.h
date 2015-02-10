#pragma once

#include <interpreterBase/robotModel/robotParts/encoderSensor.h>
#include <utils/robotCommunication/robotCommunicator.h>

namespace nxtKitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class EncoderSensor : public interpreterBase::robotModel::robotParts::EncoderSensor
{
	Q_OBJECT

public:
	EncoderSensor(const interpreterBase::robotModel::DeviceInfo &info
			, const interpreterBase::robotModel::PortInfo &port
			, utils::robotCommunication::RobotCommunicator &robotCommunicator);

	void read() override;
	void nullify() override;

private slots:
	void readingDone(QObject *addressee, const QByteArray &reading);

private:
	enum State {
		idle
		, pending
	};

	void sensorSpecificProcessResponse(const QByteArray &reading);

	char lowLevelPort() const;

	utils::robotCommunication::RobotCommunicator &mRobotCommunicator;
	State mState;
};

}
}
}
}
