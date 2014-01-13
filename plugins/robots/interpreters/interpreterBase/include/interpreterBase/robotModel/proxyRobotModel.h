#pragma once

#include <QtCore/QScopedPointer>

#include "interpreterBase/robotModel/robotModelInterface.h"

#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {
namespace robotModel {

class ROBOTS_INTERPRETER_BASE_EXPORT ProxyRobotModel : public RobotModelInterface
{
public:
	void init() override;
	void stopRobot() override;
	void disconnectFromRobot() override;

	bool needsConnection() const override;

	ConfigurationInterface &configuration() override;

	robotParts::Brick &brick() override;
	robotParts::Display &display() override;

	virtual QList<interpreterBase::robotModel::PortInfo> availablePorts() const override;
	virtual QList<interpreterBase::robotModel::PluggableDeviceInfo> supportedSensors() const override;

private:
	QScopedPointer<RobotModelInterface> mProxiedModel;
};

}
}
