/* Copyright 2017 QReal Research Group, Gleb Zakharov
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

#include "Box2DPhysicsEngineNew.h"

#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2Fixture.h>
#include <Box2D/Collision/b2Collision.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Box2D/Collision/Shapes/b2CircleShape.h>

#include "twoDModel/engine/model/worldModel.h"
#include "src/engine/items/wallItem.h"

using namespace twoDModel::model::physics;
using namespace parts;

static const qreal pi = 3.14159265358979323846;


Box2DPhysicsEngineNew::Box2DPhysicsEngineNew(const WorldModel &worldModel
										   , const QList<RobotModel *> robots)
	: PhysicsEngineBase(worldModel, robots)
	, mPixelsInCm(worldModel.pixelsInCm())
	, mWorld(new b2World(b2Vec2()))
{
}

Box2DPhysicsEngineNew::~Box2DPhysicsEngineNew()
{
//	for (box2DRobot *robot : mRobotBodies.values())
//		delete robot;
//	for (b2Body *body : mWallBodies.values())
//		mWorld->DestroyBody(body);

//	mRobotBodies.clear();
//	mWallBodies.clear();
	if (mRobotBodies.empty())
		return;
	delete robot();
}

QVector2D Box2DPhysicsEngineNew::positionShift(twoDModel::model::RobotModel &robot) const
{

	return QVector2D(cmToPx(mRobotBodies[&robot]->mBody->GetPosition().x - pxToCm(robot.position().x()))
			, cmToPx(mRobotBodies[&robot]->mBody->GetPosition().y - pxToCm(robot.position().y())));
}

qreal Box2DPhysicsEngineNew::rotation(twoDModel::model::RobotModel &robot) const
{
	if (!mRobotBodies.contains(&robot)) {
		return 0;
	}

	return (mRobotBodies[&robot]->mBody->GetAngle()) * (180 / pi) - robot.rotation();
}

QGraphicsRectItem *robotItem = nullptr;
QGraphicsRectItem *wheelItem = nullptr;
QGraphicsRectItem *wheel1Item = nullptr;
QGraphicsRectItem *wheel2Item = nullptr;

#include <QDebug>
void Box2DPhysicsEngineNew::addRobot(twoDModel::model::RobotModel * const robot)
{
	PhysicsEngineBase::addRobot(robot);

	qDebug() << "Physics engine robot created";

	/// @todo: correct coordinates!
	const qreal halfWidth = robot->info().size().width() / 2;
	const qreal halfHeight = robot->info().size().height() / 2;
	mLeftWheels[robot] = wheel(robot->position() + QPointF(-halfWidth + 5, -halfHeight + 5), robot);
	wheel1Item = wheelItem;
	mRightWheels[robot] = wheel(robot->position() + QPointF(-halfWidth + 5, halfHeight - 5), robot);
	wheel2Item = wheelItem;

	qDebug() << "Wheels created";

	robotItem = new QGraphicsRectItem(0, 0, robot->info().size().width()
									  , robot->info().size().height());
	mRobotBodies[robot] = new box2DRobot(mWorld.data()
		, robot
		, *leftWheel()
		, *rightWheel()
		, mPixelsInCm);

	b2AABB aabb;
	qDebug() << "robot box2D created";
	mRobotBodies[robot]->mBody->GetFixtureList()->GetShape()->ComputeAABB(&aabb, b2Transform(b2Vec2(0, 0), b2Rot(0)), 0);
	qDebug() << "robot:" << aabb.lowerBound.x << aabb.lowerBound.y << aabb.upperBound.x << aabb.upperBound.y;
	robotItem->setRect(cmToPx(aabb.upperBound.x), cmToPx(aabb.upperBound.y), cmToPx(aabb.lowerBound.x - aabb.upperBound.x), cmToPx(aabb.lowerBound.y - aabb.upperBound.y));


	QTimer::singleShot(10, [=]() {
		QGraphicsScene *scene = robot->startPositionMarker()->scene();
		scene->addItem(robotItem);
		scene->addItem(wheel1Item);
		scene->addItem(wheel2Item);
	});
}

void Box2DPhysicsEngineNew::removeRobot(twoDModel::model::RobotModel * const robot)
{
	PhysicsEngineBase::removeRobot(robot);
	delete mRobotBodies[robot];
}

void Box2DPhysicsEngineNew::recalculateParameters(qreal timeInterval)
{
	const int velocityIterations = 10;
	const int positionIterations = 8;

	const qreal speed1 = wheelLinearSpeed(*mRobots.first(), mRobots.first()->leftWheel());
	const qreal speed2 = wheelLinearSpeed(*mRobots.first(), mRobots.first()->rightWheel());
	leftWheel()->mBody->ApplyForceToCenter(leftWheel()->mBody->GetWorldVector(b2Vec2(speed1, 0)), true);
	rightWheel()->mBody->ApplyForceToCenter(rightWheel()->mBody->GetWorldVector(b2Vec2(speed2, 0)), true);


	leftWheel()->updateFriction();
	rightWheel()->updateFriction();

	mWorld->Step(timeInterval, velocityIterations, positionIterations);

	robotItem->setPos(cmToPx(robot()->mBody->GetPosition()));
	robotItem->setRotation((robot()->mBody->GetAngle()) * (180 / pi));
	wheel1Item->setPos(cmToPx(leftWheel()->mBody->GetPosition()));
	wheel1Item->setRotation(leftWheel()->mBody->GetAngle() * (180 / pi));
	wheel2Item->setPos(cmToPx(rightWheel()->mBody->GetPosition()));
	wheel2Item->setRotation(rightWheel()->mBody->GetAngle() * (180 / pi));

	qDebug() << robotItem->rotation();
}

void Box2DPhysicsEngineNew::onPixelsInCmChanged(qreal value)
{
	mPixelsInCm = value;
}

void Box2DPhysicsEngineNew::itemAdded(twoDModel::items::SolidItem * const item)
{
//	if (dynamic_cast<items::WallItem *>(item)) {
//	}
}

void Box2DPhysicsEngineNew::itemRemoved(twoDModel::items::SolidItem * const item)
{
//	if (mWallBodies.contains(item)) {
//		mWorld->DestroyBody(mWallBodies[item]);
//		mWallBodies.remove(item);
//	}
}

float32 Box2DPhysicsEngineNew::pxToCm(qreal px) const
{
	return static_cast<float32>(px / mPixelsInCm);
}

b2Vec2 Box2DPhysicsEngineNew::pxToCm(const QPointF &posInPx) const
{
	return b2Vec2(pxToCm(posInPx.x()), pxToCm(posInPx.y()));
}

qreal Box2DPhysicsEngineNew::cmToPx(float cm) const
{
	return static_cast<qreal>(cm * static_cast<float>(mPixelsInCm));
}

QPointF Box2DPhysicsEngineNew::cmToPx(const b2Vec2 &posInCm) const
{
	return QPointF(cmToPx(posInCm.x), cmToPx(posInCm.y));
}

#include<QDebug>
box2DWheel *Box2DPhysicsEngineNew::wheel(const QPointF &coords
										 , twoDModel::model::RobotModel * const robot) const
{
	/// @todo: customize wheel radius, torque, frequency, damping ratio and (maybe) friction and density.
	const float32 wheelHeightInCm = 6.0f;
	const float32 wheelWidthInCm = 2.0f;
	const float32 wheelFriction = 0.9f;
	const float32 wheelDensity = 1.0f;
	const b2Vec2 coordsInCm = pxToCm(coords);
	//const b2Vec2 wheelCenter(wheelHeightInCm / 2, wheelWidthInCm / 2);

	box2DWheel *wheel = new box2DWheel(mWorld.data()
		, wheelWidthInCm
		, wheelHeightInCm
		, wheelFriction
		, wheelDensity
		, coordsInCm);

	b2AABB aabb;
	wheel->mBody->GetFixtureList()->GetShape()->ComputeAABB(&aabb, b2Transform(coordsInCm, b2Rot(0)), 0);
	qDebug() << "wheel:" << aabb.lowerBound.x << aabb.lowerBound.y << aabb.upperBound.x << aabb.upperBound.y;
	wheelItem = new QGraphicsRectItem(cmToPx(aabb.upperBound.x), cmToPx(aabb.upperBound.y), cmToPx(aabb.lowerBound.x - aabb.upperBound.x), cmToPx(aabb.lowerBound.y - aabb.upperBound.y));

	return wheel;
}

parts::box2DRobot *Box2DPhysicsEngineNew::robot(){
	return mRobotBodies.first();
}
parts::box2DWheel *Box2DPhysicsEngineNew::leftWheel(){
	return mLeftWheels.first();
}
parts::box2DWheel *Box2DPhysicsEngineNew::rightWheel(){
	return mRightWheels.first();
}


//	b2PolygonShape wheelRect;
//	wheelRect.SetAsBox(wheelHeightInCm / 2, wheelWidthInCm / 2);


//	b2BodyDef wheelBodyDef;
//	wheelBodyDef.type = b2_dynamicBody;
//	wheelBodyDef.position = coordsInCm;

//	b2FixtureDef wheelFixture;
//	wheelFixture.shape = &wheelRect;
//	wheelFixture.density = wheelDensity;
//	wheelFixture.friction = wheelFriction;

//	b2Body * const wheel = mWorld->CreateBody(&wheelBodyDef);
//	wheel->CreateFixture(&wheelFixture);


//	b2WeldJointDef wheelJointDef;
//	wheelJointDef.Initialize(wheel, robot, coordsInCm);
//	b2DistanceJointDef wheelJointDef;
//	wheelJointDef.Initialize(wheel, robot, coordsInCm, coordsInCm);
//	qDebug() << wheelJointDef.localAnchorA.x << wheelJointDef.localAnchorA.y << wheelJointDef.localAnchorB.x << wheelJointDef.localAnchorB.y;

//	b2AABB aabb;
//	robot->GetFixtureList()->GetShape()->ComputeAABB(&aabb, b2Transform(b2Vec2(0, 0), b2Rot(0)), 0);
//	qDebug() << "robot:" << aabb.lowerBound.x << aabb.lowerBound.y << aabb.upperBound.x << aabb.upperBound.y;
//	robotItem->setRect(cmToPx(aabb.upperBound.x), cmToPx(aabb.upperBound.y), cmToPx(aabb.lowerBound.x - aabb.upperBound.x), cmToPx(aabb.lowerBound.y - aabb.upperBound.y));
//	wheel->GetFixtureList()->GetShape()->ComputeAABB(&aabb, b2Transform(coordsInCm, b2Rot(0)), 0);
//	qDebug() << "wheel:" << aabb.lowerBound.x << aabb.lowerBound.y << aabb.upperBound.x << aabb.upperBound.y;
//	wheelItem = new QGraphicsRectItem(cmToPx(aabb.upperBound.x), cmToPx(aabb.upperBound.y), cmToPx(aabb.lowerBound.x - aabb.upperBound.x), cmToPx(aabb.lowerBound.y - aabb.upperBound.y));

//	mWorld->CreateJoint(&wheelJointDef);
//	b2WheelJointDef wheelJointDef;
//	wheelJointDef.Initialize(robot, wheel, wheel->GetPosition(), b2Vec2(1.0f, 1.0f));
//	wheelJointDef.motorSpeed = 10.0f;
//	wheelJointDef.maxMotorTorque = 10.0f;
//	wheelJointDef.enableMotor = true;
//	wheelJointDef.frequencyHz = 2.0f;
//	wheelJointDef.dampingRatio = 0.7f;

//	return dynamic_cast<b2WheelJoint *>(mWorld->CreateJoint(&wheelJointDef));
