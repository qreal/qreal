#pragma once

#include <QtCore/QByteArray>
#include <QtCore/QString>
#include "commandConstants.h"

namespace ev3KitInterpreter {
namespace robotModel {
namespace real {

class Ev3DirectCommand
{
public:
	Ev3DirectCommand();

	static QByteArray formCommand(int size, int messageCounter, ushort globalSize, int localSize, enums::commandType::CommandTypeEnum type);
	static void addOpcode(enums::opcode::OpcodeEnum opcode, QByteArray &command, int &index);
	static void addByteParameter(qint8 parameter, QByteArray &command, int &index);
	static void addShortParameter(qint16 parameter, QByteArray &command, int &index);
	static void addStringParameter(QString const &parameter, QByteArray &command, int &index);
	static void addGlobalIndex(qint8 globalIndex, QByteArray &command, int &index);
};
}
}
}
