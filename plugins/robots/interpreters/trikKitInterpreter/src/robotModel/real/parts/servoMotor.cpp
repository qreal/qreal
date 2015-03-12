#include "servoMotor.h"

#include <qrutils/inFile.h>

using namespace trik::robotModel::real::parts;
using namespace kitBase::robotModel;

ServoMotor::ServoMotor(const DeviceInfo &info, const PortInfo &port
		, utils::TcpRobotCommunicator &tcpRobotCommunicator)
	: robotModel::parts::TrikServoMotor(info, port)
	, mRobotCommunicator(tcpRobotCommunicator)
{
}

void ServoMotor::on(int speed)
{
	const QString pathToCommand = ":/trikQts/templates/engines/forward.t";
	const QString directCommand = utils::InFile::readAll(pathToCommand)
			.replace("@@PORT@@", "\"" + port().name() + "\"")
			.replace("@@POWER@@", QString::number(speed)) + "script.run();";

	mRobotCommunicator.runDirectCommand(directCommand);
}

void ServoMotor::stop()
{
	on(0);
}

void ServoMotor::off()
{
	/// @todo It shall be a separate command, to power off motor, not to leave it in blocked state.
	on(0);
}
