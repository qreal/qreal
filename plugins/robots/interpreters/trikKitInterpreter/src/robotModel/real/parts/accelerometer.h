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
	Accelerometer(const interpreterBase::robotModel::DeviceInfo &info
			, const interpreterBase::robotModel::PortInfo &port
			, utils::TcpRobotCommunicator &robotCommunicator);

	void read() override;

public slots:
	void onIncomingData(const QString &portName, int value);

private:
	utils::TcpRobotCommunicator &mRobotCommunicator;
};

}
}
}
}
