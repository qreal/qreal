#pragma once

#include <QtCore/QScopedPointer>

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

	void init() final;

	/// Shall be implemented in descendants to stop all robot devices. Default implementation does nothing.
	void stopRobot() override;

	void disconnectFromRobot() override;

	ConfigurationInterface &mutableConfiguration() final;
	ConfigurationInterface const &configuration() const final;

	robotParts::Brick &brick() final;
	robotParts::Display &display() final;

	QList<PortInfo> availablePorts() const override;
	QList<PortInfo> configurablePorts() const override;
	QList<PluggableDeviceInfo> allowedDevices(PortInfo const &port) const override;

protected:
	void setBrick(robotParts::Brick *brick);  // Takes ownership.
	void setDisplay(robotParts::Display *display);  // Takes ownership.
	void addAllowedConnection(PortInfo const &port, QList<PluggableDeviceInfo> const &devices);

private:
	/// Shall be implemented in descendants to establish connection to a real robot. Default implementation emits
	/// connected() immediately, which is fine if model doesn't need connection.
	/// Method shall return immediately, and when connection is made, connected() signal shall be emitted.
	virtual void connectToRobot();

	/// Shall be implemented in descendants to add to configuration devices that can not be changed by user
	/// and shall be in a model every time (like Lego motors). Default implementation does nothing.
	virtual void configureKnownDevices();

	QHash<PortInfo, QList<PluggableDeviceInfo>> mAllowedConnections;

	/// Robot control block (or brick). Shall be set in descendants to an actual implementation by setBrick() method.
	QScopedPointer<robotParts::Brick> mBrick;

	/// Robot display. Shall be set in descendants to an actual implementation by setDisplay() method.
	QScopedPointer<robotParts::Display> mDisplay;

	/// Devices configuration.
	Configuration mConfiguration;
};

}
}
