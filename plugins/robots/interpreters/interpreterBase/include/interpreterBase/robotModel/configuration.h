#pragma once

#include <QtCore/QHash>

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

	void configureDevice(robotParts::PluggableDevice const &device, PortInfo const &port) override;

	void lockConfiguring() override;
	void unlockConfiguring() override;

	robotParts::PluggableDevice *pluggableDevice(
			PortInfo const &port
			, PortDirection direction = defaultDirection) const override;

	void clearDevice(PortInfo const &port) override;

private slots:
	void deviceConfiguredSlot(PortInfo const &port);

private:
	void reconfigureDevices();

	QHash<PortInfo, robotParts::PluggableDevice *> mConfiguredDevices;  // Has ownership.
	QHash<PortInfo, robotParts::PluggableDevice *> mPendingDevices;  // Has ownership.

	bool mLocked;
};

}
}
