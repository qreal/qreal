#pragma once

#include <QtCore/QHash>
#include <QtCore/QSet>

#include "interpreterBase/robotModel/configurationInterface.h"

#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {
namespace robotModel {

/// Provides current configuration (devices connected to ports and ready to work) and ability to manage it.
class ROBOTS_INTERPRETER_BASE_EXPORT Configuration : public ConfigurationInterface
{
	Q_OBJECT

public:
	Configuration();
	~Configuration() override;

	void configureDevice(robotParts::Device * const device) override;

	void applyConfiguration() override;

	QList<robotParts::Device *> devices(PortDirection direction = defaultDirection) const override;

	robotParts::Device *device(PortInfo const &port, PortDirection direction = defaultDirection) const override;

	void clearDevice(PortInfo const &port) override;

signals:
	/// Emitted when all pending devices are configured (or failed to configure).
	void allDevicesConfigured();

private slots:
	/// Called when some device is finished configuring.
	/// @param success - true if configuration was successful.
	void onDeviceConfigured(bool success);

private:
	/// Check that there are no pending or "in progress" requests and emit allDevicesConfigured() if it is true.
	void checkAllDevicesConfigured();

	/// Contains currently configured and ready devices.
	QHash<PortInfo, robotParts::Device *> mConfiguredDevices;  // Has ownership.

	/// Contains devices that are not configured yet but are added for configuration and possibly already requested
	/// to configure themselves.
	QHash<PortInfo, robotParts::Device *> mPendingDevices;  // Has ownership.

	/// Contains ports on which device is already requested to configure itself and we are waiting a signal about
	/// result of configuration.
	QSet<PortInfo> mConfigurationInProgress;
};

}
}
