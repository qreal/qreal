#include "shell.h"
#include <qrutils/inFile.h>

using namespace trikKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

Shell::Shell(DeviceInfo const &info, PortInfo const &port
		, utils::TcpRobotCommunicator &tcpRobotCommunicator)
	: robotModel::parts::TrikShell(info, port)
	, mRobotCommunicator(tcpRobotCommunicator)
{
}

void Shell::say(QString const &text)
{
	QString const pathToCommand = ":/trikQts/templates/say.t";
	QString const directCommand = utils::InFile::readAll(pathToCommand)
			.replace("@@TEXT@@", text) + "brick.run();";

	mRobotCommunicator.runDirectCommand(directCommand);
}


void Shell::runCommand(QString const &command)
{
	QString const pathToCommand = ":/trikQts/templates/system.t";
	QString const directCommand = utils::InFile::readAll(pathToCommand)
			.replace("@@COMMAND@@", command) + "brick.run();";

	mRobotCommunicator.runDirectCommand(directCommand);
}
