#pragma once

#include <QtCore/QScopedPointer>

#include "interpreterBase/robotModel/robotModelInterface.h"

#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {
namespace robotModel {

class ROBOTS_INTERPRETER_BASE_EXPORT ProxyRobotModel : public RobotModelInterface
{
public:
	RobotModelInterface *proxiedModel() const;
	void setProxiedModel(RobotModelInterface * const robotModel);

	QString name() const override;
	QString friendlyName() const override;

	void init() override;
	void stopRobot() override;
	void disconnectFromRobot() override;

	bool needsConnection() const override;

	ConfigurationInterface &configuration() override;

	robotParts::Brick &brick() override;
	robotParts::Display &display() override;

	QList<PortInfo> availablePorts() const override;
	QList<PortInfo> configurablePorts() const override;
	QList<PluggableDeviceInfo> allowedDevices(PortInfo const &port) const override;

private:
	RobotModelInterface *mProxiedModel;  // Does not take ownership
};

}
}
