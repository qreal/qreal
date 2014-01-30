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

	void init() override;
	void stopRobot() override;
	void disconnectFromRobot() override;

	bool needsConnection() const override;

	ConfigurationInterface const &configuration() const override;
	ConfigurationInterface &mutableConfiguration() override;

	robotParts::Brick &brick() override;
	robotParts::Display &display() override;

	virtual QList<interpreterBase::robotModel::PortInfo> availablePorts() const override;
	virtual QList<interpreterBase::robotModel::PluggableDeviceInfo> supportedSensors() const override;

private:
	RobotModelInterface *mProxiedModel;  // Does not take ownership
};

}
}
