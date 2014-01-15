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
	virtual ~CommonRobotModel();

	QString name() const override;

	void init() override;
	void stopRobot() override;
	void disconnectFromRobot() override;

	bool needsConnection() const override;

	ConfigurationInterface &configuration() override;  // TODO: Don't like this, breaks incapsulation.

	robotParts::Brick &brick() override;
	robotParts::Display &display() override;

	virtual QList<PortInfo> availablePorts() const override;
	virtual QList<PluggableDeviceInfo> supportedSensors() const override;
};

}
}
