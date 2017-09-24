/* Copyright 2017 Dmitry Mordvinov, Gleb Zakharov
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

#include <Box2D/Common/b2Math.h>
#include "twoDModel/engine/model/worldModel.h"

class b2World;

namespace twoDModel {
	namespace view {
		class TwoDModelScene;
		class RobotItem;
	}
	namespace model {
	namespace physics {
	namespace parts {
		class box2DRobot;
		class box2DWheel;
		class box2DWall;
}

class box2DPhysicsEngine : public PhysicsEngineBase
{
public:
	box2DPhysicsEngine(const WorldModel &worldModel, const QList<RobotModel *> robots);
	~box2DPhysicsEngine();
	QVector2D positionShift(RobotModel &robot) const override;
	qreal rotation(RobotModel &robot) const override;
	void addRobot(RobotModel * const robot) override;
	void addRobot(RobotModel * const robot, QPointF pos, qreal angle);
	void removeRobot(RobotModel * const robot) override;
	void recalculateParameters(qreal timeInterval) override;

	float pxToCm(qreal px) const;
	b2Vec2 pxToCm(const QPointF posInPx) const;
	qreal cmToPx(float cm) const;
	QPointF cmToPx(const b2Vec2 posInCm) const;
	float32 pxToM(qreal px) const;
	qreal mToPx(float32 m) const;

	b2Vec2 positionToBox2D(QPointF sceneCoords) const;
	b2Vec2 positionToBox2D(float32 x, float32 y) const;
	QPointF positionToScene(b2Vec2 boxCoords) const;
	QPointF positionToScene(float32 x, float32 y) const;
	float32 angleToBox2D(qreal sceneAngle) const;
	qreal angleToScene(float32 boxAngle) const;

	b2World &box2DWorld(){
		return *mWorld.data();
	}

public slots:
	void onWallResize();
	void onWallDragged(items::WallItem *wall);
	void onRobotStartPositionChanged(const QPointF &newPos, twoDModel::model::RobotModel *robot);
	void onRobotStartAngleChanged(const qreal newAngle, twoDModel::model::RobotModel *robot);
	void onMouseReleased(QPointF newPos, qreal newAngle);
	void onMousePressed();
	void onRecoverRobotPosition(QPointF pos);

protected:
	void onPixelsInCmChanged(qreal value) override;
	void itemAdded(items::SolidItem * const item) override;
	void itemRemoved(QGraphicsItem * const item) override;

private:
	void drawDebugRobot(model::RobotModel* const robot);
	void createDebugRobot(model::RobotModel* const robot);

	twoDModel::view::TwoDModelScene *mScene;
	qreal mPixelsInCm;
	QScopedPointer<b2World> mWorld;

	QMap<RobotModel *, parts::box2DRobot *> mBox2DRobots;  // Takes ownership on b2Body instances
	QMap<RobotModel *, parts::box2DWheel *> mLeftWheels;  // Takes ownership on b2WheelJoint instances
	QMap<RobotModel *, parts::box2DWheel *> mRightWheels;  // Takes ownership on b2WheelJoint instances
	QMap<QGraphicsItem *, parts::box2DWall *> mBox2DWalls;  // Takes ownership on b2Body instances
	items::WallItem *mCurrentWall; // Doesn't take ownership
	bool mMouseJustReleased = false;
	bool firstSetPos = true;

	b2Vec2 mPrevPosition;
	float32 mPrevAngle;

	QGraphicsRectItem *robotItem = nullptr;
	QGraphicsRectItem *wheel1Item = nullptr;
	QGraphicsRectItem *wheel2Item = nullptr;
	QMap<parts::box2DWall *, QGraphicsRectItem *> mBlackWallsItems;
};

}
}
}
