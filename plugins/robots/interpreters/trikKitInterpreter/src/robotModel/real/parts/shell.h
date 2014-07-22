#pragma once

#include <robotModel/parts/trikShell.h>

namespace trikKitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class Shell : public robotModel::parts::TrikShell
{
	Q_OBJECT

public:
	Shell(interpreterBase::robotModel::DeviceInfo const &info, interpreterBase::robotModel::PortInfo const &port);

	void runCommand(QString const &command) override;

	void say(const QString &text) override;
};

}
}
}
}
