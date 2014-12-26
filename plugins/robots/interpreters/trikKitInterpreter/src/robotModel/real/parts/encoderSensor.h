#pragma once

#include <interpreterBase/robotModel/robotParts/encoderSensor.h>
#include <utils/tcpRobotCommunicator.h>

namespace trikKitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class EncoderSensor : public interpreterBase::robotModel::robotParts::EncoderSensor
{
	Q_OBJECT

public:
	EncoderSensor(interpreterBase::robotModel::DeviceInfo const &info
		, interpreterBase::robotModel::PortInfo const &port
		, utils::TcpRobotCommunicator &tcpRobotCommunicator);

	void read() override;
	void nullify() override;

public slots:
	void onIncomingData(QString const &portName, int value);

private:
	utils::TcpRobotCommunicator &mRobotCommunicator;
};

}
}
}
}
