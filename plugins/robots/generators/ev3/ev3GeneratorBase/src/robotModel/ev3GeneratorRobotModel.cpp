/* Copyright 2007-2015 QReal Research Group
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

#include "ev3GeneratorBase/robotModel/ev3GeneratorRobotModel.h"

#include <ev3Kit/communication/ev3RobotCommunicationThread.h>

using namespace ev3::robotModel;

Ev3GeneratorRobotModel::Ev3GeneratorRobotModel(const QString &kitId
		, const QString &robotId
		, const QString &name
		, const QString &friendlyName
		, int priority
		, communication::Ev3RobotCommunicationThread * const communicator)
	: Ev3RobotModelBase(kitId, robotId)
	, mName(name)
	, mFriendlyName(friendlyName)
	, mPriority(priority)
	, mCommunicator(communicator)
{
}

Ev3GeneratorRobotModel::~Ev3GeneratorRobotModel()
{
}

QString Ev3GeneratorRobotModel::name() const
{
	return mName;
}

QString Ev3GeneratorRobotModel::friendlyName() const
{
	return mFriendlyName;
}

bool Ev3GeneratorRobotModel::needsConnection() const
{
	return false;
}

bool Ev3GeneratorRobotModel::interpretedModel() const
{
	return false;
}

int Ev3GeneratorRobotModel::priority() const
{
	return mPriority;
}

ev3::communication::Ev3RobotCommunicationThread *Ev3GeneratorRobotModel::communicator()
{
	return mCommunicator.data();
}
