#pragma once

#include <robotModel/trikRobotModelBase.h>

namespace trikKitInterpreter {
namespace robotModel {
namespace real {

class RealRobotModelBase : public TrikRobotModelBase
{
	Q_OBJECT

public:
	RealRobotModelBase();

	bool needsConnection() const override;
	void connectToRobot() override;
	void disconnectFromRobot() override;

protected:
	interpreterBase::robotModel::robotParts::Device *createDevice(
			interpreterBase::robotModel::PortInfo const &port
			, interpreterBase::robotModel::DeviceInfo const &deviceInfo) override;
};

}
}
}
