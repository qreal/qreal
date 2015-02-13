#include "led.h"
#include <qrutils/inFile.h>
#include <QtGui/QColor>

using namespace trikKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

Led::Led(const DeviceInfo &info, const PortInfo &port
		, utils::TcpRobotCommunicator &tcpRobotCommunicator)
	: robotModel::parts::TrikLed(info, port)
	, mRobotCommunicator(tcpRobotCommunicator)
{
}

void Led::setColor(const QString &color)
{
	const QString pathToCommand = ":/trikQts/templates/led.t";
	const QString directCommand = utils::InFile::readAll(pathToCommand)
			.replace("@@COLOR@@", color) + "script.run();";

	mRobotCommunicator.runDirectCommand(directCommand);
}

