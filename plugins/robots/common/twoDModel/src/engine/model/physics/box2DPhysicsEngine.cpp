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
#include "box2DPhysicsEngine.h"
#include <QDebug>
#include <math.h>

#include <Box2D/Box2D.h>

#include "twoDModel/engine/model/robotModel.h"
#include "twoDModel/engine/model/constants.h"
#include "twoDModel/engine/model/worldModel.h"
#include "src/engine/items/wallItem.h"
#include "qrutils/mathUtils/math.h"
#include "parts/box2DWheel.h"
#include "parts/box2DRobot.h"


using namespace twoDModel::model::physics;
using namespace parts;
using namespace mathUtils;

box2DPhysicsEngine::box2DPhysicsEngine (const WorldModel &worldModel
		, const QList<RobotModel *> robots)
	: PhysicsEngineBase(worldModel, robots)
	, mPixelsInCm(worldModel.pixelsInCm())
	, mWorld(new b2World(b2Vec2(0, 0)))
	, mPrevPosition(b2Vec2(0, 0))
	, mPrevAngle(0)
{
	connect(&worldModel, &model::WorldModel::wallAdded, this, &itemAdded);
	connect(&worldModel, &model::WorldModel::itemRemoved, this, &itemRemoved);
}

box2DPhysicsEngine::~box2DPhysicsEngine(){
	for (box2DRobot *robot : mBox2DRobots.values())
		delete robot;
	mBox2DRobots.clear();
	mRightWheels.clear();
	mLeftWheels.clear();
	mWallBodies.clear();
}

QVector2D box2DPhysicsEngine::positionShift(twoDModel::model::RobotModel &robot) const
{
	if (!mBox2DRobots.contains(&robot))
		return QVector2D();
	return QVector2D(positionToScene(mBox2DRobots[&robot]->body->GetPosition() - mPrevPosition));
}

qreal box2DPhysicsEngine::rotation(twoDModel::model::RobotModel &robot) const
{
	if (!mBox2DRobots.contains(&robot))
		return 0;
	return angleToScene(mBox2DRobots[&robot]->body->GetAngle() - mPrevAngle);
}

#include <QDebug>
void box2DPhysicsEngine::addRobot(twoDModel::model::RobotModel * const robot)
{
	PhysicsEngineBase::addRobot(robot);
	box2DRobot *b2Robot = new box2DRobot(this, robot);
	mBox2DRobots[robot] = b2Robot;
	mPrevPosition = b2Robot->body->GetPosition();
	mPrevAngle = b2Robot->body->GetAngle();

	box2DWheel *lWheel = b2Robot->wheels.at(0);
	box2DWheel *rWheel = b2Robot->wheels.at(1);
	mLeftWheels[robot] = lWheel;
	mRightWheels[robot] = rWheel;

	//test
	//robot
	b2AABB aabb;
	b2Robot->body->GetFixtureList()->GetShape()->ComputeAABB(
		&aabb, b2Transform(b2Robot->body->GetPosition(), b2Rot(0)), 0);
	robotItem = new QGraphicsRectItem(-25, -25
		, mToPx(aabb.upperBound.x - aabb.lowerBound.x)
		, mToPx(aabb.upperBound.y - aabb.lowerBound.y));
	robotItem->setBrush(QBrush(Qt::red));

	//leftWheel
	lWheel->body->GetFixtureList()->GetShape()->ComputeAABB(
				&aabb, b2Transform(lWheel->body->GetPosition(), b2Rot(0)), 0);
	wheel1Item = new QGraphicsRectItem(-10, -6
			, mToPx(aabb.upperBound.x - aabb.lowerBound.x)
			, mToPx(aabb.upperBound.y - aabb.lowerBound.y));
	wheel1Item->setBrush(QBrush(Qt::green));

	//rightWheel
	rWheel->body->GetFixtureList()->GetShape()->ComputeAABB(
				&aabb, b2Transform(rWheel->body->GetPosition(), b2Rot(0)), 0);
	wheel2Item = new QGraphicsRectItem(-10, -6
			, mToPx(aabb.upperBound.x - aabb.lowerBound.x)
			, mToPx(aabb.upperBound.y - aabb.lowerBound.y));
	wheel2Item->setBrush(QBrush(Qt::blue));

	QTimer::singleShot(10, [=]() {
		mScene = robot->startPositionMarker()->scene();
		mScene->addItem(robotItem);
		mScene->addItem(wheel1Item);
		mScene->addItem(wheel2Item);
	});
	//
}

void box2DPhysicsEngine::removeRobot(twoDModel::model::RobotModel * const robot)
{
	PhysicsEngineBase::removeRobot(robot);
	delete mBox2DRobots[robot];
	mBox2DRobots.remove(robot);
	mLeftWheels.remove(robot);
	mRightWheels.remove(robot);
}

void box2DPhysicsEngine::recalculateParameters(qreal timeInterval)
{	
	const int velocityIterations = 4;
	const int positionIterations = 4;

	twoDModel::model::RobotModel * const robot = mRobots.first();
	b2Body *rBody = mBox2DRobots[robot]->body;

	mPrevPosition = rBody->GetPosition();
	mPrevAngle = rBody->GetAngle();

	const qreal speed1 = pxToM(wheelLinearSpeed(*robot, robot->leftWheel())) * 1000.0f / timeInterval;
	const qreal speed2 = pxToM(wheelLinearSpeed(*robot, robot->rightWheel())) * 1000.0f / timeInterval;

	mLeftWheels[robot]->keepConstantSpeed(speed1);
	mRightWheels[robot]->keepConstantSpeed(speed2);

	mWorld->Step(timeInterval / 1000.0f, velocityIterations, positionIterations);

	robotItem->setPos(positionToScene(rBody->GetPosition()));
	robotItem->setRotation(angleToScene(rBody->GetAngle()));
	wheel1Item->setPos(positionToScene(mLeftWheels[robot]->body->GetPosition()));
	wheel1Item->setRotation(angleToScene(mLeftWheels[robot]->body->GetAngle()));
	wheel2Item->setPos(positionToScene(mRightWheels[robot]->body->GetPosition()));
	wheel2Item->setRotation(angleToScene(mRightWheels[robot]->body->GetAngle()));
}

void box2DPhysicsEngine::onPixelsInCmChanged(qreal value)
{
	mPixelsInCm = value;
}

void box2DPhysicsEngine::itemAdded(twoDModel::items::SolidItem * const item)
{
	if (dynamic_cast<items::WallItem *>(item)) {
		items::WallItem *wallItem = dynamic_cast<items::WallItem *>(item);
		QPointF beg = wallItem->begin();
		QPointF end = wallItem->end();
		QVector2D wall = QVector2D(end - beg);
		float32 length = wall.length();
		float32 cosWall = QVector2D::dotProduct(wall, QVector2D(1, 0)) / length;
		float32 wallAngle = Math::eq(length, 0, EPS) ? 0 : acos(cosWall);
		wallAngle *= end.y() > beg.y() ? -1 : 1;

		b2BodyDef wallDef;
		b2Vec2 pos = positionToBox2D(wallItem->boundingRect().center());
		wallDef.position = pos;
		wallDef.angle = wallAngle;
		wallDef.type = b2_staticBody;

		b2Body *body = mWorld->CreateBody(&wallDef);
		b2FixtureDef fixture;
		b2PolygonShape wallShape;
		QPointF wallSize = QPointF(length + wallItem->width(), wallItem->width());

		wallShape.SetAsBox(pxToM(wallSize.x()) / 2, pxToM(wallSize.y()) / 2);
		fixture.shape = &wallShape;
		fixture.density = 1.0f;
		body->CreateFixture(&fixture);
		body->SetUserData(wallItem);

		QGraphicsRectItem *wallRect = new QGraphicsRectItem(-wallSize.x() / 2, -wallSize.y() / 2
				, wallSize.x() + 10, wallSize.y() + 10);
		wallRect->setBrush(QBrush(Qt::black));
		mScene->addItem(wallRect);
		wallRect->setPos(positionToScene(body->GetPosition()));
		wallRect->setRotation(angleToScene(body->GetAngle()));
	}
}

void box2DPhysicsEngine::itemRemoved(QGraphicsItem * const item)
{
	if (mWallBodies.contains(item)) {
		mWorld->DestroyBody(mWallBodies[item]);
		mWallBodies.remove(item);
	}
}

float32 box2DPhysicsEngine::pxToCm(qreal px) const
{
	return static_cast<float32>(px / mPixelsInCm);
}

b2Vec2 box2DPhysicsEngine::pxToCm(const QPointF posInPx) const
{
	return b2Vec2(pxToCm(posInPx.x()), pxToCm(posInPx.y()));
}

qreal box2DPhysicsEngine::cmToPx(float cm) const
{
	return static_cast<qreal>(cm * static_cast<float>(mPixelsInCm));
}

QPointF box2DPhysicsEngine::cmToPx(const b2Vec2 posInCm) const
{
	return QPointF(cmToPx(posInCm.x), cmToPx(posInCm.y));
}

b2Vec2 box2DPhysicsEngine::positionToBox2D(QPointF sceneCoords) const
{
	return positionToBox2D(sceneCoords.x(), sceneCoords.y());
}

b2Vec2 box2DPhysicsEngine::positionToBox2D(float32 x, float32 y) const
{
	QPointF invertedCoords = QPointF(x, -y);
	return 0.01f * pxToCm(invertedCoords);
}

QPointF box2DPhysicsEngine::positionToScene(b2Vec2 boxCoords) const
{
	return positionToScene(boxCoords.x, boxCoords.y);
}

QPointF box2DPhysicsEngine::positionToScene(float32 x, float32 y) const
{
	b2Vec2 invertedCoords = b2Vec2(x, -y);
	return cmToPx(100.0f * invertedCoords);
}

float32 box2DPhysicsEngine::angleToBox2D(qreal sceneAngle) const
{
	return -sceneAngle * mathUtils::pi / 180;
}

qreal box2DPhysicsEngine::angleToScene(float32 boxAngle) const
{
	return -boxAngle / mathUtils::pi * 180;
}

float32 box2DPhysicsEngine::pxToM(qreal px) const
{
	return pxToCm(px) / 100.0f;
}

qreal box2DPhysicsEngine::mToPx(float32 m) const
{
	return cmToPx(100.0f * m);
}
