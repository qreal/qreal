#pragma once

#include "robotModel/parts/trikLineSensor.h"
#include <utils/tcpRobotCommunicator.h>

namespace trikKitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

/// Stub for TRIK color sensor for interpreter.
class LineSensor : public robotModel::parts::TrikLineSensor
{
	Q_OBJECT

public:
	LineSensor(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port
			, utils::TcpRobotCommunicator &tcpRobotCommunicator);

	void init() override;
	void detectLine() override;
	void read() override;

private:
	utils::TcpRobotCommunicator &mRobotCommunicator;
};

}
}
}
}
