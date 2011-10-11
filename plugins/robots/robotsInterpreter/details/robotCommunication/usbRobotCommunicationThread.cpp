#include "usbRobotCommunicationThread.h"

#include <QtCore/QDebug>

#include "../../thirdparty/qextserialport/src/qextserialenumerator.h"
#include "../../thirdparty/qextserialport/src/qextserialport.h"

using namespace qReal::interpreters::robots;

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
	qDebug() << "Sending";
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
		char *outputBufferPtr2 = new char [200];
		for (int i = 0; i < 200; i++) {
			outputBufferPtr2[i] = 0;
		}
		mFantom.nFANTOM100_iNXT_sendDirectCommand(mNXTHandle, true, newBuffer, newBuffer.length(), outputBufferPtr2, responseSize - 3, status);
		outputBuffer[0] = responseSize;
		outputBuffer[1] = 0;
		outputBuffer[2] = 2;
		for (unsigned i = 0; i < responseSize - 3; i++) {
			outputBuffer[i + 3] = outputBufferPtr2[i];
		}
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
		tmp += QString::number((char)buffer[i]);
		tmp += " ";
	}
	qDebug() << (out ? ">" : "<") << tmp;
}
