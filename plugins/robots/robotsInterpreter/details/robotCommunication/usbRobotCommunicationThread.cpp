#include "usbRobotCommunicationThread.h"

#include "../../thirdparty/qextserialport/src/qextserialenumerator.h"
#include "../../thirdparty/qextserialport/src/qextserialport.h"
#include "../tracer.h"

using namespace qReal::interpreters::robots;
using namespace details;

UsbRobotCommunicationThread::UsbRobotCommunicationThread():
	mActive(false), mNXTHandle(0)
{
}

UsbRobotCommunicationThread::~UsbRobotCommunicationThread()
{
}

bool UsbRobotCommunicationThread::isOpen()
{
	return mActive && mFantom.isAvailable();
}

void UsbRobotCommunicationThread::connect(QString const &portName)
{
	Q_UNUSED(portName);

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
			if (resName.toUpper().contains("USB"))
				break;
		}
		if (status == kStatusNoError) {
			mNXTHandle = mFantom.nFANTOM100_iNXTIterator_getNXT(nxtIterator, status);
			mActive = status == kStatusNoError;
		}
		mFantom.nFANTOM100_destroyNXTIterator(nxtIterator, status);
	}
	emit connected(mActive);
}

void UsbRobotCommunicationThread::send(QObject *addressee
		, QByteArray const &buffer, unsigned const responseSize)
{
	send(buffer, responseSize, addressee);
}

void UsbRobotCommunicationThread::send(QByteArray const &buffer, unsigned const responseSize, QObject *addressee)
{
	Tracer::debug(tracer::robotCommunication, "UsbRobotCommunicationThread::send", "Sending:");
	debugPrint(buffer, true);

	int status = 0;
	QByteArray newBuffer;
	for (int i = 3; i < buffer.length() - 1; i++)
		newBuffer[i - 3] = buffer[i];

	QByteArray outputBuffer;
	outputBuffer.resize(responseSize);
	if (buffer[2] != 0) {
		mFantom.nFANTOM100_iNXT_sendDirectCommand(mNXTHandle, false, newBuffer, newBuffer.length(), NULL, 0, status);
		emit response(addressee, QByteArray());
	} else {
		char *outputBufferPtr2 = new char[200];
		for (int i = 0; i < 200; i++) {
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
			command[3] = commandCode::RESETINPUTSCALEDVALUE;
			command[4] = port;

			mFantom.nFANTOM100_iNXT_sendDirectCommand(mNXTHandle, true, command, 2, outputBufferPtr2, 2, status);
		}

		mFantom.nFANTOM100_iNXT_sendDirectCommand(mNXTHandle, true, newBuffer, newBuffer.length(), outputBufferPtr2, responseSize - 3, status);

		outputBuffer[0] = responseSize - 2;
		outputBuffer[1] = 0;
		outputBuffer[2] = 2;
		for (unsigned i = 0; i < responseSize - 3; i++) {
			outputBuffer[i + 3] = outputBufferPtr2[i];
		}
		delete outputBufferPtr2;
		debugPrint(outputBuffer, false);
		emit response(addressee, outputBuffer);
	}
}

void UsbRobotCommunicationThread::reconnect(QString const &portName)
{
	connect(portName);
}

void UsbRobotCommunicationThread::disconnect()
{
	emit disconnected();
}

void UsbRobotCommunicationThread::sendI2C(QObject *addressee
		, QByteArray const &buffer, unsigned const responseSize
		, inputPort::InputPortEnum const &port)
{
	Q_UNUSED(addressee)
	Q_UNUSED(buffer)
	Q_UNUSED(responseSize)
	Q_UNUSED(port)
}

void UsbRobotCommunicationThread::debugPrint(QByteArray const &buffer, bool out)
{
	QString tmp = "";
	for (int i = 0; i < buffer.length(); i++) {
		tmp += QString::number(static_cast<unsigned char>(buffer[i]));
		tmp += " ";
	}
	Tracer::debug(tracer::robotCommunication, "UsbRobotCommunicationThread::debugPrint", (out ? ">" : "<") + tmp);
}
