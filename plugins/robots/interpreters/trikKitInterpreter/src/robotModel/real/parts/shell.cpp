#include "shell.h"
#include <utils/tcpRobotCommunicator.h>
#include <qrutils/inFile.h>

using namespace trikKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

Shell::Shell(DeviceInfo const &info, PortInfo const &port)
	: robotModel::parts::TrikShell(info, port)
{
}

void Shell::say(QString const &text)
{
	QString const pathToCommand = ":/trik/templates/say.t";
	QString directCommand = utils::InFile::readAll(pathToCommand).replace("@@TEXT@@", text) + "brick.run()";
	utils::TcpRobotCommunicator tcpRobotCommunicator("TrikTcpServer");
	tcpRobotCommunicator.runDirectCommand(directCommand);
}


void Shell::runCommand(QString const &command)
{
	QString const pathToCommand = ":/trik/templates/system.t";
	QString directCommand = utils::InFile::readAll(pathToCommand).replace("@@COMMAND@@", command) + "brick.run()";
	utils::TcpRobotCommunicator tcpRobotCommunicator("TrikTcpServer");
	tcpRobotCommunicator.runDirectCommand(directCommand);
}
