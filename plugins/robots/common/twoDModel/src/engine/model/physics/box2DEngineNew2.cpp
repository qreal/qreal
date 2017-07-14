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
#include "box2DEngineNew2.h"
#include <QDebug>

#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2Fixture.h>
#include <Box2D/Collision/b2Collision.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Box2D/Collision/Shapes/b2CircleShape.h>
#include <Box2D/Dynamics/Joints/b2RevoluteJoint.h>

#include "twoDModel/engine/model/robotModel.h"
#include "twoDModel/engine/model/constants.h"
#include "twoDModel/engine/model/worldModel.h"
#include "src/engine/items/wallItem.h"
#include "qrutils/mathUtils/math.h"

using namespace twoDModel::model::physics;
using namespace parts;
using namespace mathUtils;

box2DEngineNew2::box2DEngineNew2 (const WorldModel &worldModel
		, const QList<RobotModel *> robots)
	: PhysicsEngineBase(worldModel, robots)
	, mPixelsInCm(worldModel.pixelsInCm())
	, mWorld(new b2World(b2Vec2(0, 0)))
	, prevPosition(b2Vec2(0, 0))
	, prevAngle(0)
	, mass(0)
{
}

box2DEngineNew2::~box2DEngineNew2(){
	for (b2Body *robot : mRobotBodies.values())
		mWorld->DestroyBody(robot);
	for (b2Body *lw : mLeftWheels.values())
		mWorld->DestroyBody(lw);
	for (b2Body *rw : mRightWheels.values())
		mWorld->DestroyBody(rw);
	for (b2Body *body : mWallBodies.values())
		mWorld->DestroyBody(body);

	mRobotBodies.clear();
	mRightWheels.clear();
	mLeftWheels.clear();
	mWallBodies.clear();
}

QVector2D box2DEngineNew2::positionShift(twoDModel::model::RobotModel &robot) const
{
	if (!mRobotBodies.contains(&robot)) {
		return QVector2D();
	}
	return QVector2D(positionToScene(mRobotBodies[&robot]->GetPosition() - prevPosition));
}

qreal box2DEngineNew2::rotation(twoDModel::model::RobotModel &robot) const
{
	if (!mRobotBodies.contains(&robot)) {
		return 0;
	}
	return -(mRobotBodies[&robot]->GetAngle() - prevAngle) * 180 / pi;
}

#include <QDebug>
void box2DEngineNew2::addRobot(twoDModel::model::RobotModel * const robot)
{
	PhysicsEngineBase::addRobot(robot);

	const float32 wheelHeightM = pxToM(twoDModel::robotWheelDiameterInPx / 2);
	const float32 wheelWidthM = pxToM(twoDModel::robotWheelDiameterInPx);
	const float32 wheelFriction = 1.0f;
	const float32 wheelDensity = 1.0f;

	b2Vec2 posRobot = positionToBox2D(robot->rotationCenter());
	b2Vec2 posLeftWheel = positionToBox2D(robot->position() + QPointF(0, 5)) + b2Vec2(wheelWidthM / 2, 0);
	b2Vec2 posRightWheel = positionToBox2D(robot->position() + QPointF(0, robot->info().size().width() - 5)) + b2Vec2(wheelWidthM / 2, 0);

	float32 widthRobotM =  pxToM(robot->info().size().width());
	float32 heightRobotM = pxToM(robot->info().size().height());

	//robot
	{
		b2BodyDef bodyDef;
		bodyDef.position = posRobot;
		bodyDef.angle = -robot->rotation() * pi / 180;
		bodyDef.type = b2_dynamicBody;

		b2Body *mBody = mWorld->CreateBody(&bodyDef);

		b2FixtureDef robotFixture;
		b2PolygonShape polygonShape;
		polygonShape.SetAsBox( widthRobotM / 2, heightRobotM / 2 );

		robotFixture.shape = &polygonShape;
		robotFixture.density = 0.01f;
		robotFixture.friction = robot->info().friction();

		mBody->CreateFixture(&robotFixture);
		mBody->SetUserData( robot );

		b2AABB aabb;
		mBody->GetFixtureList()->GetShape()->ComputeAABB(&aabb, b2Transform(posRobot, b2Rot(0)), 0);

		robotItem = new QGraphicsRectItem(-25, -25
										  , mToPx(aabb.upperBound.x - aabb.lowerBound.x)
										  , mToPx(aabb.upperBound.y - aabb.lowerBound.y));
		robotItem->setBrush(QBrush(Qt::red));

		mRobotBodies[robot] = mBody;

		prevPosition = posRobot;
		prevAngle = bodyDef.angle;
	}

	//left wheel
	{
		//не хватает rotation
		box2DWheel *lWheel = new box2DWheel(mWorld.data()
			, wheelHeightM
			, wheelWidthM
			, wheelFriction
			, wheelDensity
			, posLeftWheel
			, mRobotBodies[robot]);

		b2AABB aabb;
		lWheel->mBody->GetFixtureList()->GetShape()->ComputeAABB(&aabb, b2Transform(posLeftWheel, b2Rot(0)), 0);
		qDebug() << "wheel:" << aabb.lowerBound.x << aabb.lowerBound.y << aabb.upperBound.x << aabb.upperBound.y;

		wheel1Item = new QGraphicsRectItem(-10, -6//mToPx(aabb.lowerBound.x), mToPx(aabb.lowerBound.y)
										  , mToPx(aabb.upperBound.x - aabb.lowerBound.x)
										  , mToPx(aabb.upperBound.y - aabb.lowerBound.y));
		wheel1Item->setBrush(QBrush(Qt::green));
		mLeftWheels[robot] = lWheel->mBody;

		{ //left joint
			b2RevoluteJointDef revDef;
			revDef.bodyA = lWheel->mBody;
			revDef.bodyB = mRobotBodies[robot];
			revDef.collideConnected = false;

			revDef.localAnchorA = lWheel->mBody->GetLocalCenter();
			revDef.localAnchorB = mRobotBodies[robot]->GetLocalPoint(lWheel->mBody->GetWorldCenter());

			revDef.referenceAngle = 0;
			revDef.enableLimit = true;
			revDef.lowerAngle = 0;
			revDef.upperAngle = 0;

			mWorld->CreateJoint(&revDef);
		}
	}

	//right wheel
	{
		//add rotation
		box2DWheel *rWheel = new box2DWheel(mWorld.data()
			, wheelHeightM
			, wheelWidthM
			, wheelFriction
			, wheelDensity
			, posRightWheel
			, mRobotBodies[robot]);

		b2AABB aabb;
		rWheel->mBody->GetFixtureList()->GetShape()->ComputeAABB(&aabb, b2Transform(posRightWheel, b2Rot(0)), 0);
		wheel2Item = new QGraphicsRectItem(-10, -6//mToPx(aabb.lowerBound.x), mToPx(aabb.lowerBound.y)
										   , mToPx(aabb.upperBound.x - aabb.lowerBound.x)
										   , mToPx(aabb.upperBound.y - aabb.lowerBound.y));
		wheel2Item->setBrush(QBrush(Qt::blue));
		mRightWheels[robot] = rWheel->mBody;

		{ //right joint
			b2RevoluteJointDef revDef;
			revDef.bodyA = rWheel->mBody;
			revDef.bodyB = mRobotBodies[robot];
			revDef.collideConnected = false;

			revDef.localAnchorA = rWheel->mBody->GetLocalCenter();
			revDef.localAnchorB = mRobotBodies[robot]->GetLocalPoint(rWheel->mBody->GetWorldCenter());

			revDef.referenceAngle = 0;
			revDef.enableLimit = true;
			revDef.lowerAngle = 0;
			revDef.upperAngle = 0;

			mWorld->CreateJoint(&revDef);
		}

	}

	mass = mRobotBodies[robot]->GetMass() + 2 * mLeftWheels[robot]->GetMass();
	qDebug() << "robot mass" << mRobotBodies[robot]->GetMass();
	qDebug() << "lWheel mass" << mLeftWheels[robot]->GetMass();
	qDebug() << "robot mass, встроенная" << robot->info().mass();

	//add bounding rect for test
	QTimer::singleShot(10, [=]() {
		QGraphicsScene *scene = robot->startPositionMarker()->scene();
		scene->addItem(robotItem);
		scene->addItem(wheel1Item);
		scene->addItem(wheel2Item);
	});
}

void box2DEngineNew2::removeRobot(twoDModel::model::RobotModel * const robot)
{
	PhysicsEngineBase::removeRobot(robot);
	mWorld->DestroyBody(mRobotBodies[robot]);
	mWorld->DestroyBody(mLeftWheels[robot]);
	mWorld->DestroyBody(mRightWheels[robot]);
}

bool isStar = true;
int test = 0;

void box2DEngineNew2::recalculateParameters(qreal timeInterval)
{	
	const int velocityIterations = 10;
	const int positionIterations = 8;

	twoDModel::model::RobotModel *robot = mRobots.first();
	b2Body *rBody = mRobotBodies[robot];

	prevPosition = rBody->GetPosition();
	prevAngle = rBody->GetAngle();

	const qreal speed1 = pxToCm(wheelLinearSpeed(*robot, robot->leftWheel()));
	const qreal speed2 = pxToCm(wheelLinearSpeed(*robot, robot->rightWheel()));

	((box2DWheel*)(mLeftWheels[mRobots.first()]->GetUserData()))->keepConstantSpeed(speed1);
	((box2DWheel*)(mRightWheels[mRobots.first()]->GetUserData()))->keepConstantSpeed(speed2);

	mWorld->Step(timeInterval / 1000.0f, velocityIterations, positionIterations);

	robotItem->setPos(positionToScene(rBody->GetPosition()));
	robotItem->setRotation(-rBody->GetAngle() * 180 / pi);
	wheel1Item->setPos(positionToScene(mLeftWheels[robot]->GetPosition()));
	wheel1Item->setRotation(-mLeftWheels[robot]->GetAngle() * 180 / pi);
	wheel2Item->setPos(positionToScene(mRightWheels[robot]->GetPosition()));
	wheel2Item->setRotation(-mRightWheels[robot]->GetAngle() * 180 / pi);
}

void box2DEngineNew2::onPixelsInCmChanged(qreal value)
{
	mPixelsInCm = value;
}

void box2DEngineNew2::itemAdded(twoDModel::items::SolidItem * const item)
{
//	if (dynamic_cast<items::WallItem *>(item)) {
//	}
}

void box2DEngineNew2::itemRemoved(twoDModel::items::SolidItem * const item)
{
//	if (mWallBodies.contains(item)) {
//		mWorld->DestroyBody(mWallBodies[item]);
//		mWallBodies.remove(item);
//	}
}

float32 box2DEngineNew2::pxToCm(qreal px) const
{
	return static_cast<float32>(px / mPixelsInCm);
}

b2Vec2 box2DEngineNew2::pxToCm(const QPointF posInPx) const
{
	return b2Vec2(pxToCm(posInPx.x()), pxToCm(posInPx.y()));
}

qreal box2DEngineNew2::cmToPx(float cm) const
{
	return static_cast<qreal>(cm * static_cast<float>(mPixelsInCm));
}

QPointF box2DEngineNew2::cmToPx(const b2Vec2 posInCm) const
{
	return QPointF(cmToPx(posInCm.x), cmToPx(posInCm.y));
}
