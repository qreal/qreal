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

#include "usbRobotCommunicationThread.h"

#include <time.h>

#include <qrkernel/settingsManager.h>
#include <plugins/robots/thirdparty/qextserialport/src/qextserialenumerator.h>
#include <plugins/robots/thirdparty/qextserialport/src/qextserialport.h>

#include "commandConstants.h"
#include "fantom.h"

using namespace nxt::communication;

const unsigned packetHeaderSize = 3;

UsbRobotCommunicationThread::UsbRobotCommunicationThread()
	: mActive(false)
	, mNXTHandle(0)
	, mFantom(Fantom::correctFantom())
	, mKeepAliveTimer(new QTimer(this))
	, mStopped(false)
{
	QObject::connect(mKeepAliveTimer, SIGNAL(timeout()), this, SLOT(checkForConnection()));
	QObject::connect(mFantom, &Fantom::errorOccured, this, &UsbRobotCommunicationThread::errorOccured);
}

UsbRobotCommunicationThread::~UsbRobotCommunicationThread()
{
	/// @todo: Some long time ago deinitialization code was here (see git history).
	///        But one day it stopped working, hanging out the system on close in background so it was removed.
	///        It would be nice to fix it.
}

bool UsbRobotCommunicationThread::isOpen()
{
	return mActive && mFantom->isAvailable();
	return false;
}

void UsbRobotCommunicationThread::connect()
{
	if (!(mFantom->isAvailable())) {
		return;
	}

	char resNamePC[10000];
	unsigned long nxtIterator;

	if (!isOpen()) {
		mActive = false;
		int status = 0;
		nxtIterator = mFantom->nFANTOM100_createNXTIterator(false, 30, status);
		while (status == kStatusNoError) {
			int status2 = 0;
			mFantom->nFANTOM100_iNXTIterator_getName(nxtIterator, resNamePC, status2);
			QString resName = QString(resNamePC);
			if (resName.toUpper().contains("USB")) {
				break;
			}
			if (mStopped) {
				return;
			}
		}
		if (status == kStatusNoError) {
			mNXTHandle = mFantom->nFANTOM100_iNXTIterator_getNXT(nxtIterator, status);
			mActive = status == kStatusNoError;
		}
		mFantom->nFANTOM100_destroyNXTIterator(nxtIterator, status);
	}
	emit connected(mActive, QString());

	if (mActive) {
		mKeepAliveTimer->start(500);
	}
}

void UsbRobotCommunicationThread::send(QObject *addressee
		, const QByteArray &buffer, const unsigned responseSize)
{
	QByteArray outputBuffer;
	outputBuffer.resize(responseSize);
	send(buffer, responseSize, outputBuffer);
	if (!isResponseNeeded(buffer)) {
		emit response(addressee, QByteArray());
	} else {
		emit response(addressee, outputBuffer);
	}
}

void UsbRobotCommunicationThread::send(const QByteArray &buffer
		, const unsigned responseSize, QByteArray &outputBuffer)
{
	int status = 0;
	QByteArray newBuffer;
	for (int i = packetHeaderSize; i < buffer.length(); i++) {
		newBuffer[i - packetHeaderSize] = buffer[i];
	}

	if (!isResponseNeeded(buffer)) {
		mFantom->nFANTOM100_iNXT_sendDirectCommand(mNXTHandle, false, newBuffer
				, newBuffer.length(), nullptr, 0, status);
	} else {
		const unsigned temporaryOutputBufferSize = 200;
		char *outputBufferPtr2 = new char[temporaryOutputBufferSize];
		for (unsigned i = 0; i < temporaryOutputBufferSize; i++) {
			outputBufferPtr2[i] = 0;
		}

		/// @todo Sending RESETINPUTSCALEDVALUE for every sensor before reading their values.
		// It is a dirty hack and will surely break some sensor functionality related to
		// accumulated some values, but without it after nearly every command
		// sensors start to return some random sh~ instead of correct value.
		// Note that even RESETINPUTSCALEDVALUE will return 0xC0 (data contains out-of-range values)
		// as status byte. Something very bad is going on in our interface to Fantom driver
		// or in driver itself. Further investigation required.
		for (int port = 0; port < 4; ++port) {
			QByteArray command(2, 0);
			command[3] = enums::commandCode::RESETINPUTSCALEDVALUE;
			command[4] = port;

			mFantom->nFANTOM100_iNXT_sendDirectCommand(mNXTHandle, true, command, 2, outputBufferPtr2, 2, status);
		}

		mFantom->nFANTOM100_iNXT_sendDirectCommand(mNXTHandle, true, newBuffer
				, newBuffer.length(), outputBufferPtr2, responseSize - 3, status);

		outputBuffer[0] = responseSize - 2;
		outputBuffer[1] = 0;
		outputBuffer[2] = 2;
		for (unsigned  i = 0; i < responseSize - packetHeaderSize; i++) {
			outputBuffer[i + packetHeaderSize] = outputBufferPtr2[i];
		}
		delete outputBufferPtr2;
	}
}

void UsbRobotCommunicationThread::reconnect()
{
	connect();
}

void UsbRobotCommunicationThread::disconnect()
{
	if (!(mFantom->isAvailable())) {
		return;
	}

	mKeepAliveTimer->stop();
	int status = 0;
	mFantom->nFANTOM100_destroyNXT(mNXTHandle, status);
	mNXTHandle = 0;
	mActive = false;
	emit disconnected();
}

void UsbRobotCommunicationThread::allowLongJobs(bool allow)
{
	mStopped = !allow;
}

void UsbRobotCommunicationThread::checkForConnection()
{
	QByteArray command(4, 0);

	command[3] = enums::commandCode::KEEPALIVE;

	const int keepAliveResponseSize = 9;

	QByteArray response;
	response.resize(keepAliveResponseSize);

	send(command, keepAliveResponseSize, response);

	if (response[3] == '\0') {
		emit disconnected();
	}
}

bool UsbRobotCommunicationThread::isResponseNeeded(const QByteArray &buffer)
{
	return buffer.size() >= 3 && buffer[2] == 0;
}

void UsbRobotCommunicationThread::checkConsistency()
{
	mFantom->checkConsistency();
}
