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

/// Represents abstract robot model with general properties that every kit has, like the ability to connect to robot,
/// configure pluggable devices or enumerate available ports. It also returns common robot parts like brick or display.
/// It can be specialized in kit plugins to provide kit-specific functionality like new parts and to define kit-specific
/// methods like establishing connection.
///
/// Model can be connected or disconnected to robot, when it is disconnected, it will queue up pluggable device
/// configuration changes and ignore requests involving physical devices. Some models (like 2d model) are assumed to be
/// always connected.
///
/// Life cycle of a model is as follows:
/// - Model is created in Disconnected state, with empty configuration
/// - Model is initialized (by calling init() method), which fills configuration and loads plugin-specific info that can
///   not be loaded in constructor.
/// - Model connects to a real robot by calling connectToRobot(). When connection is established
///   (or failed to establish), model emits connected() signal and enters Connected state (note that exact sequence
///   of this is undefined, so it can at first enter Connected state and then emit connected(), or at first emit, then
///   change state).
/// - If connection is successful, model will be able to perform configuration of its pluggable devices by
///   calling configureDevices(), when it is done it emits allDevicesConfigured() signal. Some devices may fail to
///   configure for some reason, so actual configuration status shall be retrieved by configuration() call.
/// - After allDevicesConfigured() signal model is ready to work and accept commands to a robot. Configuration still
///   can be changed "on the fly" by configureDevices() calls, model tries to reconfigure device and emits
///   allDevicesConfigured() when done.
/// - Robot can be disconnected for some internal reason (for example, by physical connection loss) or by calling
///   disconnectFromRobot() method. Anyway, model enters Disconnected state. Model can be reconnected by calling
///   connectToRobot().
class ROBOTS_INTERPRETER_BASE_EXPORT RobotModelInterface : public QObject
{
	Q_OBJECT

public:
	enum ConnectionState {
		connectedState
		, disconnectedState
	};

	virtual QString name() const = 0;
	virtual QString friendlyName() const = 0;

	/// @todo All configuration can be done in constructors of descendants?
	virtual void init() = 0;

	virtual void connectToRobot() = 0;
	virtual void disconnectFromRobot() = 0;
	virtual void stopRobot() = 0;

	virtual bool needsConnection() const = 0;

	virtual ConfigurationInterface const &configuration() const = 0;

	virtual QList<PortInfo> availablePorts() const = 0;

	/// Returns a list of ports that are allowed to be configured by user. Recommended implementation shall use
	/// allowedDevices() and make port configurable only when allowedDevices() allows some choice, but this method
	/// is provided to allow descendant models to hide some devices that actually can be configured.
	/// Main example is NXT, where on output port there can be a motor or a lamp. Motors are used almost every time,
	/// so we don't want to bother user with "lamp or motor" choice. Lamps still can be supported as separate devices
	/// and even configured by autoconfigurer, but user can not do that manually.
	/// @todo Maybe we don't need this?
	virtual QList<PortInfo> configurablePorts() const = 0;

	/// Returns a list of devices that are allowed to be connected on a given port.
	virtual QList<PluggableDeviceInfo> allowedDevices(PortInfo const &port) const = 0;

	virtual void configureDevices(QHash<PortInfo, PluggableDeviceInfo> const &devices) = 0;

	/// Returns a list of devices types that can be used for 'can convert' decision.
	/// When user changes sensors configuration with some robot model as current it must be decided for
	/// other robot models sensors configuration whether sensors on the same port must be changed to
	/// convertible one or stayed the same. Convertible bases returned by this method are devices types
	/// that can be used for answering that question. Two devices are considered to be convertible if
	/// they both inherit the same convertible base.
	/// Example: user selects light sensor for nxt 2D model. We must propagate that modification over every
	/// robot model`s sensors configuration. So, for example, for real Trik model we must set light sensor too.
	/// But how to find Nxt light sensor analogue for Trik? Here helps convertible bases provided by Trik plugin.
	/// Both light sensors inherit interpreterBase::robotModel::robotParts::LightSensor, so if this class is
	/// returned as convertible base we can simply convert Nxt`s light sensor to Trik`s one.
	virtual QList<PluggableDeviceInfo> convertibleBases() const = 0;

signals:
	/// Emitted when model is connected to a robot. If there is no need to connect (for example, 2d model), emitted
	/// immediately after connectToRobot() call.
	/// @param success - true, if connected successfully.
	void connected(bool success);

	/// Emitted when robot is disconnected.
	void disconnected();

	/// Emitted when all devices are configured (or failed to configure) and model is ready to work.
	void allDevicesConfigured();
};

}
}
