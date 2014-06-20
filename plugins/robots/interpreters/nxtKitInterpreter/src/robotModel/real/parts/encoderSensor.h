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
	EncoderSensor(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port
			, utils::robotCommunication::RobotCommunicator &robotCommunicator);

	void read() override;
	void nullify() override;

private slots:
	void readingDone(QObject *addressee, QByteArray const &reading);

private:
	enum State {
		idle
		, pending
	};

	void sensorSpecificProcessResponse(QByteArray const &reading);

	char lowLevelPort() const;

	utils::robotCommunication::RobotCommunicator &mRobotCommunicator;
	State mState;
};

}
}
}
}
