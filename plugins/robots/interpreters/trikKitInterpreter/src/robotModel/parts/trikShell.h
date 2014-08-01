#pragma once

#include <interpreterBase/robotModel/robotParts/device.h>

namespace trikKitInterpreter {
namespace robotModel {
namespace parts {

///Executes shell commands on TRIK controller
class ROBOTS_INTERPRETER_BASE_EXPORT TrikShell : public interpreterBase::robotModel::robotParts::Device
{
	Q_OBJECT
	Q_CLASSINFO("name", "shell")
	Q_CLASSINFO("friendlyName", tr("Shell"))
	Q_CLASSINFO("direction", "output")

public:
	/// Constructor, takes device type info and port on which this motor is configured.
	TrikShell(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port);

	/// Run current command in the trik
	virtual void runCommand(QString const &command) = 0;

	/// Force trik say the text
	virtual void say(QString const &text) = 0;
};

}
}
}
