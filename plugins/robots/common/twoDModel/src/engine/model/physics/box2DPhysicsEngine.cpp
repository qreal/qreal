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

#include "box2DPhysicsEngine.h"

#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2Fixture.h>
//#include <Box2D/Dynamics/Joints/b2WeldJoint.h>
#include <Box2D/Dynamics/Joints/b2DistanceJoint.h>
#include <Box2D/Collision/b2Collision.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Box2D/Collision/Shapes/b2CircleShape.h>

#include "twoDModel/engine/model/worldModel.h"
#include "src/engine/items/wallItem.h"

using namespace twoDModel::model::physics;

Box2DPhysicsEngine::Box2DPhysicsEngine(const WorldModel &worldModel, const QList<RobotModel *> robots)
	: PhysicsEngineBase(worldModel, robots)
	, mPixelsInCm(worldModel.pixelsInCm())
	, mWorld(new b2World(b2Vec2()))
{
}

Box2DPhysicsEngine::~Box2DPhysicsEngine()
{
	for (b2Body *body : mRobotBodies.values() + mWallBodies.values()) {
		mWorld->DestroyBody(body);
	}

	mRobotBodies.clear();
	mWallBodies.clear();
}

QVector2D Box2DPhysicsEngine::positionShift(twoDModel::model::RobotModel &robot) const
{
	if (!mRobotBodies.contains(&robot)) {
		return QVector2D();
	}

	return QVector2D(cmToPx(mRobotBodies[&robot]->GetPosition().x - pxToCm(robot.position().x()))
			, cmToPx(mRobotBodies[&robot]->GetPosition().y - pxToCm(robot.position().y())));
}

qreal Box2DPhysicsEngine::rotation(twoDModel::model::RobotModel &robot) const
{
	if (!mRobotBodies.contains(&robot)) {
		return 0;
	}

	return mRobotBodies[&robot]->GetAngle() - robot.rotation();
}

QGraphicsRectItem *robotItem = nullptr;
QGraphicsRectItem *wheelItem = nullptr;
QGraphicsRectItem *wheel1Item = nullptr;
QGraphicsRectItem *wheel2Item = nullptr;
void Box2DPhysicsEngine::addRobot(twoDModel::model::RobotModel * const robot)
{
	PhysicsEngineBase::addRobot(robot);

	b2BodyDef robotDef;
	robotDef.type = b2_dynamicBody;
	robotDef.position = pxToCm(robot->position());
	robotDef.angle = robot->rotation();

	b2PolygonShape robotBox;
	const float32 robotWidthCm = pxToCm(robot->info().size().width());
	const float32 robotHeightCm = pxToCm(robot->info().size().height());
	robotBox.SetAsBox(robotHeightCm / 2, robotWidthCm / 2);

	b2FixtureDef robotFixture;
	robotFixture.shape = &robotBox;
	robotFixture.density = robot->info().mass() / robotHeightCm * robotWidthCm;
	robotFixture.friction = robot->info().friction();

	b2Body * const robotBody = mWorld->CreateBody(&robotDef);
	robotBody->CreateFixture(&robotFixture);
	mRobotBodies[robot] = robotBody;

	robotItem = new QGraphicsRectItem(0, 0, cmToPx(robotWidthCm), cmToPx(robotHeightCm));
	/// @todo: correct coordinates!
	const qreal halfWidth = robot->info().size().width() / 2;
	const qreal halfHeight = robot->info().size().height() / 2;
	mLeftWheels[robot] = wheel(robot->position() + QPointF(-halfWidth + 5, -halfHeight + 5), robotBody);
	wheel1Item = wheelItem;
	mRightWheels[robot] = wheel(robot->position() + QPointF(-halfWidth + 5, halfHeight - 5), robotBody);
	wheel2Item = wheelItem;

	const b2Vec2 forwardNormal = mLeftWheels[robot]->GetWorldVector(b2Vec2(1, 0));
	mLeftWheels[robot]->ApplyForce(100 * forwardNormal, mLeftWheels[robot]->GetWorldCenter(), true);
	mRightWheels[robot]->ApplyForce(100 * forwardNormal, mRightWheels[robot]->GetWorldCenter(), true);

	QTimer::singleShot(10, [=]() {
		QGraphicsScene *scene = robot->startPositionMarker()->scene();
		scene->addItem(robotItem);
		scene->addItem(wheel1Item);
		scene->addItem(wheel2Item);
	});
}

void Box2DPhysicsEngine::removeRobot(twoDModel::model::RobotModel * const robot)
{
	PhysicsEngineBase::removeRobot(robot);
}

void Box2DPhysicsEngine::recalculateParameters(qreal timeInterval)
{
	const int velocityIterations = 1;
	const int positionIterations = 1;
	mWorld->Step(timeInterval, velocityIterations, positionIterations);

	robotItem->setPos(cmToPx(mRobotBodies.first()->GetPosition()));
	robotItem->setRotation(mRobotBodies.first()->GetAngle());
	wheel1Item->setPos(cmToPx(mLeftWheels[mRobots.first()]->GetPosition()));
	wheel1Item->setRotation(mLeftWheels[mRobots.first()]->GetAngle());
	wheel2Item->setPos(cmToPx(mRightWheels[mRobots.first()]->GetPosition()));
	wheel2Item->setRotation(mRightWheels[mRobots.first()]->GetAngle());
}

void Box2DPhysicsEngine::onPixelsInCmChanged(qreal value)
{
	mPixelsInCm = value;
}

void Box2DPhysicsEngine::itemAdded(twoDModel::items::SolidItem * const item)
{
//	if (dynamic_cast<items::WallItem *>(item)) {
//	}
}

void Box2DPhysicsEngine::itemRemoved(twoDModel::items::SolidItem * const item)
{
	if (mWallBodies.contains(item)) {
		mWorld->DestroyBody(mWallBodies[item]);
		mWallBodies.remove(item);
	}
}

float32 Box2DPhysicsEngine::pxToCm(qreal px) const
{
	return static_cast<float32>(px / mPixelsInCm);
}

b2Vec2 Box2DPhysicsEngine::pxToCm(const QPointF &posInPx) const
{
	return b2Vec2(pxToCm(posInPx.x()), pxToCm(posInPx.y()));
}

qreal Box2DPhysicsEngine::cmToPx(float cm) const
{
	return static_cast<qreal>(cm * static_cast<float>(mPixelsInCm));
}

QPointF Box2DPhysicsEngine::cmToPx(const b2Vec2 &posInCm) const
{
	return QPointF(cmToPx(posInCm.x), cmToPx(posInCm.y));
}

#include<QDebug>
b2Body *Box2DPhysicsEngine::wheel(const QPointF &coords, b2Body * const robot) const
{
	/// @todo: customize wheel radius, torque, frequency, damping ratio and (maybe) friction and density.
	const float32 wheelHeightInCm = 6.0f;
	const float32 wheelWidthInCm = 2.0f;
	const float32 wheelFriction = 0.9f;

	const float32 wheelDensity = 1.0f;
	b2PolygonShape wheelRect;
	wheelRect.SetAsBox(wheelHeightInCm / 2, wheelWidthInCm / 2);

	const b2Vec2 coordsInCm = pxToCm(coords);
	b2BodyDef wheelBodyDef;
	wheelBodyDef.type = b2_dynamicBody;
	wheelBodyDef.position = coordsInCm;

	b2FixtureDef wheelFixture;
	wheelFixture.shape = &wheelRect;
	wheelFixture.density = wheelDensity;
	wheelFixture.friction = wheelFriction;

	b2Body * const wheel = mWorld->CreateBody(&wheelBodyDef);
	wheel->CreateFixture(&wheelFixture);

	const b2Vec2 wheelCenter(wheelHeightInCm / 2, wheelWidthInCm / 2);
//	b2WeldJointDef wheelJointDef;
//	wheelJointDef.Initialize(wheel, robot, coordsInCm);
	b2DistanceJointDef wheelJointDef;
	wheelJointDef.Initialize(wheel, robot, coordsInCm, coordsInCm);
	qDebug() << wheelJointDef.localAnchorA.x << wheelJointDef.localAnchorA.y << wheelJointDef.localAnchorB.x << wheelJointDef.localAnchorB.y;

	b2AABB aabb;
	robot->GetFixtureList()->GetShape()->ComputeAABB(&aabb, b2Transform(b2Vec2(0, 0), b2Rot(0)), 0);
	qDebug() << "robot:" << aabb.lowerBound.x << aabb.lowerBound.y << aabb.upperBound.x << aabb.upperBound.y;
	robotItem->setRect(cmToPx(aabb.upperBound.x), cmToPx(aabb.upperBound.y), cmToPx(aabb.lowerBound.x - aabb.upperBound.x), cmToPx(aabb.lowerBound.y - aabb.upperBound.y));
	wheel->GetFixtureList()->GetShape()->ComputeAABB(&aabb, b2Transform(coordsInCm, b2Rot(0)), 0);
	qDebug() << "wheel:" << aabb.lowerBound.x << aabb.lowerBound.y << aabb.upperBound.x << aabb.upperBound.y;
	wheelItem = new QGraphicsRectItem(cmToPx(aabb.upperBound.x), cmToPx(aabb.upperBound.y), cmToPx(aabb.lowerBound.x - aabb.upperBound.x), cmToPx(aabb.lowerBound.y - aabb.upperBound.y));

	mWorld->CreateJoint(&wheelJointDef);
	return wheel;

//	b2WheelJointDef wheelJointDef;
//	wheelJointDef.Initialize(robot, wheel, wheel->GetPosition(), b2Vec2(1.0f, 1.0f));
//	wheelJointDef.motorSpeed = 10.0f;
//	wheelJointDef.maxMotorTorque = 10.0f;
//	wheelJointDef.enableMotor = true;
//	wheelJointDef.frequencyHz = 2.0f;
//	wheelJointDef.dampingRatio = 0.7f;

//	return dynamic_cast<b2WheelJoint *>(mWorld->CreateJoint(&wheelJointDef));
}
