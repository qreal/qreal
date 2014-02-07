#pragma once

#include <QtCore/QObject>

#include "interpreterBase/robotModel/configurationInterface.h"
#include "interpreterBase/robotModel/portInfo.h"
#include "interpreterBase/robotModel/robotParts/pluggableDevice.h"
#include "interpreterBase/robotModel/robotParts/brick.h"
#include "interpreterBase/robotModel/robotParts/display.h"
#include "interpreterBase/robotModel/pluggableDeviceInfo.h"

#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {
namespace robotModel {

class ROBOTS_INTERPRETER_BASE_EXPORT RobotModelInterface : public QObject
{
	Q_OBJECT

public:
	virtual QString name() const = 0;
	virtual QString friendlyName() const = 0;

	virtual void init() = 0;
	virtual void stopRobot() = 0;
	virtual void disconnectFromRobot() = 0;

	virtual bool needsConnection() const = 0;

	/// \todo Don't like this, breaks incapsulation.
	virtual ConfigurationInterface const &configuration() const = 0;

	/// \todo Don't like this, breaks incapsulation.
	virtual ConfigurationInterface &mutableConfiguration() = 0;

	virtual robotParts::Brick &brick() = 0;
	virtual robotParts::Display &display() = 0;

	virtual QList<PortInfo> availablePorts() const = 0;

	/// Returns a list of ports that are allowed to be configured by user.
	/// \todo Why it is needed? We have allowedDevices() method, if it returns more than one device for a given port
	///       and a given direction, it shall be configured.
	virtual QList<PortInfo> configurablePorts() const = 0;

	/// Returns a list of devices that are allowed to be connected on a given port.
	virtual QList<PluggableDeviceInfo> allowedDevices(PortInfo const &port) const = 0;

	virtual void configureDevice(PortInfo const &port, PluggableDeviceInfo const &deviceInfo) = 0;

public slots:
	virtual void rereadSettings() = 0;

signals:
	/// Emitted when model is connected to a robot. If there is no need to connect (for example, 2d model), emitted
	/// immediately after init() call.
	/// @param success - true, if connected successfully.
	void connected(bool success);

	/// Emitted when robot is disconnected.
	void disconnected();
};

}
}
