#include "led.h"
#include <qrutils/inFile.h>
#include <QtGui/QColor>

using namespace trikKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

Led::Led(DeviceInfo const &info, PortInfo const &port
		, utils::TcpRobotCommunicator &tcpRobotCommunicator)
	: robotModel::parts::TrikLed(info, port)
	, mRobotCommunicator(tcpRobotCommunicator)
{
}

void Led::setColor(const QString &color)
{
	QString const pathToCommand = ":/trik/templates/led.t";
	QString const directCommand = utils::InFile::readAll(pathToCommand).replace("@@COLOR@@", color) + "brick.run();";
	mRobotCommunicator.runDirectCommand(directCommand);
}

