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

#include "physicsEngineBase.h"

#include <qrutils/mathUtils/math.h>

#include "twoDModel/engine/model/worldModel.h"

using namespace twoDModel::model::physics;

PhysicsEngineBase::PhysicsEngineBase(const WorldModel &worldModel, const QList<RobotModel *> robots)
	: mWorldModel(worldModel)
{
	for (RobotModel * const robot : robots) {
		addRobot(robot);
	}
}

PhysicsEngineBase::~PhysicsEngineBase()
{
}

void PhysicsEngineBase::addRobot(twoDModel::model::RobotModel * const robot)
{
	if (!mRobots.contains(robot)) {
		mRobots.append(robot);
	}
}

void PhysicsEngineBase::removeRobot(twoDModel::model::RobotModel * const robot)
{
	mRobots.removeAll(robot);
}

void PhysicsEngineBase::wakeUp()
{
}

void PhysicsEngineBase::onPixelsInCmChanged(qreal value)
{
	Q_UNUSED(value)
}

void PhysicsEngineBase::itemAdded(QGraphicsItem * const item)
{
	Q_UNUSED(item)
}

void PhysicsEngineBase::itemRemoved(QGraphicsItem * const item)
{
	Q_UNUSED(item)
}

qreal PhysicsEngineBase::wheelLinearSpeed(RobotModel &robot, const RobotModel::Wheel &wheel) const
{
	return wheel.spoiledSpeed * 2 * mathUtils::pi * wheel.radius * robot.info().onePercentAngularVelocity() / 360;
}
