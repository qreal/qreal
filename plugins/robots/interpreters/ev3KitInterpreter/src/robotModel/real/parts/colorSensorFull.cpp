#include "colorSensorFull.h"

unsigned const colorSensorResponseSize = 9;

using namespace ev3KitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

ColorSensorFull::ColorSensorFull(const interpreterBase::robotModel::DeviceInfo &info
						 , const interpreterBase::robotModel::PortInfo &port
						 , utils::robotCommunication::RobotCommunicator &robotCommunicator)
	: interpreterBase::robotModel::robotParts::ColorSensorFull(info, port)
	, mColorSensor(port, robotCommunicator, SensorTypeEnum::COLORFULL)
	, mRobotCommunicator(robotCommunicator)
{
}

void ColorSensorFull::read()
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
	command[12] = 0; //port[0-3]
	command[13] = (PRIMPAR_LONG | PRIMPAR_CONST | PRIMPAR_1_BYTE);
	command[14] = 0x00; //type (0 = Don’t change type)
	command[15] = (PRIMPAR_LONG | PRIMPAR_CONST | PRIMPAR_1_BYTE);
	command[16] = 2; //mode – Device mode [0-7]
	command[17] = (PRIMPAR_LONG | PRIMPAR_CONST | PRIMPAR_1_BYTE);
	command[18] = 1; //Number of return values
	command[19] = 225;
	command[20] = 0;

	QByteArray outputBuf;
	mRobotCommunicator.send(command, colorSensorResponseSize, outputBuf);
	emit newData(static_cast<int>(outputBuf.data()[6]));
}

