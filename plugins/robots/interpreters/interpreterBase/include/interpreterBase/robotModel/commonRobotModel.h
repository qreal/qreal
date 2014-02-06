#pragma once

#include "interpreterBase/robotModel/robotModelInterface.h"
#include "interpreterBase/robotModel/configuration.h"
#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {
namespace robotModel {

class ROBOTS_INTERPRETER_BASE_EXPORT CommonRobotModel : public RobotModelInterface
{
	Q_OBJECT

public:
	CommonRobotModel();
	~CommonRobotModel() override;

	void init() override;
	void stopRobot() override;
	void disconnectFromRobot() override;

	ConfigurationInterface &mutableConfiguration() override;
	ConfigurationInterface const &configuration() const override;

	robotParts::Brick &brick() override;
	robotParts::Display &display() override;

	QList<PortInfo> availablePorts() const override;
	QList<PortInfo> configurablePorts() const override;
	QList<PluggableDeviceInfo> allowedDevices(PortInfo const &port) const override;

protected:
	QHash<PortInfo, QList<PluggableDeviceInfo>> mAllowedConnections;

	robotParts::Brick mBrick;

	robotParts::Display mDisplay;

	/// \todo implement
	Configuration mConfiguration;
};

}
}
