#pragma once

#include <trikKit/robotModel/parts/trikGamepadConnectionIndicator.h>
#include <utils/tcpRobotCommunicator.h>

namespace trik {
namespace robotModel {
namespace real {
namespace parts {

/// Implementation of TRIK Android gamepad connection indicator for interpretation mode on real robot.
class GamepadConnectionIndicator : public robotModel::parts::TrikGamepadConnectionIndicator
{
	Q_OBJECT

public:
	/// Constructor.
	/// @param info - device info for that device.
	/// @param port - port on which this device is configured.
	/// @param tcpRobotCommunicator - communicator object that is used to send/receive data to/from telemetry
	///        service on a robot.
	GamepadConnectionIndicator(const kitBase::robotModel::DeviceInfo &info
		, const kitBase::robotModel::PortInfo &port
		, utils::TcpRobotCommunicator &tcpRobotCommunicator);

	void read() override;

private slots:
	/// Called when new data arrived from robot.
	void onIncomingData(const QString &portName, int value);

private:
	/// Communicator object that is used to send/receive data to/from telemetry service on a robot.
	utils::TcpRobotCommunicator &mRobotCommunicator;
};

}
}
}
}
