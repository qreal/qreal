#include "usbRobotCommunicationThread.h"

#include <QtCore/QDebug>
#include <time.h>

#include <qrkernel/settingsManager.h>
#include <plugins/robots/thirdparty/qextserialport/src/qextserialenumerator.h>
#include <plugins/robots/thirdparty/qextserialport/src/qextserialport.h>
#include <utils/tracer.h>

#include "commandConstants.h"

using namespace nxtKitInterpreter::communication;

unsigned const packetHeaderSize = 3;

UsbRobotCommunicationThread::UsbRobotCommunicationThread()
	: mActive(false)
	, mNXTHandle(0)
	, mKeepAliveTimer(new QTimer(this))
	, mStopped(false)
{
	QObject::connect(mKeepAliveTimer, SIGNAL(timeout()), this, SLOT(checkForConnection()));
}

UsbRobotCommunicationThread::~UsbRobotCommunicationThread()
{
	int status = 0;
	mFantom.nFANTOM100_destroyNXT(mNXTHandle, status);
}

bool UsbRobotCommunicationThread::isOpen()
{
	return mActive && mFantom.isAvailable();
}

void UsbRobotCommunicationThread::connect()
{
	if (!mFantom.isAvailable()) {
		return;
	}

	char resNamePC[10000];
	unsigned long nxtIterator;

	if (!isOpen()) {
		mActive = false;
		int status = 0;
		nxtIterator = mFantom.nFANTOM100_createNXTIterator(false, 30, status);
		while (status == kStatusNoError) {
			int status2 = 0;
			mFantom.nFANTOM100_iNXTIterator_getName(nxtIterator, resNamePC, status2);
			QString resName = QString(resNamePC);
			if (resName.toUpper().contains("USB")) {
				break;
			}
			if (mStopped) {
				return;
			}
		}
		if (status == kStatusNoError) {
			mNXTHandle = mFantom.nFANTOM100_iNXTIterator_getNXT(nxtIterator, status);
			mActive = status == kStatusNoError;
		}
		mFantom.nFANTOM100_destroyNXTIterator(nxtIterator, status);
	}
	emit connected(mActive);

	if (mActive) {
		mKeepAliveTimer->start(500);
	}
}

void UsbRobotCommunicationThread::send(QObject *addressee
		, QByteArray const &buffer, unsigned const responseSize)
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

void UsbRobotCommunicationThread::send(QByteArray const &buffer
		, unsigned const responseSize, QByteArray &outputBuffer)
{
	utils::Tracer::debug(utils::Tracer::robotCommunication, "UsbRobotCommunicationThread::send", "Sending:");

	int status = 0;
	QByteArray newBuffer;
	for (int i = packetHeaderSize; i < buffer.length(); i++) {
		newBuffer[i - packetHeaderSize] = buffer[i];
	}

	if (!isResponseNeeded(buffer)) {
		mFantom.nFANTOM100_iNXT_sendDirectCommand(mNXTHandle, false, newBuffer, newBuffer.length(), nullptr, 0, status);
	} else {
		unsigned const temporaryOutputBufferSize = 200;
		char *outputBufferPtr2 = new char[temporaryOutputBufferSize];
		for (unsigned i = 0; i < temporaryOutputBufferSize; i++) {
			outputBufferPtr2[i] = 0;
		}

		// TODO: Sending RESETINPUTSCALEDVALUE for every sensor before reading their values.
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

			mFantom.nFANTOM100_iNXT_sendDirectCommand(mNXTHandle, true, command, 2, outputBufferPtr2, 2, status);
		}

		mFantom.nFANTOM100_iNXT_sendDirectCommand(mNXTHandle, true, newBuffer
				, newBuffer.length(), outputBufferPtr2, responseSize - 3, status);

		outputBuffer[0] = responseSize - 2;
		outputBuffer[1] = 0;
		outputBuffer[2] = 2;
		for (unsigned  i = 0; i < responseSize - packetHeaderSize; i++) {
			outputBuffer[i + packetHeaderSize] = outputBufferPtr2[i];
		}
		delete outputBufferPtr2;
		debugPrint(outputBuffer, false);
	}
}

void UsbRobotCommunicationThread::reconnect()
{
	connect();
}

void UsbRobotCommunicationThread::disconnect()
{
	if (!mFantom.isAvailable()) {
		return;
	}
	mKeepAliveTimer->stop();
	int status = 0;
	mFantom.nFANTOM100_destroyNXT(mNXTHandle, status);
	mActive = false;
	emit disconnected();
}

void UsbRobotCommunicationThread::allowLongJobs(bool allow)
{
	mStopped = !allow;
}

void UsbRobotCommunicationThread::debugPrint(QByteArray const &buffer, bool out)
{
	QString tmp = "";
	for (int i = 0; i < buffer.length(); i++) {
		tmp += QString::number(static_cast<unsigned char>(buffer[i]));
		tmp += " ";
	}
	utils::Tracer::debug(utils::Tracer::robotCommunication, "UsbRobotCommunicationThread::debugPrint"
			, (out ? ">" : "<") + tmp);
}

void UsbRobotCommunicationThread::checkForConnection()
{
	QByteArray command(4, 0);

	command[3] = enums::commandCode::KEEPALIVE;

	int const keepAliveResponseSize = 9;

	QByteArray response;
	response.resize(keepAliveResponseSize);

	send(command, keepAliveResponseSize, response);

	if (response[3] == '\0') {
		emit disconnected();
	}
}

bool UsbRobotCommunicationThread::isResponseNeeded(QByteArray const &buffer)
{
	return buffer.size() >= 3 && buffer[2] == 0;
}

void UsbRobotCommunicationThread::checkConsistency()
{
	QString const selectedKit = qReal::SettingsManager::value("SelectedRobotKit").toString();
	if (selectedKit != "nxtKit") {
		return;
	}

	QString const selectedRobotModel = qReal::SettingsManager::value("SelectedModelFor" + selectedKit).toString();
	if (selectedRobotModel != "NxtRealRobotModel") {
		return;
	}

	if (!mFantom.isAvailable()) {
		QString const fantomDownloadLink = qReal::SettingsManager::value("fantomDownloadLink").toString();
		QString errorMessage = tr("Fantom Driver is unavailable. Usb connection to robot is impossible.");
		if (!fantomDownloadLink.isEmpty()) {
			errorMessage += tr(" You can download Fantom Driver on <a href='%1'>Lego website</a>")
					.arg(fantomDownloadLink);
		}

		emit errorOccured(errorMessage);
	}
}
