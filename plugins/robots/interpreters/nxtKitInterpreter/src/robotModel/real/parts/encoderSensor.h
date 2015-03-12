#pragma once

#include <kitBase/robotModel/robotParts/encoderSensor.h>
#include <utils/robotCommunication/robotCommunicator.h>

namespace nxt {
namespace robotModel {
namespace real {
namespace parts {

class EncoderSensor : public kitBase::robotModel::robotParts::EncoderSensor
{
	Q_OBJECT

public:
	EncoderSensor(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port
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
