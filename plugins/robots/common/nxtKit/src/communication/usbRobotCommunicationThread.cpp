/* Copyright 2012-2016 QReal Research Group
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

#include "nxtKit/communication/usbRobotCommunicationThread.h"

#include <QtCore/QCoreApplication>
#include <time.h>

#include <qrkernel/logging.h>
#include <plugins/robots/thirdparty/libusb-1.0.19/include/libusb-1.0/libusb.h>

#include "nxtKit/communication/nxtCommandConstants.h"
#include "nxtKit/communication/nxtUsbDriverInstaller.h"

using namespace nxt::communication;

const int packetHeaderSize = 2;

static const int NXT_VID   = 0x0694;
static const int NXT_PID   = 0x0002;
static const int ATMEL_VID  = 0x03EB;
static const int SAMBA_PID = 0x6124;
static const int NXT_USB_TIMEOUT = 2000;
static const int NXT_EP_OUT = 0x01;
static const int NXT_EP_IN = 0x82;
static const int NXT_INTERFACE_NUMBER = 0;
static const int DEBUG_LEVEL = 4;

UsbRobotCommunicationThread::UsbRobotCommunicationThread()
	: mHandle(nullptr)
	, mFirmwareMode(false)
	, mKeepAliveTimer(new QTimer(this))
	, mDriverInstaller(new NxtUsbDriverInstaller)
	, mStopped(false)
{
	QObject::connect(mKeepAliveTimer, &QTimer::timeout, this, &UsbRobotCommunicationThread::checkForConnection);
	mDriverInstaller->moveToThread(qApp->thread());
	QObject::connect(this, &UsbRobotCommunicationThread::noDriversFound, mDriverInstaller.data()
			, &NxtUsbDriverInstaller::installUsbDriver, Qt::QueuedConnection);
	QObject::connect(mDriverInstaller.data(), &NxtUsbDriverInstaller::errorOccured
			, this, &UsbRobotCommunicationThread::errorOccured, Qt::QueuedConnection);
	QObject::connect(mDriverInstaller.data(), &NxtUsbDriverInstaller::messageArrived
			, this, &UsbRobotCommunicationThread::messageArrived, Qt::QueuedConnection);
}

UsbRobotCommunicationThread::~UsbRobotCommunicationThread()
{
	if (mHandle) {
		disconnect();
	}
}

bool UsbRobotCommunicationThread::connectImpl(bool firmwareMode, int vid, int pid, const QString &notConnectedErrorText)
{
	if (mHandle) {
		emit connected(true, QString());
		return true;
	}

	libusb_init(nullptr);
	libusb_set_debug(nullptr, DEBUG_LEVEL);

	libusb_device **devices;
	int count = libusb_get_device_list(nullptr, &devices);
	for (int i = 0; i < count; ++i) {
		libusb_device_descriptor device_descriptor;
		if (libusb_get_device_descriptor(devices[i], &device_descriptor) < 0) {
			continue;
		}

		if (device_descriptor.idVendor == vid && device_descriptor.idProduct == pid) {
			const int err = libusb_open(devices[i], &mHandle);
			if (err == LIBUSB_ERROR_NOT_SUPPORTED) {
				emit noDriversFound();
				libusb_free_device_list(devices, 1);
				if (mHandle) {
					libusb_close(mHandle);
				}

				return false;
			} else if (err < 0 || !mHandle) {
				qDebug() << err << mHandle;
				QLOG_ERROR() << "libusb_open returned" << err;
				emit connected(false, notConnectedErrorText);
				libusb_free_device_list(devices, 1);
				return false;
			} else {
				break;
			}
		}
	}

	if (!mHandle) {
		libusb_free_device_list(devices, 1);
		emit connected(false, notConnectedErrorText);
		return false;
	}

	if (libusb_kernel_driver_active(mHandle, NXT_INTERFACE_NUMBER)) {
		qDebug() << "detaching kernel driver";
		libusb_detach_kernel_driver(mHandle, NXT_INTERFACE_NUMBER);
	}

	const QList<int> configurations = {0, 1, 2};
	bool configurationFound = false;
	for (int configiration : configurations) {
		const int err = libusb_set_configuration(mHandle, configiration);
		if (err < 0 && err != LIBUSB_ERROR_NOT_FOUND && err != LIBUSB_ERROR_INVALID_PARAM) {
			QLOG_ERROR() << "libusb_set_configuration for NXT returned" << err << "for configuration" << configiration;
			break;
		} else if (err >= 0) {
			configurationFound = true;
			break;
		}
	}

	if (!configurationFound) {
		QLOG_ERROR() << "No appropriate configuration found among" << configurations << ". Giving up.";
		emit connected(false, tr("USB Device configuration problem. Try to restart TRIK Studio and re-plug NXT."));
		libusb_close(mHandle);
		mHandle = nullptr;
		libusb_free_device_list(devices, 1);
		return false;
	}

	const QList<int> interfaces = {0, 1, 2};
	bool interfaceFound = false;
	for (int interface : interfaces) {
		const int err = libusb_claim_interface(mHandle, interface);
		if (err < 0 && err != LIBUSB_ERROR_NOT_FOUND && err != LIBUSB_ERROR_INVALID_PARAM) {
			QLOG_ERROR() << "libusb_claim_interface for NXT returned" << err << "for interface" << interface;
			break;
		} else if (err >= 0) {
			interfaceFound = true;
			break;
		}
	}

	if (!interfaceFound) {
		QLOG_ERROR() << "No appropriate interface found among" << interfaces << ". Giving up.";
		emit connected(false, tr("NXT device is already used by another software."));
		libusb_close(mHandle);
		mHandle = nullptr;
		libusb_free_device_list(devices, 1);
		return false;
	}

	QByteArray getFirmwareCommand(4, 0);
	getFirmwareCommand[0] = '\0';
	getFirmwareCommand[1] = '\0';
	getFirmwareCommand[2] = enums::telegramType::directCommandResponseRequired;
	getFirmwareCommand[3] = 0x88;

	QByteArray handshakeResponse;
	send(getFirmwareCommand, 9, handshakeResponse);
	if (handshakeResponse.isEmpty()) {
		emit connected(false, tr("NXT handshake procedure failed. Please contact developers."));
		libusb_close(mHandle);
		mHandle = nullptr;
		libusb_free_device_list(devices, 1);
		return false;
	}

	mFirmwareMode = firmwareMode;
	emit connected(true, QString());

	if (!firmwareMode) {
		mKeepAliveTimer->moveToThread(thread());
		mKeepAliveTimer->start(500);
	}

	return true;
}

bool UsbRobotCommunicationThread::connect()
{
	const QString error = tr("Cannot find NXT device. Check robot connected and turned on and try again.");
	return connectImpl(false, NXT_VID, NXT_PID, error);
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

bool UsbRobotCommunicationThread::connectFirmware()
{
	qDebug() << "connecting to firmware...";
	if (mHandle) {
		if (mFirmwareMode) {
			emit errorOccured(tr("Already connected to NXT that is not in resetted mode."));
			return false;
		}

		emit connected(true, QString());
		return true;
	}

	libusb_init(nullptr);
	libusb_set_debug(nullptr, DEBUG_LEVEL);
	qDebug() << "initted...";
//	libusb_device **devices;
//	int count = libusb_get_device_list(nullptr, &devices);
//	for (int i = 0; i < count; ++i) {
//		libusb_device_descriptor desc;
//		libusb_get_device_descriptor(devices[i], &desc);
//		qDebug() << "DEVICE:" << desc.idProduct << desc.idVendor;
//	}

	mHandle = libusb_open_device_with_vid_pid(nullptr, ATMEL_VID, SAMBA_PID);
	if (!mHandle) {
		emit connected(false, tr("Cannot find NXT device in resetted mode. Check robot resetted, connected "\
				"and ticking and try again."));
		return false;
	}

	if (libusb_kernel_driver_active(mHandle, NXT_INTERFACE_NUMBER)) {
		libusb_detach_kernel_driver(mHandle, NXT_INTERFACE_NUMBER);
	}

	if (libusb_set_configuration(mHandle, 1/*NXT_CONFIGURATION_NB*/) < 0) {
		emit connected(false, tr("USB Device configuration problem. Please contact developers."));
		libusb_close(mHandle);
		mHandle = nullptr;
		return false;
	}

	if (libusb_claim_interface(mHandle, NXT_INTERFACE_NUMBER) < 0) {
		emit connected(false, tr("NXT device is already used by another software."));
		libusb_close(mHandle);
		mHandle = nullptr;
		return false;
	}

	qDebug() << "handshaking...";
	QByteArray getFirmwareCommand(4, 0);
	getFirmwareCommand[0] = '\0';
	getFirmwareCommand[1] = '\0';
	getFirmwareCommand[2] = enums::telegramType::directCommandResponseRequired;
	getFirmwareCommand[3] = 0x88;

	QByteArray handshakeResponse;
	send(getFirmwareCommand, 9, handshakeResponse);
	if (handshakeResponse.isEmpty()) {
		emit connected(false, tr("NXT handshake procedure failed. Please contact developers."));
		libusb_close(mHandle);
		mHandle = nullptr;
		return false;
	}

	mFirmwareMode = true;
	emit connected(true, QString());
	return true;
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
	return buffer.size() >= 3 && (buffer[2] == enums::telegramType::directCommandResponseRequired
			|| buffer[2] == enums::telegramType::systemCommandResponseRequired);
}
