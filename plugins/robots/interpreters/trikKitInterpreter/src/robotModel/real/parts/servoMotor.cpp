#include "servoMotor.h"
#include <qrutils/inFile.h>
#include <utils/tcpRobotCommunicator.h>

using namespace trikKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

ServoMotor::ServoMotor(DeviceInfo const &info, PortInfo const &port)
	: robotModel::parts::TrikServoMotor(info, port)
{
}

void ServoMotor::on(int speed)
{
	QString const pathToCommand = ":/trik/templates/engines/forward.t";
	QString directCommand = utils::InFile::readAll(pathToCommand) + "brick.run()";
	directCommand = directCommand.replace("@@PORT@@", port().name());
	directCommand = directCommand.replace("@@POWER@@", QString::number(speed));
	utils::TcpRobotCommunicator tcpRobotCommunicator("TrikTcpServer");
	tcpRobotCommunicator.runDirectCommand(directCommand);
}

void ServoMotor::stop()
{
	on(0);
}

void ServoMotor::off()
{
	on(0);
}

