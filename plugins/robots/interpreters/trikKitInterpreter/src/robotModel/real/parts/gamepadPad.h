#pragma once

#include <utils/tcpRobotCommunicator.h>

#include "robotModel/parts/trikGamepadPad.h"

namespace trikKitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class GamepadPad : public robotModel::parts::TrikGamepadPad
{
	Q_OBJECT

public:
	GamepadPad(interpreterBase::robotModel::DeviceInfo const &info
		, interpreterBase::robotModel::PortInfo const &port
		, utils::TcpRobotCommunicator &tcpRobotCommunicator);

	void read() override;

private slots:
	void onIncomingData(const QString &portName, const QVector<int> &value);

private:
	utils::TcpRobotCommunicator &mRobotCommunicator;
};

}
}
}
}
