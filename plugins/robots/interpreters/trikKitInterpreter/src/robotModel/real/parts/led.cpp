#include "led.h"
#include <utils/tcpRobotCommunicator.h>
#include <qrutils/inFile.h>
#include <QColor>

using namespace trikKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

Led::Led(DeviceInfo const &info, PortInfo const &port)
	: robotModel::parts::TrikLed(info, port)
{
}

void Led::setColor(const QString &color)
{
	QString const pathToCommand = ":/trik/templates/led.t";
	QString directCommand = utils::InFile::readAll(pathToCommand).replace("@@COLOR@@", color) + "brick.run()";
	utils::TcpRobotCommunicator tcpRobotCommunicator("TrikTcpServer");
	tcpRobotCommunicator.runDirectCommand(directCommand);
}

