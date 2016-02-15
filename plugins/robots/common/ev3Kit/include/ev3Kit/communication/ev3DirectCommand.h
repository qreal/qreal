/* Copyright 2014-2015 CyberTech Labs Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#include <QtCore/QByteArray>
#include <QtCore/QString>

#include "ev3Kit/communication/commandConstants.h"

namespace ev3 {
namespace communication {

class Ev3DirectCommand
{
public:
	Ev3DirectCommand();

	static QByteArray formCommand(int size, int messageCounter, ushort globalSize
			, int localSize, enums::commandType::CommandTypeEnum type);
	static void addOpcode(enums::opcode::OpcodeEnum opcode, QByteArray &command, int &index);
	static void addByteParameter(qint8 parameter, QByteArray &command, int &index);
	static void addShortParameter(qint16 parameter, QByteArray &command, int &index);
	static void addStringParameter(const QString &parameter, QByteArray &command, int &index);
	static void addGlobalIndex(qint8 globalIndex, QByteArray &command, int &index);
};

}
}
