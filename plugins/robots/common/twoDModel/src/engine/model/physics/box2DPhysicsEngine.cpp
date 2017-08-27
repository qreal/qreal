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
#include <Box2D/Box2D.h>

#include "twoDModel/engine/model/robotModel.h"
#include "twoDModel/engine/model/constants.h"
#include "twoDModel/engine/model/worldModel.h"
#include "src/engine/view/scene/twoDModelScene.h"
#include "src/engine/view/scene/robotItem.h"
#include "src/engine/items/wallItem.h"
#include "qrutils/mathUtils/math.h"
#include "parts/box2DWheel.h"
#include "parts/box2DRobot.h"
#include "parts/box2DWall.h"


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
	mBox2DWalls.clear();
}

QVector2D box2DPhysicsEngine::positionShift(model::RobotModel &robot) const
{
	if (!mBox2DRobots.contains(&robot))
		return QVector2D();
	return QVector2D(positionToScene(mBox2DRobots[&robot]->body->GetPosition() - mPrevPosition));
}

qreal box2DPhysicsEngine::rotation(model::RobotModel &robot) const
{
	if (!mBox2DRobots.contains(&robot))
		return 0;
	return angleToScene(mBox2DRobots[&robot]->body->GetAngle() - mPrevAngle);
}

void box2DPhysicsEngine::addRobot(model::RobotModel * const robot)
{
	PhysicsEngineBase::addRobot(robot);
	box2DRobot *b2Robot = new box2DRobot(this, robot, positionToBox2D(robot->rotationCenter())
		, angleToBox2D(robot->rotation()));
	mBox2DRobots[robot] = b2Robot;
	mPrevPosition = b2Robot->body->GetPosition();
	mPrevAngle = b2Robot->body->GetAngle();
	mRobotWasOnGround = true;

	box2DWheel *lWheel = b2Robot->wheels.at(0);
	box2DWheel *rWheel = b2Robot->wheels.at(1);
	mLeftWheels[robot] = lWheel;
	mRightWheels[robot] = rWheel;

	auto funcPos = [&] (const QPointF &newPos) {
		onRobotStartPositionChanged(newPos, dynamic_cast<model::RobotModel *>(sender()));
	};

	auto funcAngle = [&] (const qreal newAngle){
		onRobotStartAngleChanged(newAngle, dynamic_cast<model::RobotModel *>(sender()));
	};

	connect(robot, &model::RobotModel::positionChanged, this, funcPos);
	connect(robot, &model::RobotModel::rotationChanged, this, funcAngle);
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
		mScene = dynamic_cast<view::TwoDModelScene *>(robot->startPositionMarker()->scene());
		auto funcRelease = [=] {
			view::RobotItem *rItem = mScene->robot(*robot);
			if (rItem != nullptr)
				onMouseReleased(rItem->pos(), rItem->rotation());
		};
		connect(mScene, &view::TwoDModelScene::mouseReleased, this, funcRelease);
		//connect(mScene, &view::TwoDModelScene::mousePressed, this, &onMousePressed);
		//connect(mScene->robot(*mBox2DRobots.first()->model), &view::RobotItem::itemChanged, this, &onMouseReleased);

		mScene->addItem(robotItem);
		mScene->addItem(wheel1Item);
		mScene->addItem(wheel2Item);

		drawDebugRobot();
	});
	//
}

void box2DPhysicsEngine::onRobotStartPositionChanged(const QPointF &newPos, model::RobotModel *robot)
{
	b2Body *rb = mBox2DRobots[robot]->body;
	const QPointF oldPos = positionToScene(rb->GetPosition());
	bool oldPosIsZero = firstSetPos;
	firstSetPos = false;
	bool newPosIsZero = Math::eq(newPos.x(), 0) && Math::eq(newPos.y(), 0);
	bool robotOnTheGroundNow = mRobotWasOnGround;
	if (oldPosIsZero || newPosIsZero || robotOnTheGroundNow){
		float angle = mBox2DRobots[robot]->body->GetAngle();
		delete mBox2DRobots[robot];
		QPointF newPosShift = newPos + QPointF(robot->info().size().width() / 2, robot->info().size().height() / 2);
		mBox2DRobots[robot] = new box2DRobot(this, robot, positionToBox2D(newPosShift), angle);
		box2DWheel *lWheel = mBox2DRobots[robot]->wheels.at(0);
		box2DWheel *rWheel = mBox2DRobots[robot]->wheels.at(1);
		mLeftWheels[robot] = lWheel;
		mRightWheels[robot] = rWheel;

		drawDebugRobot();
	}
}

void box2DPhysicsEngine::onRobotStartAngleChanged(const qreal newAngle, model::RobotModel *robot)
{
	b2Body *rb = mBox2DRobots[robot]->body;
	const qreal oldAngle = angleToScene(rb->GetAngle());
	bool oldAngleIsZero = Math::eq(oldAngle, 0);
	bool newAngleIsZero = Math::eq(newAngle, 0);
	bool robotOnTheGroundNow = mRobotWasOnGround;
	if (oldAngleIsZero || newAngleIsZero || robotOnTheGroundNow){
		b2Vec2 pos = mBox2DRobots[robot]->body->GetPosition();
		delete mBox2DRobots[robot];
		mBox2DRobots[robot] = new box2DRobot(this, robot, pos, angleToBox2D(newAngle));
		box2DWheel *lWheel = mBox2DRobots[robot]->wheels.at(0);
		box2DWheel *rWheel = mBox2DRobots[robot]->wheels.at(1);
		mLeftWheels[robot] = lWheel;
		mRightWheels[robot] = rWheel;

		drawDebugRobot();
	}
}

void box2DPhysicsEngine::onMouseReleased(QPointF newPos, qreal newAngle)
{
	mRobotWasOnGround = true;
	box2DRobot *robot = mBox2DRobots.first();
	onRobotStartAngleChanged(newAngle, robot->model);
	onRobotStartPositionChanged(newPos, robot->model);
	mRobotWasOnGround = false;
}

void box2DPhysicsEngine::removeRobot(model::RobotModel * const robot)
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
	const int positionIterations = 3;

	model::RobotModel * const robot = mRobots.first();
	b2Body *rBody = mBox2DRobots[robot]->body;

	mPrevPosition = rBody->GetPosition();
	mPrevAngle = rBody->GetAngle();

	// pixels/milliseconds -> meters/seconds * 10
	const qreal speed1 = pxToM(wheelLinearSpeed(*robot, robot->leftWheel())) * 1000.0f / timeInterval * 10;
	const qreal speed2 = pxToM(wheelLinearSpeed(*robot, robot->rightWheel())) * 1000.0f / timeInterval * 10;

	mLeftWheels[robot]->keepConstantSpeed(speed1);
	mRightWheels[robot]->keepConstantSpeed(speed2);

	mWorld->Step(timeInterval / 1000.0f, velocityIterations, positionIterations);
	drawDebugRobot();
}

void box2DPhysicsEngine::onPixelsInCmChanged(qreal value)
{
	mPixelsInCm = value;
}

void box2DPhysicsEngine::itemAdded(items::SolidItem * const item)
{
	if (dynamic_cast<items::WallItem *>(item)) {
		mCurrentWall = dynamic_cast<items::WallItem *>(item);
		//connect(mCurrentWall, &items::WallItem::wallDragged, this, &onWallDragged);

		auto func = [&] {
			onWallDragged(dynamic_cast<items::WallItem *> (sender()));
		};
		connect(mCurrentWall, &items::WallItem::x1Changed, this, func);
		connect(mCurrentWall, &items::WallItem::x2Changed, this, func);
		connect(mCurrentWall, &items::WallItem::y1Changed, this, func);
		connect(mCurrentWall, &items::WallItem::y2Changed, this, func);
		onWallResize();
	}
}

void box2DPhysicsEngine::onWallResize(){
	if (!mCurrentWall)
		return;
	box2DWall *wall = new box2DWall(this, *mCurrentWall);
	mBox2DWalls[mCurrentWall] = wall;

	//test
	float32 length = QVector2D(mCurrentWall->end() - mCurrentWall->begin()).length();
	QPointF wallSize = QPointF(length + mCurrentWall->width(), mCurrentWall->width());
	QGraphicsRectItem *wallRect = new QGraphicsRectItem(-wallSize.x() / 2, -wallSize.y() / 2
			, wallSize.x() + 10, wallSize.y() + 10);
	wallRect->setBrush(QBrush(Qt::black));
	mScene->addItem(wallRect);
	wallRect->setPos(positionToScene(wall->body->GetPosition()));
	wallRect->setRotation(angleToScene(wall->body->GetAngle()));
	mBlackWallsItems[mBox2DWalls[mCurrentWall]] = wallRect;
	//

	mCurrentWall = nullptr;
}

void box2DPhysicsEngine::onWallDragged(items::WallItem *wall){
	itemRemoved(wall);
	mCurrentWall = wall;
	onWallResize();
}

void box2DPhysicsEngine::itemRemoved(QGraphicsItem * const item)
{
	if (mBox2DWalls.contains(item)) {
		//test
		mScene->removeItem(mBlackWallsItems[mBox2DWalls[item]]);
		//
		delete mBox2DWalls[item];
		mBox2DWalls.remove(item);
	}
}

void box2DPhysicsEngine::drawDebugRobot()
{
	robotItem->setPos(positionToScene(mBox2DRobots.first()->body->GetPosition()));
	robotItem->setRotation(angleToScene(mBox2DRobots.first()->body->GetAngle()));
	wheel1Item->setPos(positionToScene(mLeftWheels.first()->body->GetPosition()));
	wheel1Item->setRotation(angleToScene(mLeftWheels.first()->body->GetAngle()));
	wheel2Item->setPos(positionToScene(mRightWheels.first()->body->GetPosition()));
	wheel2Item->setRotation(angleToScene(mRightWheels.first()->body->GetAngle()));
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
