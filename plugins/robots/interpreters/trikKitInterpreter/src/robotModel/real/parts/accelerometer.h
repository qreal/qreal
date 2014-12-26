#pragma once

#include <interpreterBase/robotModel/robotParts/accelerometerSensor.h>
#include <utils/tcpRobotCommunicator.h>

namespace trikKitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class Accelerometer : public interpreterBase::robotModel::robotParts::AccelerometerSensor
{
	Q_OBJECT

public:
	Accelerometer(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port
			, utils::TcpRobotCommunicator &robotCommunicator);

	void read() override;

public slots:
	void onIncomingData(QString const &portName, int value);

private:
	utils::TcpRobotCommunicator &mRobotCommunicator;
};

}
}
}
}
