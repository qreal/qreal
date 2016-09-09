/* Copyright 2015 QReal Research Group, Dmitry Mordvinov
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

class b2World;
class b2Body;
class b2Vec2;

namespace twoDModel {
namespace model {
namespace physics {

class Box2DPhysicsEngine : public PhysicsEngineBase
{
public:
	Box2DPhysicsEngine(const WorldModel &worldModel, const QList<RobotModel *> robots);
	~Box2DPhysicsEngine();

	QVector2D positionShift(RobotModel &robot) const override;
	qreal rotation(RobotModel &robot) const override;
	void addRobot(RobotModel * const robot) override;
	void removeRobot(RobotModel * const robot) override;
	void recalculateParameters(qreal timeInterval) override;

protected:
	void onPixelsInCmChanged(qreal value) override;
	void itemAdded(items::SolidItem * const item) override;
	void itemRemoved(items::SolidItem * const item) override;

private:
	inline float pxToCm(qreal px) const;
	inline b2Vec2 pxToCm(const QPointF &posInPx) const;
	inline qreal cmToPx(float cm) const;
	inline QPointF cmToPx(const b2Vec2 &posInCm) const;

	b2Body *wheel(const QPointF &coords, b2Body * const robot) const;

	qreal mPixelsInCm;
	QScopedPointer<b2World> mWorld;
	QMap<RobotModel *, b2Body *> mRobotBodies;  // Takes ownership on b2Body instances
	QMap<RobotModel *, b2Body *> mLeftWheels;  // Takes ownership on b2WheelJoint instances
	QMap<RobotModel *, b2Body *> mRightWheels;  // Takes ownership on b2WheelJoint instances
	QMap<QGraphicsItem *, b2Body *> mWallBodies;  // Takes ownership on b2Body instances
};

}
}
}
