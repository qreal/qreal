#include "usbRobotCommunicationThread.h"

#include <QtCore/QDebug>

#include "../../thirdparty/qextserialport/src/qextserialenumerator.h"
#include "../../thirdparty/qextserialport/src/qextserialport.h"

using namespace qReal::interpreters::robots;

UsbRobotCommunicationThread::UsbRobotCommunicationThread():
	mActive(false), mResPort(""), mBTName(""), mUseBT(false), mBluetoothSearchTimeout(30), mPort(""), mNXTHandle(0)
{
}

UsbRobotCommunicationThread::~UsbRobotCommunicationThread()
{
}

bool UsbRobotCommunicationThread::getIsOpen()
{
	return mActive;
}

bool UsbRobotCommunicationThread::isOpen()
{
	bool result = getIsOpen();
	return result && mFantom.isAvailable();

}

QString UsbRobotCommunicationThread::getBluetoothName()
{
	return mBTName;
}

void UsbRobotCommunicationThread::setBluetoothName(QString name)
{
	mBTName = name;
}

bool UsbRobotCommunicationThread::getUseBT()
{
	return mUseBT;
}

void UsbRobotCommunicationThread::setUseBT(bool value)
{
	if (mUseBT != value) {
		mUseBT = value;
	}
}

unsigned long UsbRobotCommunicationThread::getBST() {
	return mBluetoothSearchTimeout;
}

void UsbRobotCommunicationThread::setBST(unsigned long value) {
	mBluetoothSearchTimeout = value;
}

void UsbRobotCommunicationThread::connect(QString const &portName)
{
	bool result = isOpen();
	QString resName, pName, bName;
	char resNamePC[10000];
	unsigned long nxtIterator;

	int status, status2;

	setPort("usb");
	if (!result) {
		resName = "";
		pName = mResPort.toUpper();
		bName = getBluetoothName().toUpper();
	}
	if (!(pName.length() > 4)) {
		nxtIterator = mFantom.nFANTOM100_createNXTIterator((unsigned char)(getUseBT()), getBST(), status);
		while (status >= kStatusNoError) {
			mFantom.nFANTOM100_iNXTIterator_getName(nxtIterator, resNamePC, status2);
			QString resName = QString(resNamePC);
			for (int i = 0; i < resName.length() - 1; i++) {
				if(resName[i].isLetter())
					resName[i] = resName[i].toUpper();
			}
			if (resName.indexOf(pName) >= 0)
				break;
		}
		if (status >= kStatusNoError) {
			mNXTHandle = mFantom.nFANTOM100_iNXTIterator_getNXT(nxtIterator, status);
			if (status >= kStatusNoError) {
				mActive = true; //in interface brick_common???
				result = true;
			}
		}
		mFantom.nFANTOM100_destroyNXTIterator(nxtIterator, status);
	}
	emit connected(true);
	//return result;
}

QString UsbRobotCommunicationThread::getPort()
{
	return mPort;
}

void UsbRobotCommunicationThread::setPort(QString value)
{
	if (QString::compare(value, mPort) != 0) {
		mPort = value;
		mResPort = mPort;
	}
}

QList<QString> UsbRobotCommunicationThread::NXTListWidget()
{
	QString resName;
	char resNamePC[10000];
	unsigned long nxtIterator;
	int status, status2;
	//bool result = false;
	QList<QString> StringList;
	StringList.clear();
	if (true) {
		resName = "";
		status = 0;
		nxtIterator = mFantom.nFANTOM100_createNXTIterator(1, getBST(), status);
		while (status >= kStatusNoError) {
			mFantom.nFANTOM100_iNXTIterator_getName(nxtIterator, resNamePC, status2);
			resName = QString(resNamePC).toUpper();
			StringList.append(resName);
			mFantom.nFANTOM100_iNXTIterator_advance(nxtIterator,  status);
		}
		mFantom.nFANTOM100_destroyNXTIterator(nxtIterator,  status);
		mFantom.nFANTOM100_iNXT_findDeviceInFirmwareDownloadMode(resNamePC, status);
		if (status >= kStatusNoError) {
			resName = QString(resNamePC).toUpper();
			StringList.append(resName);
		}
		//result = ListWidget.count() > 0;
	}
	//return result;
	return StringList;
}

void UsbRobotCommunicationThread::send(QObject *addressee
		, QByteArray const &buffer, unsigned const responseSize)
{
	this->setPort("usb");

	/*if (this->isOpen()) {
		QList<QString> StringList = NXTListWidget();
		QString robotId = "";
		for (int i = 0; i < StringList.count(); i++) {
			robotId += StringList.at(i);
		}
	} else {
		emit response(addressee, QByteArray());
		return;
	}*/

	send(buffer, responseSize, addressee);
}

void UsbRobotCommunicationThread::send(QByteArray const &buffer, unsigned const responseSize, QObject *addressee)
{
	qDebug() << "Sending";
	QString tmp = "";
	for (int i = 0; i < buffer.length(); i++) {
		tmp += QString::number((byte)buffer[i]);
		tmp += " ";
	}
	qDebug() << ">" << tmp;

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
		QString buffer = "";
		for (int i = 0; i < outputBuffer.length(); i++) {
			buffer += QString::number((byte)outputBuffer[i]);
			buffer += " ";
		}
		qDebug() << "<" << buffer;
//		qDebug() << "[14] = " << (byte)outputBuffer[14] << "[15] =" << (byte)outputBuffer[15];
//		if ((byte)outputBufferPtr[14] == 181)
//			outputBufferPtr[14] = 1;
//		if ((byte)outputBufferPtr[14] == 255)
//			outputBufferPtr[14] = 0;
		emit response(addressee, outputBuffer);
	}
}

QByteArray UsbRobotCommunicationThread::receive(int size) const
{
	return QByteArray();
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
}
