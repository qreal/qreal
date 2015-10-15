/* Copyright 2007-2015 QReal Research Group
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

#include "i2cCommunicator.h"

#include <time.h>

#include "commandConstants.h"

using namespace nxt::communication;

const unsigned lsGetStatusResponseSize = 6;

I2CCommunicator::I2CCommunicator(utils::robotCommunication::RobotCommunicator &robotCommunicator)
	: mRobotCommunicator(robotCommunicator)
{
}

void I2CCommunicator::sendI2C(QObject *addressee
		, const QByteArray &buffer, const int responseSize
		, const kitBase::robotModel::PortInfo &port)
{
	QByteArray command(buffer.length() + 7, 0);
	command[0] = buffer.length() + 5;
	command[1] = 0x00;
	command[2] = enums::telegramType::directCommandNoResponse;
	command[3] = enums::commandCode::LSWRITE;
	command[4] = toNxtInputPort(port);
	command[5] = buffer.length();
	command[6] = responseSize;
	for (int i = 0; i < buffer.length(); ++i) {
		command[i + 7] = buffer[i];
	}

	QByteArray dumpOutput;
	mRobotCommunicator.send(command, 0, dumpOutput);

	if (!waitForI2CBytes(responseSize, port)) {
		/// @todo: Violates incapsuation
		emit mRobotCommunicator.response(addressee, QByteArray());
		return;
	}

	if (responseSize > 0) {
		command.clear();
		command.resize(5);

		command[0] = 0x03;
		command[1] = 0x00;
		command[2] = enums::telegramType::directCommandResponseRequired;
		command[3] = enums::commandCode::LSREAD;
		command[4] = toNxtInputPort(port);

		QByteArray result;
		mRobotCommunicator.send(command, 22, result);

		QByteArray decodedResult = result.right(result.length() - 5);
		/// @todo: Violates incapsuation
		emit mRobotCommunicator.response(addressee, decodedResult);
	} else {
		/// @todo Correctly process empty required response
		QByteArray result(1, 0);
		/// @todo: Violates incapsuation
		emit mRobotCommunicator.response(addressee, result);
	}
}

bool I2CCommunicator::waitForI2CBytes(int bytes, const kitBase::robotModel::PortInfo &port)
{
	time_t const startTime = clock();
	do {
		const int bytesReady = i2cBytesReady(port);
		QThread::msleep(10);
		if (clock() - startTime > i2cTimeout) {
			return false;
		}
		if (bytesReady >= bytes) {
			return true;
		}
	} while (true);
}

int I2CCommunicator::i2cBytesReady(const kitBase::robotModel::PortInfo &port)
{
	QByteArray command(5, 0);
	command[0] = 0x03;
	command[1] = 0x00;

	command[2] = enums::telegramType::directCommandResponseRequired;
	command[3] = enums::commandCode::LSGETSTATUS;
	command[4] = toNxtInputPort(port);

	QByteArray result;
	mRobotCommunicator.send(command, lsGetStatusResponseSize, result);

	// static_cats<int> prevents a warning about operator != ambiguity
	if (result.isEmpty() || static_cast<int>(result[4]) != enums::errorCode::success) {
		return 0;
	} else {
		return result[5];
	}
}

char I2CCommunicator::toNxtInputPort(const kitBase::robotModel::PortInfo &port)
{
	return static_cast<char>(port.name().toInt() - 1);
}
