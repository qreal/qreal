#pragma once

#include <QtCore/QHash>
#include <QtCore/QSet>

#include "interpreterBase/robotModel/configurationInterface.h"

#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {
namespace robotModel {

class ROBOTS_INTERPRETER_BASE_EXPORT Configuration : public ConfigurationInterface
{
	Q_OBJECT

public:
	Configuration();
	~Configuration() override;

	void configureDevice(robotParts::PluggableDevice * const device) override;

	void lockConfiguring() override;
	void unlockConfiguring() override;

	robotParts::PluggableDevice *pluggableDevice(
			PortInfo const &port
			, PortDirection direction = defaultDirection) const override;

	void clearDevice(PortInfo const &port) override;

private slots:
	void deviceConfiguredSlot(bool success);

private:
	void reconfigureDevices();

	/// Contains currently configured and ready devices.
	QHash<PortInfo, robotParts::PluggableDevice *> mConfiguredDevices;  // Has ownership.

	/// Contains devices that are not configured yet but are added for configuration and possibly already requested
	/// to configure themselves.
	QHash<PortInfo, robotParts::PluggableDevice *> mPendingDevices;  // Has ownership.

	/// Contains ports on which device is already requested to configure itself and we are waiting a signal about
	/// result of configuration.
	QSet<PortInfo> mConfigurationInProgress;

	/// True if configuration is deferred now. All configuration requests are queued.
	bool mLocked;
};

}
}
