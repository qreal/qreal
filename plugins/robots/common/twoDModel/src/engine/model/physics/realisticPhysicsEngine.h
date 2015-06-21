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

#include "physicsEngineBase.h"

namespace twoDModel {
namespace model {

class Timeline;

namespace physics {

/// An implementation of 2D model physical engine with some realistic effects (like friction emulation)
class RealisticPhysicsEngine : public PhysicsEngineBase
{
public:
	explicit RealisticPhysicsEngine(const WorldModel &worldModel
			, const QList<RobotModel *> &robots
			, const Timeline &timeline);

	QVector2D positionShift(RobotModel &robot) const override;
	qreal rotation(RobotModel &robot) const override;
	void recalculateParameters(qreal timeInterval) override;

	void addRobot(RobotModel * const robot) override;
	void removeRobot(RobotModel * const robot) override;

private:
	/// Recalculates parameters for one robot.
	void recalculateParameters(qreal timeInterval, RobotModel &robot);

	/// Counts and returns traction force vector taking into consideration engines speed and placement
	void countTractionForceAndItsMoment(qreal speed1, qreal speed2, bool breakMode
			, const QVector2D &direction, RobotModel &robot);

	/// Applies all forces currently acting on robot
	void recalculateVelocity(qreal timeInterval, RobotModel &robot);
	void applyRotationalFrictionForce(qreal timeInterval, const QVector2D &direction, RobotModel &robot);

	/// Calculates forces and force moments acting on the robot from the walls
	void findCollision(const QPainterPath &robotBoundingRegion
			, const QPainterPath &wallBoundingRegion
			, const QPointF &rotationCenter
			, RobotModel &robot);

	QMap<RobotModel *, QVector2D> mTractionForce;
	QMap<RobotModel *, QVector2D> mReactionForce;
	QMap<RobotModel *, QVector2D> mWallsFrictionForce;
	QMap<RobotModel *, QVector2D> mGettingOutVector;
	QMap<RobotModel *, qreal> mForceMomentDecrement;
	QMap<RobotModel *, qreal> mForceMoment;

	QMap<RobotModel *, qreal> mAngularVelocity;
	QMap<RobotModel *, QVector2D> mVelocity;

	QMap<RobotModel *, QVector2D> mPositionShift;
	QMap<RobotModel *, qreal> mRotation;
};

}
}
}
