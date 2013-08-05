#include <QtCore/QMetaType>

#include "robotCommunicator.h"

#include "../../thirdparty/qextserialport/src/qextserialenumerator.h"
#include "../../thirdparty/qextserialport/src/qextserialport.h"

using namespace qReal::interpreters;
using namespace qReal::interpreters::robots::details;

RobotCommunicator::RobotCommunicator()
		: mRobotCommunicationThreadObject(NULL)
{
	qRegisterMetaType<robots::enums::inputPort::InputPortEnum>("inputPort::InputPortEnum");
}

RobotCommunicator::~RobotCommunicator()
{
	mRobotCommunicationThreadObject->allowLongJobs(false);
	mRobotCommunicationThread.quit();
	mRobotCommunicationThread.wait();
	delete mRobotCommunicationThreadObject;
}

void RobotCommunicator::send(QObject *addressee, QByteArray const &buffer, unsigned const responseSize)
{
	emit threadSend(addressee, buffer, responseSize);
}

void RobotCommunicator::sendI2C(QObject *addressee, QByteArray const &buffer
		, unsigned const responseSize, robots::enums::inputPort::InputPortEnum const port)
{
	emit threadSendI2C(addressee, buffer, responseSize, static_cast<robots::enums::inputPort::InputPortEnum>(port));
}

void RobotCommunicator::connect()
{
	emit threadConnect();
}

void RobotCommunicator::disconnect()
{
	emit threadDisconnect();
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

void RobotCommunicator::onErrorOccured(const QString &message)
{
	emit errorOccured(message);
}

void RobotCommunicator::setRobotCommunicationThreadObject(RobotCommunicationThreadInterface *robotCommunication)
{
	if (mRobotCommunicationThreadObject) {
		mRobotCommunicationThreadObject->allowLongJobs(false);
	}

	mRobotCommunicationThread.quit();
	mRobotCommunicationThread.wait();
	delete mRobotCommunicationThreadObject;
	mRobotCommunicationThreadObject = robotCommunication;
	mRobotCommunicationThreadObject->moveToThread(&mRobotCommunicationThread);
	mRobotCommunicationThreadObject->allowLongJobs();
	mRobotCommunicationThread.start();

	QObject::connect(this, SIGNAL(threadConnect()), mRobotCommunicationThreadObject, SLOT(connect()));
	QObject::connect(this, SIGNAL(threadReconnect()), mRobotCommunicationThreadObject, SLOT(reconnect()));
	QObject::connect(this, SIGNAL(threadDisconnect()), mRobotCommunicationThreadObject, SLOT(disconnect()));
	QObject::connect(this, SIGNAL(threadSend(QObject*, QByteArray, unsigned)), mRobotCommunicationThreadObject, SLOT(send(QObject*, QByteArray, unsigned)));
	QObject::connect(this, SIGNAL(threadSendI2C(QObject*, QByteArray, unsigned, robots::enums::inputPort::InputPortEnum))
			, mRobotCommunicationThreadObject, SLOT(sendI2C(QObject*, QByteArray, unsigned, robots::enums::inputPort::InputPortEnum)));

	QObject::connect(mRobotCommunicationThreadObject, SIGNAL(connected(bool)), this, SLOT(connectedSlot(bool)));
	QObject::connect(mRobotCommunicationThreadObject, SIGNAL(disconnected()), this, SLOT(disconnectedSlot()));
	QObject::connect(mRobotCommunicationThreadObject, SIGNAL(response(QObject*, QByteArray)), this, SLOT(responseSlot(QObject*, QByteArray)));
	QObject::connect(mRobotCommunicationThreadObject, SIGNAL(errorOccured(QString)), this, SLOT(onErrorOccured(QString)));

	mRobotCommunicationThreadObject->checkConsistency();
}
