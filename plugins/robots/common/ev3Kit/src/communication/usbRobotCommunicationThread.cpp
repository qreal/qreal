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

#include "ev3Kit/communication/usbRobotCommunicationThread.h"

#include <QtCore/QTimer>
#include <QtCore/QThread>

#include <libusb.h>

#include "ev3Kit/communication/ev3DirectCommand.h"
#include "ev3Kit/communication/commandConstants.h"

static const int EV3_VID = 0x0694;
static const int EV3_PID = 0x0005;
static const int EV3_USB_TIMEOUT = 2000;
static const int EV3_INTERFACE_NUMBER = 0;
static const int EV3_CONFIGURATION_NB = 1;
static const int EV3_EP_OUT = 0x01;
static const int EV3_EP_IN = 0x81;
static const int EV3_PACKET_SIZE = 0x400;
static const int MAX_DEBUG_LEVEL = 3;

using namespace ev3::communication;

UsbRobotCommunicationThread::UsbRobotCommunicationThread()
	: mHandle(nullptr)
	, mKeepAliveTimer(new QTimer(this))
{
	QObject::connect(mKeepAliveTimer, SIGNAL(timeout()), this, SLOT(checkForConnection()));
}

UsbRobotCommunicationThread::~UsbRobotCommunicationThread()
{
	disconnect();
}

void UsbRobotCommunicationThread::send(QObject *addressee, const QByteArray &buffer, unsigned responseSize)
{
	if (!mHandle) {
		emit response(addressee, QByteArray());
		return;
	}

	send(buffer);
	if (buffer.size() >= 5 && buffer[4] == enums::commandType::CommandTypeEnum::DIRECT_COMMAND_REPLY) {
		const QByteArray result = receive(responseSize);
		emit response(addressee, result);
	} else {
		emit response(addressee, QByteArray());
	}
}

void UsbRobotCommunicationThread::connect()
{
	if (mHandle) {
		emit connected(true, QString());
		return;
	}

	libusb_init(nullptr);
	// Uncomment it to debug usb communication: libusb_set_debug(nullptr, MAX_DEBUG_LEVEL);
	mHandle = libusb_open_device_with_vid_pid(nullptr, EV3_VID, EV3_PID);
	if (!mHandle) {
		emit connected(false, tr("Cannot find EV3 device. Check robot connected and turned on and try again."));
		return;
	}

	if (libusb_kernel_driver_active(mHandle,EV3_INTERFACE_NUMBER)) {
		libusb_detach_kernel_driver(mHandle, EV3_INTERFACE_NUMBER);
	}

	if (libusb_set_configuration(mHandle, EV3_CONFIGURATION_NB) < 0) {
		emit connected(false, tr("USB Device configuration problem. Please contact developers."));
		return;
	}

	if (libusb_claim_interface(mHandle, EV3_INTERFACE_NUMBER) < 0) {
		emit connected(false, tr("USB Device interface problem. Please contact developers."));
		return;
	}

	emit connected(true, QString());
	mKeepAliveTimer->moveToThread(this->thread());
	mKeepAliveTimer->disconnect();
	QObject::connect(mKeepAliveTimer, SIGNAL(timeout()), this, SLOT(checkForConnection()));
	mKeepAliveTimer->start(500);
}

void UsbRobotCommunicationThread::reconnect()
{
	connect();
}

void UsbRobotCommunicationThread::disconnect()
{
	if (mHandle) {
		libusb_attach_kernel_driver(mHandle, EV3_INTERFACE_NUMBER);
		libusb_close(mHandle);
		libusb_exit(nullptr);
		mHandle = nullptr;
	}

	mKeepAliveTimer->stop();
	emit disconnected();
}

void UsbRobotCommunicationThread::allowLongJobs(bool allow)
{
	Q_UNUSED(allow);
}

void UsbRobotCommunicationThread::checkForConnection()
{
	if (!mHandle) {
		return;
	}

	// Sending "Keep alive" command to check connection.
	unsigned char command[10];
	command[0] = 8;
	command[1] = 0;
	command[2] = 0;
	command[3] = 0;
	command[4] = enums::commandType::CommandTypeEnum::DIRECT_COMMAND_NO_REPLY;
	command[5] = 0;
	command[6] = 0;
	command[7] = enums::opcode::OpcodeEnum::KEEP_ALIVE;
	command[8] = enums::argumentSize::ArgumentSizeEnum::BYTE;
	command[9] = 10; //Number of minutes before entering sleep mode.

	int actualLength = 0;
	int success = libusb_bulk_transfer(mHandle, EV3_EP_OUT, command, EV3_PACKET_SIZE, &actualLength, EV3_USB_TIMEOUT);

	if (success != 0) {
		emit disconnected();
		mKeepAliveTimer->stop();
	}
}

void UsbRobotCommunicationThread::send(const QByteArray &buffer, unsigned responseSize, QByteArray &outputBuffer)
{
	send(buffer);
	outputBuffer = receive(responseSize);
}

void UsbRobotCommunicationThread::send(const QByteArray &buffer) const
{
	uchar *cmd = reinterpret_cast<uchar *>(const_cast<char *>(buffer.data()));
	int actualLength = 0;
	libusb_bulk_transfer(mHandle, EV3_EP_OUT, cmd, EV3_PACKET_SIZE, &actualLength, EV3_USB_TIMEOUT);
}

QByteArray UsbRobotCommunicationThread::receive(int size) const
{
	// It's strange, but regardless of kind of response, it must have size = 1024 (EV3_PACKET_SIZE)
	// If we set another size(For example, response[10]), program will throw error.
	unsigned char response[EV3_PACKET_SIZE];
	int actualLength = 0;
	libusb_bulk_transfer(mHandle, EV3_EP_IN, response, EV3_PACKET_SIZE, &actualLength, EV3_USB_TIMEOUT);
	return QByteArray::fromRawData(reinterpret_cast<char *>(response), size);
}

void UsbRobotCommunicationThread::checkConsistency()
{
}
