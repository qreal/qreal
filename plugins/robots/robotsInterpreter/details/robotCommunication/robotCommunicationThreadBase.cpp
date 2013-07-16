#include <time.h>

#include "robotCommunicationThreadBase.h"
#include "../tracer.h"

using namespace qReal::interpreters;
using namespace qReal::interpreters::robots::details;

unsigned const lsGetStatusResponseSize = 6;

RobotCommunicationThreadBase::RobotCommunicationThreadBase()
{
}

void RobotCommunicationThreadBase::sendI2C(QObject *addressee
		, QByteArray const &buffer, unsigned const responseSize
		, robots::enums::inputPort::InputPortEnum const port)
{
	Tracer::debug(tracer::enums::robotCommunication, "RobotCommunicationThreadBase::sendI2C", "Sending:");

	QByteArray command(buffer.length() + 7, 0);
	command[0] = buffer.length() + 5;
	command[1] = 0x00;
	command[2] = enums::telegramType::directCommandNoResponse;
	command[3] = enums::commandCode::LSWRITE;
	command[4] = port;
	command[5] = buffer.length();
	command[6] = responseSize;
	for (int i = 0; i < buffer.length(); ++i) {
		command[i + 7] = buffer[i];
	}

	QByteArray dumpOutput;
	send(command, 0, dumpOutput);

	if (!waitForI2CBytes(responseSize, port)) {
		Tracer::debug(tracer::enums::robotCommunication, "RobotCommunicationThreadBase::sendI2C", "No response, connection error");
		emit response(addressee, QByteArray());
		return;
	}

	if (responseSize > 0) {
		command.clear();
		command.resize(5);

		command[0] = 0x03;
		command[1] = 0x00;
		command[2] = enums::telegramType::directCommandResponseRequired;
		command[3] = enums::commandCode::LSREAD;
		command[4] = port;

		QByteArray result;
		send(command, 22, result);

		QByteArray decodedResult = result.right(result.length() - 5);
		emit response(addressee, decodedResult);
	} else {
		// TODO: Correctly process empty required response
		QByteArray result(1, 0);
		emit response(addressee, result);
	}
}

bool RobotCommunicationThreadBase::waitForI2CBytes(int bytes, robots::enums::inputPort::InputPortEnum port)
{
	time_t const startTime = clock();
	do {
		int const bytesReady = i2cBytesReady(port);
		SleeperThread::msleep(10);
		if (clock() - startTime > i2cTimeout) {
			return false;
		}
		if (bytesReady >= bytes) {
			return true;
		}
	} while (true);
}

int RobotCommunicationThreadBase::i2cBytesReady(robots::enums::inputPort::InputPortEnum port)
{
	QByteArray command(5, 0);
	command[0] = 0x03;
	command[1] = 0x00;

	command[2] = enums::telegramType::directCommandResponseRequired;
	command[3] = enums::commandCode::LSGETSTATUS;
	command[4] = port;

	QByteArray result;
	send(command, lsGetStatusResponseSize, result);

	// static_cats<int> prevents a warning about operator != ambiguity
	if (result.isEmpty() || static_cast<int>(result[4]) != enums::errorCode::success) {
		return 0;
	} else {
		return result[5];
	}
}
