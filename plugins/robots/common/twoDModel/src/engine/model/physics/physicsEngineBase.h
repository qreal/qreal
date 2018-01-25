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

#pragma once

#include <QtGui/QVector2D>
#include <QtGui/QPainterPath>

#include "twoDModel/engine/model/robotModel.h"

namespace twoDModel {

namespace items {
class SolidItem;
}

namespace model {

class WorldModel;

namespace physics {

/// An interface for 2D model physical engine strategy. Calling recalculateParams() method
/// counts position shift and angle modification for the given time interval that can be get
/// with shift() and rotation() methods correspondingly.
class PhysicsEngineBase : public QObject
{
	Q_OBJECT

public:
	explicit PhysicsEngineBase(const WorldModel &worldModel, const QList<RobotModel *> robots);
	virtual ~PhysicsEngineBase();

	/// Returns item`s position delta after last recalculation.
	virtual QVector2D positionShift(RobotModel &robot) const = 0;

	/// Returns item`s angle delta after last recalculation.
	virtual qreal rotation(RobotModel &robot) const = 0;

	/// Adds new robot model for its position and rotation tracking.
	virtual void addRobot(RobotModel * const robot);

	/// Stops tracking robot`s position and rotation.
	virtual void removeRobot(RobotModel * const robot);

	/// Recalculates all solid items positions and angles.
	virtual void recalculateParameters(qreal timeInterval) = 0;

	/// Recalculates all solid items positions and angles correspond to world model changes.
	virtual void wakeUp();

protected:
	/// A useful method for counting wheel linear speed from interpreter`s speed.
	qreal wheelLinearSpeed(RobotModel &robot, const RobotModel::Wheel &wheel) const;

	const WorldModel &mWorldModel;
	QList<RobotModel *> mRobots;

protected slots:
	/// Processes modification of metrics system.
	virtual void onPixelsInCmChanged(qreal value);

	/// Starts tracking item`s position and rotation.
	virtual void itemAdded(QGraphicsItem * const item);

	/// Stops tracking item`s position and rotation.
	virtual void itemRemoved(QGraphicsItem * const item);
};

}
}
}
