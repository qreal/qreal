#include "motionSensor.h"
#include <utils/tcpRobotCommunicator.h>
#include <qrutils/inFile.h>

using namespace trikKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

MotionSensor::MotionSensor(DeviceInfo const &info, PortInfo const &port)
	: robotModel::parts::TrikMotionSensor(info, port)
{
}

void MotionSensor::read()
{
	QString const pathToCommand = ":/trik/templates/wait/motion.t";
	QString directCommand = utils::InFile::readAll(pathToCommand) + "brick.run()";
	utils::TcpRobotCommunicator tcpRobotCommunicator("TrikTcpServer");
	tcpRobotCommunicator.runDirectCommand(directCommand);

	/// to do feedback from trikRuntime. Now, the command is only sent there
	emit newData(0);
}
