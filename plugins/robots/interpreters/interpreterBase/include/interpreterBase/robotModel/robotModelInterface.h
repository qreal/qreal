#pragma once

#include <QtCore/QObject>

#include "interpreterBase/robotModel/configurationInterface.h"
#include "interpreterBase/robotModel/portInfo.h"
#include "interpreterBase/robotModel/robotParts/device.h"
#include "interpreterBase/robotModel/deviceInfo.h"

#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace utils {
class TimelineInterface;
}

namespace interpreterBase {
namespace robotModel {

/// Represents abstract robot model with general properties that every kit has, like the ability to connect to robot,
/// configure devices or enumerate available ports. It can be specialized in kit plugins to provide kit-specific
/// functionality and to define kit-specific methods like establishing connection.
///
/// Model can be connected or disconnected to robot, when it is disconnected, it will queue up device
/// configuration changes and ignore requests involving physical devices. Some models (like 2d model) are assumed to be
/// always connected.
///
/// Life cycle of a model is as follows:
/// - Model is created in Disconnected state
/// - Model is initialized (by calling init() method), which loads plugin-specific info that can not be loaded in
///   constructor.
/// - Model connects to a real robot by calling connectToRobot(). When connection is established
///   (or failed to establish), model enters Connected state and emits connected() signal.
/// - Model is then configured by configureDevice() calls which add to the current configuration devices that can
///   be selected by user. Some devices can be configured by model itself, like display or speaker.
/// - Configuration is applied by applyConfiguration() call. It queues current configuration to be uploaded to actual
///   robot when model is connected, or, if it is already connected, uploads it to robot immediately.
///   After all devices respond that they are configured, model emits allDevicesConfigured(). Some devices may fail to
///   configure for some reason, so actual configuration status shall be retrieved by configuration() call.
/// - After allDevicesConfigured() signal model is ready to work and accept commands to a robot. Configuration still
///   can be changed "on the fly" by configureDevice() and applyConfiguration() calls, model tries to reconfigure
///   device and emits allDevicesConfigured() when done.
/// - Robot can be disconnected for some internal reason (for example, by physical connection loss) or by calling
///   disconnectFromRobot() method. Anyway, model enters Disconnected state and emits disconnected() signal.
///   Model can be reconnected again by calling connectToRobot().
/// All implementations are required to emit allDevicesConfigured() after connected().
class ROBOTS_INTERPRETER_BASE_EXPORT RobotModelInterface : public QObject
{
	Q_OBJECT

public:
	/// State of a model.
	enum ConnectionState {
		/// Model is connected to a robot. Note that it doesn't mean that it is ready to work, since devices may
		/// not be configured yet.
		connectedState

		/// Model is disconnected.
		, disconnectedState
	};

	/// Destructor.
	virtual ~RobotModelInterface() {}

	/// Returns internal name (id) of a model. Used to identify a model in kit manager, configuration and so on.
	/// Shall be unique.
	virtual QString name() const = 0;

	/// Returns user name of a model, which is shown on settings page.
	virtual QString friendlyName() const = 0;

	/// Additional model initialization that shall be done after constructor, used to call virtual methods of descendant
	/// models.
	virtual void init() = 0;

	/// Tries to connect to robot. Shall emit connected() when done, can emit immediately (in the same stack) or after
	/// some time.
	virtual void connectToRobot() = 0;

	/// Disconnects from robot. Emits disconnected() signal when done.
	virtual void disconnectFromRobot() = 0;

	/// Stops robot (turns off all motors and sensors).
	virtual void stopRobot() = 0;

	/// Returns false if model is always connected to a robot and there is no need for special order to connect from
	/// user (like 2d model).
	virtual bool needsConnection() const = 0;

	/// Returns current connection state.
	virtual ConnectionState connectionState() const = 0;

	/// Returns current device configuration which is used to list all configured devices and use them.
	virtual ConfigurationInterface const &configuration() const = 0;

	/// Lists all available ports for that model.
	virtual QList<PortInfo> availablePorts() const = 0;

	/// Returns a list of ports that are allowed to be configured by user. Recommended implementation shall use
	/// allowedDevices() and make port configurable only when allowedDevices() allows some choice, but this method
	/// is provided to allow descendant models to hide some devices that actually can be configured.
	/// Main example is NXT, where on output port there can be a motor or a lamp. Motors are used almost every time,
	/// so we don't want to bother user with "lamp or motor" choice. Lamps still can be supported as separate devices
	/// and even configured by autoconfigurer, but user can not do that manually.
	virtual QList<PortInfo> configurablePorts() const = 0;

	/// Returns a list of devices that are allowed to be connected on a given port.
	virtual QList<DeviceInfo> allowedDevices(PortInfo const &port) const = 0;

	/// Adds a device on a given port to pending configuration. Configuration is actualized by applyConfiguration()
	/// call. If deviceInfo is empty, clears configuration on that port. If there is no such port, does nothing.
	virtual void configureDevice(PortInfo const &port, DeviceInfo const &deviceInfo) = 0;

	/// Uploads current configuration on a robot (as soon as it becomes connected). Emits allDevicesConfigured() when
	/// done (note that if some device does not respond, it may not emit this signal). Can emit immediately or after
	/// some time.
	/// @todo Add configuration timeout for devices and proper handling of configuration failure.
	virtual void applyConfiguration() = 0;

	/// Returns a list of devices types that can be used for 'can convert' decision.
	/// When user changes sensors configuration with some robot model as current it must be decided for
	/// other robot models sensors configuration whether sensors on the same port must be changed to
	/// convertible one or stayed the same. Convertible bases returned by this method are devices types
	/// that can be used for answering that question. Two devices are considered to be convertible if
	/// they both inherit the same convertible base.
	///
	/// Example: user selects light sensor for nxt 2D model. We must propagate that modification over every
	/// robot model`s sensors configuration. So, for example, for real Trik model we must set light sensor too.
	/// But how to find Nxt light sensor analogue for Trik? Here helps convertible bases provided by Trik plugin.
	/// Both light sensors inherit interpreterBase::robotModel::robotParts::LightSensor, so if this class is
	/// returned as convertible base we can simply convert Nxt`s light sensor to Trik`s one.
	virtual QList<DeviceInfo> convertibleBases() const = 0;

	/// Returns a timeline object that can return current timestamps (for this robot model time) and produce timers.
	virtual utils::TimelineInterface &timeline() = 0;

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
