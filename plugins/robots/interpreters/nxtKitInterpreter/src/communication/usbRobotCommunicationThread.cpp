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
#include <qrkernel/logging.h>
#include <plugins/robots/thirdparty/libusb-1.0.19/include/libusb-1.0/libusb.h>

#include "commandConstants.h"

using namespace nxt::communication;

const int packetHeaderSize = 2;

static const int NXT_VID   = 0x0694;
static const int NXT_PID   = 0x0002;
static const int NXT_USB_TIMEOUT = 2000;
static const int NXT_EP_OUT = 0x01;
static const int NXT_EP_IN = 0x82;
static const int NXT_INTERFACE_NUMBER = 0;
static const int NXT_CONFIGURATION_NB = 1;
static const int DEBUG_LEVEL = 3;

UsbRobotCommunicationThread::UsbRobotCommunicationThread()
	: mHandle(nullptr)
	, mKeepAliveTimer(new QTimer(this))
	, mStopped(false)
{
	QObject::connect(mKeepAliveTimer, SIGNAL(timeout()), this, SLOT(checkForConnection()));
}

UsbRobotCommunicationThread::~UsbRobotCommunicationThread()
{
	if (mHandle) {
		disconnect();
	}
}

void UsbRobotCommunicationThread::connect()
{
	if (mHandle) {
		emit connected(true, QString());
		return;
	}

	libusb_init(nullptr);
	libusb_set_debug(nullptr, DEBUG_LEVEL);
	mHandle = libusb_open_device_with_vid_pid(nullptr, NXT_VID, NXT_PID);
	if (!mHandle) {
		emit connected(false, tr("Cannot find NXT device. Check robot connected and turned on and try again."));
		return;
	}

	if (libusb_kernel_driver_active(mHandle, NXT_INTERFACE_NUMBER)) {
		libusb_detach_kernel_driver(mHandle, NXT_INTERFACE_NUMBER);
	}

	if (libusb_set_configuration(mHandle, NXT_CONFIGURATION_NB) < 0) {
		emit connected(false, tr("USB Device configuration problem. Please contact developers."));
		libusb_close(mHandle);
		mHandle = nullptr;
		return;
	}

	if (libusb_claim_interface(mHandle, NXT_INTERFACE_NUMBER) < 0) {
		emit connected(false, tr("NXT device is already used by another software."));
		libusb_close(mHandle);
		mHandle = nullptr;
		return;
	}

	QByteArray getFirmwareCommand(4, 0);
	getFirmwareCommand[0] = '\0';
	getFirmwareCommand[1] = '\0';
	getFirmwareCommand[2] = enums::telegramType::directCommandResponseRequired/* 0x01*/;
	getFirmwareCommand[3] = 0x88;

	QByteArray handshakeResponse;
	send(getFirmwareCommand, 9, handshakeResponse);
	if (handshakeResponse.isEmpty()) {
		emit connected(false, tr("NXT handshake procedure failed. Please contact developers."));
		libusb_close(mHandle);
		mHandle = nullptr;
		return;
	}

	emit connected(true, QString());
	mKeepAliveTimer->moveToThread(thread());
	mKeepAliveTimer->start(500);
}

void UsbRobotCommunicationThread::send(QObject *addressee, const QByteArray &buffer, int responseSize)
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

void UsbRobotCommunicationThread::send(const QByteArray &buffer, int responseSize, QByteArray &outputBuffer)
{
	if (!mHandle) {
		return;
	}

	QByteArray newBuffer;
	for (int i = packetHeaderSize; i < buffer.length(); ++i) {
		newBuffer[i - packetHeaderSize] = buffer[i];
	}

	uchar *cmd = reinterpret_cast<uchar *>(const_cast<char *>(newBuffer.data()));
	int actualLength = 0;
	int err = libusb_bulk_transfer(mHandle, NXT_EP_OUT, cmd, newBuffer.length(), &actualLength, NXT_USB_TIMEOUT);
	if (err == LIBUSB_ERROR_IO || err == LIBUSB_ERROR_PIPE || err == LIBUSB_ERROR_INTERRUPTED) {
		emit errorOccured(tr("Connection to NXT lost"));
		disconnect();
		return;
	} else if (err < 0) {
		QLOG_TRACE() << "Sending" << buffer << "failed with libusb error" << err;
		return;
	}

	if (!isResponseNeeded(buffer)) {
		return;
	}

	uchar response[responseSize];
	actualLength = 0;
	outputBuffer = QByteArray(responseSize, '\0');
	err = libusb_bulk_transfer(mHandle, NXT_EP_IN, response, responseSize, &actualLength, NXT_USB_TIMEOUT);
	if (err == LIBUSB_ERROR_IO || err == LIBUSB_ERROR_PIPE || err == LIBUSB_ERROR_INTERRUPTED) {
		emit errorOccured(tr("Connection to NXT lost"));
		disconnect();
		return;
	} else if (err < 0) {
		QLOG_TRACE() << "Recieving answer from command" << buffer << "failed with libusb error" << err;
		return;
	}

	outputBuffer[0] = responseSize - 2;
	outputBuffer[1] = 0;
	for (int i = 0; i < responseSize - packetHeaderSize; ++i) {
		outputBuffer[i + packetHeaderSize] = response[i];
	}
}

void UsbRobotCommunicationThread::reconnect()
{
	connect();
}

void UsbRobotCommunicationThread::disconnect()
{
	if (mHandle) {
		libusb_attach_kernel_driver(mHandle, NXT_INTERFACE_NUMBER);
		libusb_close(mHandle);
		libusb_exit(nullptr);
		mHandle = nullptr;
	}

	mKeepAliveTimer->stop();
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
