#include "rangeSensor.h"

const unsigned rangeSensorResponseSize = 9;

using namespace ev3::robotModel::real::parts;
using namespace kitBase::robotModel;

RangeSensor::RangeSensor(const kitBase::robotModel::DeviceInfo &info
		, const kitBase::robotModel::PortInfo &port
		, utils::robotCommunication::RobotCommunicator &robotCommunicator)
	: robotParts::RangeSensor(info, port)
	, mImplementation(robotCommunicator, port, REFLECTION, READY_RAW)
	, mRobotCommunicator(robotCommunicator)
{
}

void RangeSensor::read()
{
	const int size = 21;
	QByteArray command(size, 0);
	command[0] = size - 2;
	command[1] = size >> 8;
	command[2] = 4;
	command[3] = 0;
	command[4] = DIRECT_COMMAND_REPLY;
	const int globalVariablesCount = 4;
	const int localVariablesCount = 0;
	command[5] = globalVariablesCount & 0xFF;
	command[6] = ((localVariablesCount << 2) | (globalVariablesCount >> 8));
	command[7] = opINPUT_DEVICE;
	command[8] = READY_RAW;
	command[9] = (PRIMPAR_LONG | PRIMPAR_CONST | PRIMPAR_1_BYTE);
	command[10] = 0x00; //layer
	command[11] = (PRIMPAR_LONG | PRIMPAR_CONST | PRIMPAR_1_BYTE);
	command[12] = mImplementation.lowLevelPort(); //port[0-3]
	command[13] = (PRIMPAR_LONG | PRIMPAR_CONST | PRIMPAR_1_BYTE);
	command[14] = 0x00; //type (0 = Don’t change type)
	command[15] = (PRIMPAR_LONG | PRIMPAR_CONST | PRIMPAR_1_BYTE);
	command[16] = 0x00; //mode – Device mode [0-7]
	command[17] = (PRIMPAR_LONG | PRIMPAR_CONST | PRIMPAR_1_BYTE);
	command[18] = 1; //Number of return values
	command[19] = 225;
	command[20] = 0;

	QByteArray outputBuf;
	mRobotCommunicator.send(command, rangeSensorResponseSize, outputBuf);
	emit newData(static_cast<int>(outputBuf.data()[5]));
}

