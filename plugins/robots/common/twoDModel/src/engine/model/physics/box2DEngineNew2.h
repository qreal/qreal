/* Copyright 2017 QReal Research Group
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

#include "parts/box2DWheel.h"
#include "parts/box2DRobot.h"
#include "twoDModel/engine/model/worldModel.h"

class b2World;
class b2Body;
class b2Vec2;

namespace twoDModel {
namespace model {
namespace physics {

class box2DEngineNew2 : public PhysicsEngineBase
{
public:
	box2DEngineNew2(const WorldModel &worldModel, const QList<RobotModel *> robots);
	~box2DEngineNew2();
	QVector2D positionShift(RobotModel &robot) const override;
	qreal rotation(RobotModel &robot) const override;
	void addRobot(RobotModel * const robot) override;
	void removeRobot(RobotModel * const robot) override;
	void recalculateParameters(qreal timeInterval) override;

	inline float pxToCm(qreal px) const;
	inline b2Vec2 pxToCm(const QPointF posInPx) const;
	inline qreal cmToPx(float cm) const;
	inline QPointF cmToPx(const b2Vec2 posInCm) const;

protected:
	void onPixelsInCmChanged(qreal value) override;
	void itemAdded(items::SolidItem * const item) override;
	void itemRemoved(items::SolidItem * const item) override;

private:
	qreal mPixelsInCm;
	QScopedPointer<b2World> mWorld;

	parts::box2DRobot *robot();
	parts::box2DWheel *leftWheel();
	parts::box2DWheel *rightWheel();

	QMap<RobotModel *, b2Body *> mRobotBodies;  // Takes ownership on b2Body instances
	QMap<RobotModel *, b2Body *> mLeftWheels;  // Takes ownership on b2WheelJoint instances
	QMap<RobotModel *, b2Body *> mRightWheels;  // Takes ownership on b2WheelJoint instances
	QMap<QGraphicsItem *, b2Body *> mWallBodies;  // Takes ownership on b2Body instances

	b2Vec2 prevPosition;
	float32 prevAngle;
	float32 mass;

	b2Vec2 positionToBox2D(QPointF sceneCoords) const {
		return positionToBox2D(sceneCoords.x(), sceneCoords.y());
	}

	b2Vec2 positionToBox2D(float32 x, float32 y) const{
		QPointF invertedCoords = QPointF(x, -y);
		return 0.01f * pxToCm(invertedCoords);
	}

	QPointF positionToScene(float32 x, float32 y) const{
		b2Vec2 invertedCoords = b2Vec2(x, -y);
		return cmToPx(100.0f * invertedCoords);
	}

	QPointF positionToScene(b2Vec2 boxCoords) const{
		return positionToScene(boxCoords.x, boxCoords.y);
	}

	inline float32 pxToM(float32 px){
		return pxToCm(px) / 100.0f;
	}

	inline float32 mToPx(float32 m){
		return cmToPx(100.0f * m);
	}

	void createTestBox(){
		b2BodyDef bodyDef;
		bodyDef.position = b2Vec2(0.5f, 0.5f);
		bodyDef.angle = 0;
		bodyDef.type = b2_dynamicBody;

		b2Body *mBody = mWorld->CreateBody(&bodyDef);

		b2FixtureDef robotFixture;
		b2PolygonShape polygonShape;
		polygonShape.SetAsBox( 0.2 / 2, 0.2 / 2 );

		robotFixture.shape = &polygonShape;
		robotFixture.density = 1.0f;
		robotFixture.friction = 1.0f;

		mBody->CreateFixture(&robotFixture);
		mBody->SetUserData( nullptr );

		b2AABB aabb;
		mBody->GetFixtureList()->GetShape()->ComputeAABB(&aabb, b2Transform(b2Vec2(0.5f, 0.5f), b2Rot(0)), 0);
		//minus po y ne nuzhen?? vezde!!
		robotItem = new QGraphicsRectItem(mToPx(aabb.lowerBound.x), mToPx(aabb.lowerBound.y)
										  , mToPx(aabb.upperBound.x - aabb.lowerBound.x)
										  , mToPx(aabb.upperBound.y - aabb.lowerBound.y));
		robotItem->setBrush(QBrush(Qt::cyan));
	}

	QGraphicsRectItem *robotItem = nullptr;
	QGraphicsRectItem *wheel1Item = nullptr;
	QGraphicsRectItem *wheel2Item = nullptr;
};

}
}
}
