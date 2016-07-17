/* Copyright 2012-2016 Yurii Litvinov, Dmitry Mordvinov, CyberTech Labs Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include <QtCore/QMetaType>
#include <QtCore/QMetaObject>

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
	QMetaObject::invokeMethod(mRobotCommunicationThreadObject, "connect");
}

void RobotCommunicator::disconnect()
{
	QMetaObject::invokeMethod(mRobotCommunicationThreadObject, "disconnect");
}

RobotCommunicationThreadInterface *RobotCommunicator::currentCommunicator() const
{
	return mRobotCommunicationThreadObject;
}

void RobotCommunicator::setRobotCommunicationThreadObject(RobotCommunicationThreadInterface *robotCommunication)
{
	if (mRobotCommunicationThreadObject) {
		mRobotCommunicationThreadObject->allowLongJobs(false);
	}

	mRobotCommunicationThread.quit();
	mRobotCommunicationThread.wait();
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
	QObject::connect(mRobotCommunicationThreadObject, &RobotCommunicationThreadInterface::messageArrived
			, this, &RobotCommunicator::messageArrived);
}
