#include "colorSensorFull.h"

unsigned const colorSensorResponseSize = 9;

using namespace ev3KitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

ColorSensorFull::ColorSensorFull(interpreterBase::robotModel::DeviceInfo const &info
		, interpreterBase::robotModel::PortInfo const &port
		, utils::robotCommunication::RobotCommunicator &robotCommunicator)
	: interpreterBase::robotModel::robotParts::ColorSensorFull(info, port)
	, mColorSensor(port, robotCommunicator, enums::sensorType::SensorTypeEnum::COLOR_FULL)
	, mRobotCommunicator(robotCommunicator)
{
}

void ColorSensorFull::read()
{
	//Something weird with ColorSensorFull and ColorSensorImpl
	QByteArray command = Ev3DirectCommand::formCommand(21, 2, 4, 0, enums::commandType::CommandTypeEnum::DIRECT_COMMAND_REPLY);
	int index = 7;
	Ev3DirectCommand::addOpcode(enums::opcode::OpcodeEnum::INPUT_DEVICE_READY_RAW, command, index);
	Ev3DirectCommand::addByteParameter(enums::daisyChainLayer::DaisyChainLayerEnum::EV3, command, index);
	Ev3DirectCommand::addByteParameter(0, command, index); //port
	Ev3DirectCommand::addByteParameter(0x00, command, index); //type (0 = Don’t change type)
	Ev3DirectCommand::addByteParameter(2, command, index); //mode – Device mode [0-7]
	Ev3DirectCommand::addByteParameter(0x01, command, index); // # values
	Ev3DirectCommand::addGlobalIndex(0x00, command, index); // index for return data

	QByteArray outputBuf;
	mRobotCommunicator.send(command, colorSensorResponseSize, outputBuf);
	emit newData(static_cast<int>(outputBuf.data()[6]));
}

