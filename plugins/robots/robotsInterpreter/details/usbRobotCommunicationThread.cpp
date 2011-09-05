#include "usbRobotCommunicationThread.h"

#include "fantomMethods.h"

#include <QtCore/QDebug>

#include "../../thirdparty/qextserialport/src/qextserialenumerator.h"
#include "../../thirdparty/qextserialport/src/qextserialport.h"

using namespace qReal::interpreters::robots;

UsbRobotCommunicationThread::UsbRobotCommunicationThread():
	mActive(false), mResPort(""), mBTName(""), mUseBT(false), mBluetoothSearchTimeout(30), mPort(""), mNXTHandle(0)
{}

bool UsbRobotCommunicationThread::getIsOpen()
{
	return mActive;
}

bool UsbRobotCommunicationThread::isOpen()
{
	bool result = getIsOpen();
	return result && FantomAPILoaded;

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
		nxtIterator = nFANTOM100_createNXTIterator((unsigned char)(getUseBT()), getBST(), status);
		while (status >= kStatusNoError) {
			nFANTOM100_iNXTIterator_getName(nxtIterator, resNamePC, status2);
			QString resName = QString(resNamePC);
			for (int i = 0; i < resName.length() - 1; i++) {
				if(resName[i].isLetter())
					resName[i] = resName[i].toUpper();
			}
			if (resName.indexOf(pName) >= 0)
				break;
		}
		if (status >= kStatusNoError) {
			mNXTHandle = nFANTOM100_iNXTIterator_getNXT(nxtIterator, status);
			if (status >= kStatusNoError) {
				mActive = true; //in interface brick_common???
				result = true;
			}
		}
		nFANTOM100_destroyNXTIterator(nxtIterator, status);
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
		nxtIterator = nFANTOM100_createNXTIterator(1, getBST(), status);
		while (status >= kStatusNoError) {
			nFANTOM100_iNXTIterator_getName(nxtIterator, resNamePC, status2);
			resName = QString(resNamePC).toUpper();
			StringList.append(resName);
			nFANTOM100_iNXTIterator_advance(nxtIterator,  status);
		}
		nFANTOM100_destroyNXTIterator(nxtIterator,  status);
		nFANTOM100_iNXT_findDeviceInFirmwareDownloadMode(resNamePC, status);
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
	if (buffer.size() >= 3 && buffer[2] == 0x00) {
		QByteArray const result = receive(responseSize);
		emit response(addressee, result);
	} else {
		emit response(addressee, QByteArray());
	}
}

void UsbRobotCommunicationThread::send(QByteArray const &buffer, unsigned const responseSize, QObject *addressee)
{
	int status;
	int j = 0;
	QByteArray newbuffer;
	for (int i = 3; i < buffer.length() - 1; i++)
	{
		newbuffer[j] = buffer[i];
		j++;
	}
	QByteArray  outputBufferPtr;
	outputBufferPtr.resize(responseSize);
	if (buffer[2] != 0) {
		nFANTOM100_iNXT_sendDirectCommand(mNXTHandle, false, newbuffer, newbuffer.length(), NULL, 0, status);
	} else {
		char *outputBufferPtr2 = new char [200];
		for (int i = 0; i < 200; i++) {
			outputBufferPtr2[i] = 0;
		}
		nFANTOM100_iNXT_sendDirectCommand(mNXTHandle, true, newbuffer, newbuffer.length(), outputBufferPtr2, responseSize - 3, status);
		outputBufferPtr[0] = responseSize;
		outputBufferPtr[1] = 0;
		outputBufferPtr[2] = 0;
		for (int i = 0; i < responseSize - 3; i++) {
			outputBufferPtr[i + 3] = outputBufferPtr2[i];
		}
		QString buffer = "";
		for (int i = 0; i < outputBufferPtr.length(); i++) {
			buffer += (byte)outputBufferPtr[i];
			buffer += " ";
		}
		qDebug() << buffer;
		qDebug() << "[14] = " << (byte)outputBufferPtr[14] << "[15] =" << (byte)outputBufferPtr[15];
//		if ((byte)outputBufferPtr[14] == 181)
//			outputBufferPtr[14] = 1;
//		if ((byte)outputBufferPtr[14] == 255)
//			outputBufferPtr[14] = 0;
		emit response(addressee, outputBufferPtr);
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
