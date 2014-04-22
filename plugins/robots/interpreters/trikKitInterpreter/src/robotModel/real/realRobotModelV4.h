#pragma once

#include <robotModel/trikRobotModelBase.h>

namespace trikKitInterpreter {
namespace robotModel {
namespace real {

class RealRobotModelV4 : public TrikRobotModelBase
{
	Q_OBJECT

public:
	RealRobotModelV4();

	QString name() const override;
	QString friendlyName() const override;
	bool needsConnection() const override;

	void connectToRobot() override;
	void disconnectFromRobot() override;

public slots:
	void rereadSettings() override;

protected:
	interpreterBase::robotModel::robotParts::Device *createDevice(
			interpreterBase::robotModel::PortInfo const &port
			, interpreterBase::robotModel::DeviceInfo const &deviceInfo) override;
};

}
}
}
