#include "button.h"

using namespace trikKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

Button::Button(DeviceInfo const &info, PortInfo const &port)
	: robotParts::Button(info, port)
{
}

void Button::read()
{
	QString const pathToCommand = ":/trik/templates/" + port.name() + ".t";
	qDebug() << pathToCommand << "\n";
	QString directCommand = utils::InFile::readAll(pathToCommand) + "brick.run()";
	utils::TcpRobotCommunicator tcpRobotCommunicator("TrikTcpServer");
	tcpRobotCommunicator.runDirectCommand(directCommand);
	emit newData(1);
}
