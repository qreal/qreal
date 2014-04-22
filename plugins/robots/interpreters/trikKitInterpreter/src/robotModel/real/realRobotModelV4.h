#pragma once

#include <robotModel/trikV4RobotModelBase.h>

namespace trikKitInterpreter {
namespace robotModel {
namespace real {

class RealRobotModel : public TrikV4RobotModelBase
{
	Q_OBJECT

public:
	RealRobotModel();

	QString name() const override;
	QString friendlyName() const override;
	bool needsConnection() const override;

	void connectToRobot() override;
	void disconnectFromRobot() override;

public slots:
	void rereadSettings() override;

private:
	interpreterBase::robotModel::robotParts::Device *createDevice(
			interpreterBase::robotModel::PortInfo const &port
			, interpreterBase::robotModel::DeviceInfo const &deviceInfo) override;
};

}
}
}
