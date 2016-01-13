/* Copyright 2015 CyberTech Labs Ltd.
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

#include "ev3Kit/communication/ev3RobotCommunicationThread.h"

#include <QtCore/QFileInfo>

#include "ev3Kit/communication/commandConstants.h"
#include "ev3Kit/communication/ev3DirectCommand.h"

static const int SYSTEM_COMMAND_REPLY =             0x01;    //  System command, reply required
static const int SYSTEM_COMMAND_NO_REPLY =          0x81;    //  System command, reply not required
static const int BEGIN_DOWNLOAD =                   0x92;    //  Begin file down load
static const int CONTINUE_DOWNLOAD =                0x93;    //  Continue file down load
static const int SYSTEM_REPLY =                     0x03;    //  System command reply
static const int SYSTEM_REPLY_ERROR =               0x05;    //  System command reply error
static const int BEGIN_DOWNLOAD_RESPONSE_SIZE =     8;
static const int CONTINUE_DOWNLOAD_RESPONSE_SIZE =  8;
static const int SUCCESS =                          0x00;
static const int END_OF_FILE =                      0x08;

using namespace ev3::communication;

Ev3RobotCommunicationThread::~Ev3RobotCommunicationThread()
{
}

QString Ev3RobotCommunicationThread::uploadFile(const QString &sourceFile, const QString &targetDir)
{
	const QFileInfo fileInfo(sourceFile);
	// A path to file on the remote device.
	const QString devicePath = targetDir + "/" + fileInfo.fileName();
	QFile file(sourceFile);
	/// @todo: Implement more detailed error reporting
	if (!file.open(QIODevice::ReadOnly)) {
		return QString();
	}

	QByteArray data = file.readAll();
	file.close();
	const int chunkSize = 960;

	const int cmdBeginSize = 11 + devicePath.size();
	QByteArray commandBegin(cmdBeginSize, 0);
	commandBegin[0] = (cmdBeginSize - 2) & 0xFF;
	commandBegin[1] = ((cmdBeginSize - 2) >> 8) & 0xFF ;
	commandBegin[2] = 0x02;
	commandBegin[3] = 0x00;
	commandBegin[4] = SYSTEM_COMMAND_REPLY;
	commandBegin[5] = BEGIN_DOWNLOAD;
	commandBegin[6] = data.size() & 0xFF;
	commandBegin[7] = (data.size() >> 8) & 0xFF;
	commandBegin[8] = (data.size() >> 16) & 0xFF;
	commandBegin[9] = (data.size() >> 24) & 0xFF;
	int index = 10;
	for (int i = 0; i < devicePath.size(); ++i) {
		commandBegin[index++] = devicePath.at(i).toLatin1();
	}

	commandBegin[index] = 0x00;

	send(commandBegin);
	QByteArray commandBeginResponse = receive(BEGIN_DOWNLOAD_RESPONSE_SIZE);
	if (commandBeginResponse.at(4) != SYSTEM_REPLY) {
		return QString();
	}

	char handle = commandBeginResponse.at(7);
	int sizeSent = 0;
	while (sizeSent < data.size()) {
		const int sizeToSend = qMin(chunkSize, data.size() - sizeSent);
		const int cmdContinueSize = 7 + sizeToSend;
		QByteArray commandContinue(cmdContinueSize, 0);
		commandContinue[0] = (cmdContinueSize - 2) & 0xFF;
		commandContinue[1] = ((cmdContinueSize - 2) >> 8) & 0xFF ;
		commandContinue[2] = 0x03;
		commandContinue[3] = 0x00;
		commandContinue[4] = SYSTEM_COMMAND_REPLY;
		commandContinue[5] = CONTINUE_DOWNLOAD;
		commandContinue[6] = handle;
		for (int i = 0; i < sizeToSend; ++i) {
			commandContinue[7 + i] = data.at(sizeSent++);
		}

		send(commandContinue);
		QByteArray commandContinueResponse = receive(CONTINUE_DOWNLOAD_RESPONSE_SIZE);
		if (commandContinueResponse.at(7) != SUCCESS &&
				(commandContinueResponse.at(7) != END_OF_FILE && sizeSent == data.size())) {
			return QString();
		}
	}

	return devicePath;
}

bool Ev3RobotCommunicationThread::runProgram(const QString &pathOnRobot)
{
	QByteArray command = Ev3DirectCommand::formCommand(21 + pathOnRobot.size(), 0, 0x08, 0
			, enums::commandType::CommandTypeEnum::DIRECT_COMMAND_NO_REPLY);
	int index = 7;
	command[index++] = 0xC0;  // opFILE            Opcode file related
	command[index++] = 0x08;
	command[index++] = 0x82;  // LC0(LOAD_IMAGE)   Command encoded as single byte constant
	command[index++] = 0x01;  // LC2(USER_SLOT)    User slot (1 = program slot) encoded as single constant byte
	command[index++] = 0x00;
	command[index++] = 0x84;  // LCS               Encoding: String to follow (zero terminated)

	for (int i = 0; i < pathOnRobot.size(); ++i) {
		command[index++] = pathOnRobot.at(i).toLatin1();
	}

	command[index++] = 0x00;  // Zero termination of string above
	command[index++] = 0x60;  // GV0(0), Return Image Size at Global Var offset 0. Offset encoded as single byte
	command[index++] = 0x64;  // GV0(4), Return Address of image at Global Var offset 4. Offset encoded as single byte
	command[index++] = 0x03;  // opPROGRAM_START Opcode
	command[index++] = 0x01;
	command[index++] = 0x60;  // GV0(0), Size of image at Global Var offset 0.
	command[index++] = 0x64;  // GV0(4), Address of image at Global Var offset 4
	command[index++] = 0x00;  // LC0(0), Debug mode (0 = normal) encoded as single byte constant
	send(command);

	return true;
}

void Ev3RobotCommunicationThread::stopProgram()
{
	QByteArray command = Ev3DirectCommand::formCommand(9, 0, 0, 0
			, enums::commandType::CommandTypeEnum::DIRECT_COMMAND_NO_REPLY);
	command[7] = 0x02;  // opPROGRAM_STOP Opcode
	command[8] = 0x01;  // LC0(USER_SLOT), User slot = 1 (program slot)
	send(command);
}
