#include <QtCore/QMetaType>

#include "utils/robotCommunication/robotCommunicator.h"

#include <plugins/robots/thirdparty/qextserialport/src/qextserialenumerator.h>
#include <plugins/robots/thirdparty/qextserialport/src/qextserialport.h>

using namespace utils::robotCommunication;

RobotCommunicator::RobotCommunicator(QObject *parent)
	: QObject(parent)
	, mRobotCommunicationThreadObject(nullptr)
{
}

RobotCommunicator::~RobotCommunicator()
{
	if (mRobotCommunicationThreadObject) {
		mRobotCommunicationThreadObject->allowLongJobs(false);
	}

	mRobotCommunicationThread.quit();
	mRobotCommunicationThread.wait();
	delete mRobotCommunicationThreadObject;
}

void RobotCommunicator::send(QObject *addressee, const QByteArray &buffer, const unsigned responseSize)
{
	mRobotCommunicationThreadObject->send(addressee, buffer, responseSize);
}

void RobotCommunicator::send(const QByteArray &buffer, const unsigned responseSize, QByteArray &outputBuffer)
{
	mRobotCommunicationThreadObject->send(buffer, responseSize, outputBuffer);
}

void RobotCommunicator::connect()
{
	mRobotCommunicationThreadObject->connect();
}

void RobotCommunicator::disconnect()
{
	mRobotCommunicationThreadObject->disconnect();
}

void RobotCommunicator::checkConsistency()
{
	if (mRobotCommunicationThreadObject) {
		mRobotCommunicationThreadObject->checkConsistency();
	}
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

	QObject::connect(mRobotCommunicationThreadObject, &RobotCommunicationThreadInterface::connected
			, this, &RobotCommunicator::connected);
	QObject::connect(mRobotCommunicationThreadObject, &RobotCommunicationThreadInterface::disconnected
			, this, &RobotCommunicator::disconnected);
	QObject::connect(mRobotCommunicationThreadObject, &RobotCommunicationThreadInterface::response
			, this, &RobotCommunicator::response);
	QObject::connect(mRobotCommunicationThreadObject, &RobotCommunicationThreadInterface::errorOccured
			, this, &RobotCommunicator::errorOccured);

	mRobotCommunicationThreadObject->checkConsistency();
}
