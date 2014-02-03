#pragma once

#include "interpreterBase/robotModel/robotModelInterface.h"
#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {
namespace robotModel {

class ROBOTS_INTERPRETER_BASE_EXPORT CommonRobotModel : public RobotModelInterface
{
	Q_OBJECT

public:
	CommonRobotModel();
	~CommonRobotModel() override;

	QString name() const override;
	QString friendlyName() const override;

	void init() override;
	void stopRobot() override;
	void disconnectFromRobot() override;

	bool needsConnection() const override;

	ConfigurationInterface &configuration() override;  // TODO: Don't like this, breaks incapsulation.

	robotParts::Brick &brick() override;
	robotParts::Display &display() override;

	QList<PortInfo> availablePorts() const override;
	QList<PortInfo> configurablePorts() const override;
	QList<PluggableDeviceInfo> allowedDevices(PortInfo const &port) const override;

protected:
	QHash<interpreterBase::robotModel::PortInfo
			, QList<interpreterBase::robotModel::PluggableDeviceInfo> > mAllowedConnections;

};

}
}
