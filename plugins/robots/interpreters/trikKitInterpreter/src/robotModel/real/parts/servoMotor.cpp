#include "servoMotor.h"

#include <qrutils/inFile.h>

using namespace trikKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

ServoMotor::ServoMotor(DeviceInfo const &info, PortInfo const &port
		, utils::TcpRobotCommunicator &tcpRobotCommunicator)
	: robotModel::parts::TrikServoMotor(info, port)
	, mRobotCommunicator(tcpRobotCommunicator)
{
}

void ServoMotor::on(int speed)
{
	QString const pathToCommand = ":/trikQts/templates/engines/forward.t";
	QString const directCommand = utils::InFile::readAll(pathToCommand)
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
