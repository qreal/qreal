#pragma once

#include <QtCore/QHash>
#include <QtCore/QSet>

#include "kitBase/robotModel/configurationInterface.h"

#include "kitBase/kitBaseDeclSpec.h"

namespace kitBase {
namespace robotModel {

/// Provides current configuration (devices connected to ports and ready to work) and ability to manage it.
class ROBOTS_KIT_BASE_EXPORT Configuration : public ConfigurationInterface
{
	Q_OBJECT

public:
	~Configuration() override;

	void configureDevice(robotParts::Device * const device) override;

	void applyConfiguration() override;

	QList<robotParts::Device *> devices() const override;

	robotParts::Device *device(const PortInfo &port) const override;

	void clearDevice(const PortInfo &port) override;

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
