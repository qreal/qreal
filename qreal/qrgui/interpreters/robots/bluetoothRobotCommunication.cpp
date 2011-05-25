#include "bluetoothRobotCommunication.h"

#include <QtCore/QDebug>

#include "../../thirdparty/qextserialport/src/qextserialenumerator.h"
#include "../../thirdparty/qextserialport/src/qextserialport.h"

using namespace qReal::interpreters::robots;

BluetoothRobotCommunication::BluetoothRobotCommunication(QString const &portName)
	: mPortName(portName)
{
	mRobotCommunicationThreadObject.moveToThread(&mRobotCommunicationThread);
	mRobotCommunicationThread.start();

	QObject::connect(this, SIGNAL(threadConnect(QString)), &mRobotCommunicationThreadObject, SLOT(connect(QString)));
	QObject::connect(this, SIGNAL(threadReconnect(QString)), &mRobotCommunicationThreadObject, SLOT(reconnect(QString)));
	QObject::connect(this, SIGNAL(threadDisconnect()), &mRobotCommunicationThreadObject, SLOT(disconnect()));
	QObject::connect(this, SIGNAL(threadSend(QObject*, QByteArray, unsigned)), &mRobotCommunicationThreadObject, SLOT(send(QObject*, QByteArray, unsigned)));
	QObject::connect(this, SIGNAL(threadSendI2C(QObject*, QByteArray, unsigned, details::lowLevelInputPort::InputPortEnum))
			, &mRobotCommunicationThreadObject, SLOT(sendI2C(QObject*, QByteArray, unsigned, details::lowLevelInputPort::InputPortEnum)));

	QObject::connect(&mRobotCommunicationThreadObject, SIGNAL(connected(bool)), this, SLOT(connectedSlot(bool)));
	QObject::connect(&mRobotCommunicationThreadObject, SIGNAL(disconnected()), this, SLOT(disconnectedSlot()));
	QObject::connect(&mRobotCommunicationThreadObject, SIGNAL(response(QObject*, QByteArray)), this, SLOT(responseSlot(QObject*, QByteArray)));
}

BluetoothRobotCommunication::~BluetoothRobotCommunication()
{
	mRobotCommunicationThread.quit();
	mRobotCommunicationThread.wait();
}

void BluetoothRobotCommunication::send(QObject *addressee, QByteArray const &buffer, unsigned const responseSize)
{
	emit threadSend(addressee, buffer, responseSize);
}

void BluetoothRobotCommunication::sendI2C(QObject *addressee, QByteArray const &buffer
		, unsigned const responseSize, inputPort::InputPortEnum const &port)
{
	emit threadSendI2C(addressee, buffer, responseSize, static_cast<details::lowLevelInputPort::InputPortEnum>(port));
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

void BluetoothRobotCommunication::connectedSlot(bool success)
{
	emit connected(success);
}

void BluetoothRobotCommunication::disconnectedSlot()
{
	emit disconnected();
}

void BluetoothRobotCommunication::responseSlot(QObject *addressee, QByteArray const &buffer)
{
	emit response(addressee, buffer);
}
