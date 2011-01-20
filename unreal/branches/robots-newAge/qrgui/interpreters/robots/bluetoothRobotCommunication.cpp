#include "bluetoothRobotCommunication.h"

#include <QtCore/QDebug>

#include "../../thirdparty/qextserialport-1.2win-alpha/qextserialenumerator.h"
#include "../../thirdparty/qextserialport-1.2win-alpha/qextserialport.h"

using namespace qReal::interpreters::robots;

BluetoothRobotCommunication::BluetoothRobotCommunication(QString const &portName)
	: mPortName(portName)
{
	mRobotCommunicationThreadObject.moveToThread(&mRobotCommunicationThread);
	mRobotCommunicationThread.start();

	QObject::connect(this, SIGNAL(threadConnect(QString)), &mRobotCommunicationThreadObject, SLOT(connect(QString)));
	QObject::connect(this, SIGNAL(threadReconnect(QString)), &mRobotCommunicationThreadObject, SLOT(reconnect(QString)));
	QObject::connect(this, SIGNAL(threadDisconnect()), &mRobotCommunicationThreadObject, SLOT(disconnect()));
	QObject::connect(this, SIGNAL(threadSend(QObject*, QByteArray)), &mRobotCommunicationThreadObject, SLOT(send(QObject*, QByteArray)));

	QObject::connect(&mRobotCommunicationThreadObject, SIGNAL(connected()), this, SLOT(connectedSlot()));
	QObject::connect(&mRobotCommunicationThreadObject, SIGNAL(disconnected()), this, SLOT(disconnectedSlot()));
	QObject::connect(&mRobotCommunicationThreadObject, SIGNAL(response(QObject*, QByteArray)), this, SLOT(responseSlot(QObject*, QByteArray)));
}

void BluetoothRobotCommunication::send(QObject *addressee, QByteArray const &buffer)
{
	emit threadSend(addressee, buffer);
}

void BluetoothRobotCommunication::connect()
{
	emit threadConnect(mPortName);
}

void BluetoothRobotCommunication::disconnect()
{
	emit threadDisconnect();
}

void BluetoothRobotCommunication::setPortName(QString const &portName)
{
	bool needReconnect = portName != mPortName;
	mPortName = portName;
	if (needReconnect)
		emit threadReconnect(mPortName);
}

void BluetoothRobotCommunication::connectedSlot()
{
	emit connected();
}

void BluetoothRobotCommunication::disconnectedSlot()
{
	emit disconnected();
}

void BluetoothRobotCommunication::responseSlot(QObject *addressee, QByteArray const &buffer)
{
	emit response(addressee, buffer);
}
