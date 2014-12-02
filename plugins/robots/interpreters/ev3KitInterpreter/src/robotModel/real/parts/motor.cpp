#include "motor.h"

using namespace ev3KitInterpreter::robotModel::real::parts;
using namespace interpreterBase;
using namespace robotModel;
using namespace utils;
using namespace robotCommunication;

Motor::Motor(DeviceInfo const &info, PortInfo const &port, RobotCommunicator &robotCommunicator)
	: Ev3Motor(info, port)
	, mRobotCommunicator(robotCommunicator)
{
}

void Motor::on(int speed)
{
	QByteArray command = Ev3DirectCommand::formCommand(19, 0, 0, 0, enums::commandType::CommandTypeEnum::DIRECT_COMMAND_NO_REPLY);
	int index = 7;
	Ev3DirectCommand::addOpcode(enums::opcode::OpcodeEnum::OUTPUT_POWER, command, index);
	Ev3DirectCommand::addByteParameter(enums::daisyChainLayer::DaisyChainLayerEnum::EV3, command, index);
	Ev3DirectCommand::addByteParameter(parsePort(port().name().at(0).toLatin1()), command, index);
	Ev3DirectCommand::addByteParameter(speed, command, index);
	Ev3DirectCommand::addOpcode(enums::opcode::OpcodeEnum::OUTPUT_START, command, index);
	Ev3DirectCommand::addByteParameter(enums::daisyChainLayer::DaisyChainLayerEnum::EV3, command, index);
	Ev3DirectCommand::addByteParameter(parsePort(port().name().at(0).toLatin1()), command, index);
	mRobotCommunicator.send(this, command, 3);
}

void Motor::stop()
{
	on(0);
}

void Motor::off()
{
	QByteArray command = Ev3DirectCommand::formCommand(14, 0, 0, 0, enums::commandType::CommandTypeEnum::DIRECT_COMMAND_NO_REPLY);
	int index = 7;
	Ev3DirectCommand::addOpcode(enums::opcode::OpcodeEnum::OUTPUT_STOP, command, index);
	Ev3DirectCommand::addByteParameter(enums::daisyChainLayer::DaisyChainLayerEnum::EV3, command, index);
	Ev3DirectCommand::addByteParameter(parsePort(port().name().at(0).toLatin1()), command, index);
	Ev3DirectCommand::addByteParameter(0x00, command, index); // brake (0 = coast, 1 = brake)
	mRobotCommunicator.send(this, command, 3);
}

char Motor::parsePort(QChar portName)
{
	if (portName == 'A') {
		return enums::outputPort::OutputPortEnum::A;
	} else if (portName == 'B') {
		return enums::outputPort::OutputPortEnum::B;
	} else if (portName == 'C') {
		return enums::outputPort::OutputPortEnum::C;
	} else if (portName == 'D') {
		return enums::outputPort::OutputPortEnum::D;
	}
	return 0x00;
}

