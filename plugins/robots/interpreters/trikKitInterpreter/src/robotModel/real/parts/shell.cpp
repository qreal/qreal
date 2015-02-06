#include "shell.h"
#include <qrutils/inFile.h>

using namespace trikKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

Shell::Shell(const DeviceInfo &info, const PortInfo &port
		, utils::TcpRobotCommunicator &tcpRobotCommunicator)
	: robotModel::parts::TrikShell(info, port)
	, mRobotCommunicator(tcpRobotCommunicator)
{
}

void Shell::say(const QString &text)
{
	const QString pathToCommand = ":/trikQts/templates/say.t";
	const QString directCommand = utils::InFile::readAll(pathToCommand)
			.replace("@@TEXT@@", "\"" + text + "\"") + "brick.run();";

	mRobotCommunicator.runDirectCommand(directCommand);
}

void Shell::runCommand(const QString &command)
{
	const QString pathToCommand = ":/trikQts/templates/system.t";
	const QString directCommand = utils::InFile::readAll(pathToCommand)
			.replace("@@COMMAND@@", command) + "brick.run();";

	mRobotCommunicator.runDirectCommand(directCommand);
}
