#include "ev3InputDevice.h"

#include <utils/tracer.h>

using namespace ev3KitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;
using namespace utils;

Ev3InputDevice::Ev3InputDevice(utils::robotCommunication::RobotCommunicator &robotCommunicator
		, interpreterBase::robotModel::PortInfo const &port)
	: mRobotCommunicator(robotCommunicator)
	, mLowLevelPort(port.name().at(0).toLatin1() - '1')
{
}

void Ev3InputDevice::send(QByteArray command, unsigned const responseSize, QByteArray reading)
{
	mRobotCommunicator.send(command, responseSize, reading);
}

char Ev3InputDevice::lowLevelPort() const
{
	return mLowLevelPort;
}

QByteArray Ev3InputDevice::readySiCommand(int port, int sensorMode)
{
	QByteArray command = Ev3DirectCommand::formCommand(21, 2, 4, 0, enums::commandType::CommandTypeEnum::DIRECT_COMMAND_REPLY);
	int index = 7;
	Ev3DirectCommand::addOpcode(enums::opcode::OpcodeEnum::INPUT_DEVICE_READY_SI, command, index);
	Ev3DirectCommand::addByteParameter(enums::daisyChainLayer::DaisyChainLayerEnum::EV3, command, index);
	Ev3DirectCommand::addByteParameter(port, command, index);
	Ev3DirectCommand::addByteParameter(0x00, command, index); //type (0 = Don’t change type)
	Ev3DirectCommand::addByteParameter(sensorMode, command, index); //mode – Device mode [0-7]
	Ev3DirectCommand::addByteParameter(0x01, command, index); // # values
	Ev3DirectCommand::addGlobalIndex(0x00, command, index); // index for return data
	return command;
}

QByteArray Ev3InputDevice::readyRawCommand(int port, int sensorMode)
{
	QByteArray command = Ev3DirectCommand::formCommand(21, 2, 4, 0, enums::commandType::CommandTypeEnum::DIRECT_COMMAND_REPLY);
	int index = 7;
	Ev3DirectCommand::addOpcode(enums::opcode::OpcodeEnum::INPUT_DEVICE_READY_RAW, command, index);
	Ev3DirectCommand::addByteParameter(enums::daisyChainLayer::DaisyChainLayerEnum::EV3, command, index);
	Ev3DirectCommand::addByteParameter(port, command, index);
	Ev3DirectCommand::addByteParameter(0x00, command, index); //type (0 = Don’t change type)
	Ev3DirectCommand::addByteParameter(sensorMode, command, index); //mode – Device mode [0-7]
	Ev3DirectCommand::addByteParameter(0x01, command, index); // # values
	Ev3DirectCommand::addGlobalIndex(0x00, command, index); // index for return data
	return command;
}

QByteArray Ev3InputDevice::readyPercentCommand(int port, int sensorMode)
{
	QByteArray command = Ev3DirectCommand::formCommand(21, 2, 1, 0, enums::commandType::CommandTypeEnum::DIRECT_COMMAND_REPLY);
	int index = 7;
	Ev3DirectCommand::addOpcode(enums::opcode::OpcodeEnum::INPUT_DEVICE_READY_RAW, command, index);
	Ev3DirectCommand::addByteParameter(enums::daisyChainLayer::DaisyChainLayerEnum::EV3, command, index);
	Ev3DirectCommand::addByteParameter(port, command, index);
	Ev3DirectCommand::addByteParameter(0x00, command, index); //type (0 = Don’t change type)
	Ev3DirectCommand::addByteParameter(sensorMode, command, index); //mode – Device mode [0-7]
	Ev3DirectCommand::addByteParameter(0x01, command, index); // # values
	Ev3DirectCommand::addGlobalIndex(0x00, command, index); // index for return data
	return command;
}
