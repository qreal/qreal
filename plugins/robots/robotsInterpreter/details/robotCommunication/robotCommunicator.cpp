#include "robotCommunicator.h"

#include "../../thirdparty/qextserialport/src/qextserialenumerator.h"
#include "../../thirdparty/qextserialport/src/qextserialport.h"
using namespace qReal::interpreters::robots;

RobotCommunicator::RobotCommunicator(QString const &portName)
		: mPortName(portName)
		, mRobotCommunicationThreadObject(NULL)
{
}

RobotCommunicator::~RobotCommunicator()
{
	mRobotCommunicationThread.quit();
	mRobotCommunicationThread.wait();
}

void RobotCommunicator::send(QObject *addressee, QByteArray const &buffer, unsigned const responseSize)
{
	emit threadSend(addressee, buffer, responseSize);
}

void RobotCommunicator::sendI2C(QObject *addressee, QByteArray const &buffer
		, unsigned const responseSize, inputPort::InputPortEnum const &port)
{
	emit threadSendI2C(addressee, buffer, responseSize, static_cast<inputPort::InputPortEnum>(port));
}

void RobotCommunicator::connect()
{
	emit threadConnect(mPortName);
}

void RobotCommunicator::disconnect()
{
	emit threadDisconnect();
}

void RobotCommunicator::setPortName(QString const &portName)
{
	bool needReconnect = portName != mPortName;
	mPortName = portName;
	if (needReconnect) {
		emit threadReconnect(mPortName);
	}
}

void RobotCommunicator::connectedSlot(bool success)
{
	emit connected(success);
}

void RobotCommunicator::disconnectedSlot()
{
	emit disconnected();
}

void RobotCommunicator::responseSlot(QObject *addressee, QByteArray const &buffer)
{
	emit response(addressee, buffer);
}

void RobotCommunicator::setRobotCommunicationThreadObject(RobotCommunicationThreadInterface *robotCommunication)
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
