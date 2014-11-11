#pragma once

#include "robotModel/parts/trikSonarSensor.h"
#include <utils/tcpRobotCommunicator.h>

namespace trikKitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class SonarSensor : public robotModel::parts::TrikSonarSensor
{
	Q_OBJECT

public:
	SonarSensor(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port
			, utils::TcpRobotCommunicator &tcpRobotCommunicator);

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
