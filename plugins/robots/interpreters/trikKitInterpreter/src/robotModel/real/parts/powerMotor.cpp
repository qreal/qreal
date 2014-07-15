#include "powerMotor.h"
#include <qrutils/inFile.h>
#include <utils/tcpRobotCommunicator.h>

using namespace trikKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

PowerMotor::PowerMotor(DeviceInfo const &info, PortInfo const &port)
	: robotModel::parts::TrikPowerMotor(info, port)
{
}

void PowerMotor::on(int speed)
{
	QString const pathToCommand = ":/trik/templates/engines/forward.t";
	QString directCommand = utils::InFile::readAll(pathToCommand);
	directCommand = directCommand.replace("@@PORT@@", port().name());
	directCommand = directCommand.replace("@@POWER@@", QString::number(speed));
	utils::TcpRobotCommunicator tcpRobotCommunicator("TrikTcpServer");
	tcpRobotCommunicator.runDirectCommand(directCommand);
}

void PowerMotor::stop()
{
	on(0);
}

void PowerMotor::off()
{
	on(0);
}
