#include "robotCommunication.h"

#include <QtCore/QDebug>

#include "../../thirdparty/qextserialport/src/qextserialenumerator.h"
#include "../../thirdparty/qextserialport/src/qextserialport.h"

using namespace qReal::interpreters::robots;

RobotCommunication::RobotCommunication(QString const &portName)
		: mPortName(portName)
		, mRobotCommunicationThreadObject(NULL)
{

}

RobotCommunication::~RobotCommunication()
{
	mRobotCommunicationThread.quit();
	mRobotCommunicationThread.wait();
}

void RobotCommunication::send(QObject *addressee, QByteArray const &buffer, unsigned const responseSize)
{
	emit threadSend(addressee, buffer, responseSize);
}

void RobotCommunication::sendI2C(QObject *addressee, QByteArray const &buffer
		, unsigned const responseSize, inputPort::InputPortEnum const &port)
{
	emit threadSendI2C(addressee, buffer, responseSize, static_cast<inputPort::InputPortEnum>(port));
}

void RobotCommunication::connect()
{
	emit threadConnect(mPortName);
}

void RobotCommunication::disconnect()
{
	emit threadDisconnect();
}

void RobotCommunication::setPortName(QString const &portName)
{
	bool needReconnect = portName != mPortName;
	mPortName = portName;
	if (needReconnect)
		emit threadReconnect(mPortName);
}

void RobotCommunication::connectedSlot(bool success)
{
	emit connected(success);
}

void RobotCommunication::disconnectedSlot()
{
	emit disconnected();
}

void RobotCommunication::responseSlot(QObject *addressee, QByteArray const &buffer)
{
	emit response(addressee, buffer);
}

void RobotCommunication::setRobotCommunicationThreadObject(RobotCommunicationThreadInterface *robotCommunication)
{
	mRobotCommunicationThread.quit();
	mRobotCommunicationThread.wait();
	delete mRobotCommunicationThreadObject;
	mRobotCommunicationThreadObject = robotCommunication;
	mRobotCommunicationThreadObject->moveToThread(&mRobotCommunicationThread);
	mRobotCommunicationThread.start();

	QObject::connect(this, SIGNAL(threadConnect(QString)), mRobotCommunicationThreadObject, SLOT(connect(QString)));
	QObject::connect(this, SIGNAL(threadReconnect(QString)), mRobotCommunicationThreadObject, SLOT(reconnect(QString)));
	QObject::connect(this, SIGNAL(threadDisconnect()), mRobotCommunicationThreadObject, SLOT(disconnect()));
	QObject::connect(this, SIGNAL(threadSend(QObject*, QByteArray, unsigned)), mRobotCommunicationThreadObject, SLOT(send(QObject*, QByteArray, unsigned)));
	QObject::connect(this, SIGNAL(threadSendI2C(QObject*, QByteArray, unsigned, inputPort::InputPortEnum))
			, mRobotCommunicationThreadObject, SLOT(sendI2C(QObject*, QByteArray, unsigned, inputPort::InputPortEnum)));

	QObject::connect(mRobotCommunicationThreadObject, SIGNAL(connected(bool)), this, SLOT(connectedSlot(bool)));
	QObject::connect(mRobotCommunicationThreadObject, SIGNAL(disconnected()), this, SLOT(disconnectedSlot()));
	QObject::connect(mRobotCommunicationThreadObject, SIGNAL(response(QObject*, QByteArray)), this, SLOT(responseSlot(QObject*, QByteArray)));
}
