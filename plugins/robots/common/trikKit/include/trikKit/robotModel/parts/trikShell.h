#pragma once

#include <kitBase/robotModel/robotParts/device.h>

namespace trik {
namespace robotModel {
namespace parts {

/// Executes shell commands on TRIK controller
class TrikShell : public kitBase::robotModel::robotParts::Device
{
	Q_OBJECT
	Q_CLASSINFO("name", "shell")
	Q_CLASSINFO("friendlyName", tr("Shell"))
	Q_CLASSINFO("direction", "output")

public:
	/// Constructor, takes device type info and port on which this motor is configured.
	TrikShell(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port);

	/// Run current command in the trik
	virtual void runCommand(const QString &command) = 0;

	/// Force trik say the text
	virtual void say(const QString &text) = 0;
};

}
}
}
