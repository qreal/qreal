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
#include "box2DPhysicsEngine.h"

#include <QDebug>
#include <Box2D/Box2D.h>

#include <qrutils/graphicsUtils/abstractItem.h>

#include "twoDModel/engine/model/robotModel.h"
#include "twoDModel/engine/model/constants.h"
#include "twoDModel/engine/model/worldModel.h"
#include "src/engine/view/scene/twoDModelScene.h"
#include "src/engine/view/scene/robotItem.h"
#include "src/engine/view/scene/sensorItem.h"
#include "src/engine/items/wallItem.h"
#include "src/engine/items/skittleItem.h"
#include "qrutils/mathUtils/math.h"
#include "parts/box2DWheel.h"
#include "parts/box2DRobot.h"
#include "parts/box2DItem.h"


using namespace twoDModel::model::physics;
using namespace parts;
using namespace mathUtils;

const qreal scaleCoeff = 0.001;

box2DPhysicsEngine::box2DPhysicsEngine (const WorldModel &worldModel
		, const QList<RobotModel *> robots)
	: PhysicsEngineBase(worldModel, robots)
	, mPixelsInCm(worldModel.pixelsInCm() * scaleCoeff)
	, mWorld(new b2World(b2Vec2(0, 0)))
	, mPrevPosition(b2Vec2(0, 0))
	, mPrevAngle(0)
{
	connect(&worldModel, &model::WorldModel::wallAdded, this, &box2DPhysicsEngine::itemAdded);
	connect(&worldModel, &model::WorldModel::skittleAdded, this, &box2DPhysicsEngine::itemAdded);
	connect(&worldModel, &model::WorldModel::itemRemoved, this, &box2DPhysicsEngine::itemRemoved);
}

box2DPhysicsEngine::~box2DPhysicsEngine(){
	for (box2DRobot *robot : mBox2DRobots.values()) {
		delete robot;
	}

	mBox2DRobots.clear();
	mRightWheels.clear();
	mLeftWheels.clear();
	mBox2DResizableItems.clear();
	mBox2DDynamicItems.clear();
}

QVector2D box2DPhysicsEngine::positionShift(model::RobotModel &robot) const
{
	if (!mBox2DRobots.contains(&robot))
		return QVector2D();
	return QVector2D(positionToScene(mBox2DRobots[&robot]->body->GetPosition() - mPrevPosition));
}

qreal box2DPhysicsEngine::rotation(model::RobotModel &robot) const
{
	if (!mBox2DRobots.contains(&robot)) {
		return 0;
	}

	return angleToScene(mBox2DRobots[&robot]->body->GetAngle() - mPrevAngle);
}

void box2DPhysicsEngine::createDebugRobot(model::RobotModel* const robot)
{
	//robot
	b2AABB aabb;
	mBox2DRobots[robot]->body->GetFixtureList()->GetShape()->ComputeAABB(
		&aabb, b2Transform(mBox2DRobots[robot]->body->GetPosition(), b2Rot(0)), 0);
	mDebugRobotItem = new QGraphicsRectItem(-25, -25
		, mToPx(aabb.upperBound.x - aabb.lowerBound.x)
		, mToPx(aabb.upperBound.y - aabb.lowerBound.y));
	mDebugRobotItem->setBrush(QBrush(Qt::red));

	//leftWheel
	mLeftWheels[robot]->body->GetFixtureList()->GetShape()->ComputeAABB(
				&aabb, b2Transform(mLeftWheels[robot]->body->GetPosition(), b2Rot(0)), 0);
	mDebugWheel1Item = new QGraphicsRectItem(-10, -6
			, mToPx(aabb.upperBound.x - aabb.lowerBound.x)
			, mToPx(aabb.upperBound.y - aabb.lowerBound.y));
	mDebugWheel1Item->setBrush(QBrush(Qt::green));

	//rightWheel
	mRightWheels[robot]->body->GetFixtureList()->GetShape()->ComputeAABB(
				&aabb, b2Transform(mRightWheels[robot]->body->GetPosition(), b2Rot(0)), 0);
	myDebugWheel2Item = new QGraphicsRectItem(-10, -6
			, mToPx(aabb.upperBound.x - aabb.lowerBound.x)
			, mToPx(aabb.upperBound.y - aabb.lowerBound.y));
	myDebugWheel2Item->setBrush(QBrush(Qt::blue));

	mScene->addItem(mDebugRobotItem);
	mScene->addItem(mDebugWheel1Item);
	mScene->addItem(myDebugWheel2Item);
}

void box2DPhysicsEngine::addRobot(model::RobotModel * const robot)
{
	PhysicsEngineBase::addRobot(robot);
	addRobot(robot, robot->rotationCenter(), robot->rotation());

	mPrevPosition = mBox2DRobots[robot]->body->GetPosition();
	mPrevAngle = mBox2DRobots[robot]->body->GetAngle();

	auto handlePos = [&] (const QPointF &newPos) {
		onRobotStartPositionChanged(newPos, dynamic_cast<model::RobotModel *>(sender()));
	};

	auto handleAngle = [&] (const qreal newAngle){
		onRobotStartAngleChanged(newAngle, dynamic_cast<model::RobotModel *>(sender()));
	};

	connect(robot, &model::RobotModel::positionChanged, this, handlePos);
	connect(robot, &model::RobotModel::rotationChanged, this, handleAngle);

	QTimer::singleShot(10, [=]() {
//		createDebugRobot(robot);
		mScene = dynamic_cast<view::TwoDModelScene *>(robot->startPositionMarker()->scene());
		auto funcRelease = [=]() {
			view::RobotItem *rItem = mScene->robot(*robot);
			if (rItem != nullptr)
				onMouseReleased(rItem->pos(), rItem->rotation());
		};
		connect(mScene->robot(*robot), &view::RobotItem::mouseInteractionStopped, this, funcRelease);
		connect(mScene->robot(*robot), &view::RobotItem::mouseInteractionStarted
				, this, &box2DPhysicsEngine::onMousePressed);
		connect(mScene->robot(*robot), &view::RobotItem::recoverRobotPosition
				, this, &box2DPhysicsEngine::onRecoverRobotPosition);

		connect(mScene->robot(*robot), &view::RobotItem::sensorAdded, this, [&](twoDModel::view::SensorItem *sensor) {
			auto rItem = dynamic_cast<view::RobotItem *>(sender());
			auto model = &rItem->robotModel();
			mRobotSensors[model].insert(sensor);
			mBox2DRobots[model]->addSensor(sensor);
		});
		connect(mScene->robot(*robot), &view::RobotItem::sensorRemoved, this, [&](twoDModel::view::SensorItem *sensor) {
			auto rItem = dynamic_cast<view::RobotItem *>(sender());
			auto model = &rItem->robotModel();
			mRobotSensors[model].remove(sensor);
			mBox2DRobots[model]->removeSensor(sensor);
		});
		connect(mScene->robot(*robot), &view::RobotItem::sensorUpdated
				, this, [&](twoDModel::view::SensorItem *sensor) {
			Q_UNUSED(sensor);
			auto rItem = dynamic_cast<view::RobotItem *>(sender());
			auto model = &rItem->robotModel();
			mBox2DRobots[model]->reinit();
		});

		connect(robot, &model::RobotModel::deserialized, this, &box2DPhysicsEngine::onMouseReleased);
		//add connect to stop robot here
//		drawDebugRobot(robot);
	});
}

void box2DPhysicsEngine::addRobot(model::RobotModel * const robot, QPointF pos, qreal angle)
{
	if (mBox2DRobots.contains(robot)) {
		delete mBox2DRobots[robot];
	}

	mBox2DRobots[robot] = new box2DRobot(this, robot, positionToBox2D(pos), angleToBox2D(angle));
	mLeftWheels[robot] = mBox2DRobots[robot]->wheels.at(0);
	mRightWheels[robot] = mBox2DRobots[robot]->wheels.at(1);

	for (auto sensor : mRobotSensors[robot]) {
		mBox2DRobots[robot]->addSensor(sensor);
	}
}

void box2DPhysicsEngine::onRobotStartPositionChanged(const QPointF &newPos, model::RobotModel *robot)
{
	if (!mBox2DRobots.contains(robot)) {
		return;
	}

	float angle = mBox2DRobots[robot]->body->GetAngle();
	QPointF newPosShift = newPos + QPointF(robot->info().size().width() / 2, robot->info().size().height() / 2);
	addRobot(robot, newPosShift, angleToScene(angle));
}

void box2DPhysicsEngine::onRobotStartAngleChanged(const qreal newAngle, model::RobotModel *robot)
{
	if (!mBox2DRobots.contains(robot)) {
		return;
	}

	b2Vec2 pos = mBox2DRobots[robot]->body->GetPosition();
	addRobot(robot, positionToScene(pos), newAngle);
}

void box2DPhysicsEngine::onMouseReleased(QPointF newPos, qreal newAngle)
{
	box2DRobot *robot = mBox2DRobots.first();

	robot->finishStopping();
	robot->reinit();
	onRobotStartAngleChanged(newAngle, robot->model);
	onRobotStartPositionChanged(newPos, robot->model);
}

void box2DPhysicsEngine::onMousePressed()
{
	for (box2DRobot *robot: mBox2DRobots){
		robot->startStopping();

		QPainterPath p;
		p.addPolygon(robot->mPoly);
		mScene->addPath(p);
		QPainterPath p1;
		p1.addPolygon(robot->wheels[0]->mPoly);
		mScene->addPath(p1);
		QPainterPath p2;
		p2.addPolygon(robot->wheels[1]->mPoly);
		mScene->addPath(p2);
	}
}

void box2DPhysicsEngine::onRecoverRobotPosition(QPointF pos)
{
	onMouseReleased(pos, angleToScene(mBox2DRobots.first()->body->GetAngle()));
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
	const int velocityIterations = 10;
	const int positionIterations = 6;

	model::RobotModel * const robot = mRobots.first();
	if (mBox2DRobots[robot]) {
		b2Body *rBody = mBox2DRobots[robot]->body;
		float32 secondsInterval = timeInterval / 1000.0f;

		if (mBox2DRobots[robot]->isStopping()){
			mBox2DRobots[robot]->stop();
		} else {
			const qreal speed1 = pxToM(wheelLinearSpeed(*robot, robot->leftWheel())) / secondsInterval * 10;
			const qreal speed2 = pxToM(wheelLinearSpeed(*robot, robot->rightWheel())) / secondsInterval * 10;

			mLeftWheels[robot]->keepConstantSpeed(speed1);
			mRightWheels[robot]->keepConstantSpeed(speed2);
		}

		mPrevPosition = rBody->GetPosition();
		mPrevAngle = rBody->GetAngle();

		mWorld->Step(secondsInterval, velocityIterations, positionIterations);

		for(QGraphicsItem *item : mBox2DDynamicItems.keys()) {
			item->setPos(positionToScene(mBox2DDynamicItems[item]->getPosition()));
			item->moveBy(-item->boundingRect().center().x(), -item->boundingRect().center().y());
			item->setRotation(angleToScene(mBox2DDynamicItems[item]->getRotation()));
		}
	}
}

void box2DPhysicsEngine::wakeUp()
{
	for (box2DRobot *robot : mBox2DRobots) {
		onRobotStartAngleChanged(robot->model->rotation(), robot->model);
		onRobotStartPositionChanged(robot->model->position(), robot->model);
	}
}

void box2DPhysicsEngine::onPixelsInCmChanged(qreal value)
{
	mPixelsInCm = value * scaleCoeff;
}

void box2DPhysicsEngine::itemAdded(QGraphicsItem * const item)
{
	if (auto abstractItem = dynamic_cast<graphicsUtils::AbstractItem *>(item)) {
		auto onItemDraggedLambda = [&] { onItemDragged(dynamic_cast<graphicsUtils::AbstractItem *> (sender())); };

		connect(abstractItem, &graphicsUtils::AbstractItem::x1Changed, this, onItemDraggedLambda);
		connect(abstractItem, &graphicsUtils::AbstractItem::x2Changed, this, onItemDraggedLambda);
		connect(abstractItem, &graphicsUtils::AbstractItem::y1Changed, this, onItemDraggedLambda);
		connect(abstractItem, &graphicsUtils::AbstractItem::y2Changed, this, onItemDraggedLambda);
		onItemDragged(abstractItem);
	}
}

void box2DPhysicsEngine::onItemDragged(graphicsUtils::AbstractItem *item)
{
	itemRemoved(item);
	if (auto solidItem = dynamic_cast<items::SolidItem *>(item)) {
		QPolygonF collidingPolygon = solidItem->collidingPolygon();
		if (collidingPolygon.boundingRect().isEmpty() || collidingPolygon.size() < 3) {
			return;
		}

		b2Vec2 pos = positionToBox2D(collidingPolygon.boundingRect().center());
		Box2DItem *box2dItem = new Box2DItem(this, *solidItem, pos, angleToBox2D(item->rotation()));
		mBox2DResizableItems[item] = box2dItem;
		if (solidItem->bodyType() == items::SolidItem::DYNAMIC) {
			mBox2DDynamicItems[item] = box2dItem;
		}
	}
}

void box2DPhysicsEngine::itemRemoved(QGraphicsItem * const item)
{
	if (mBox2DResizableItems.contains(item)) {
		delete mBox2DResizableItems[item];
		mBox2DResizableItems.remove(item);
	}

	if (mBox2DDynamicItems.contains(item)) {
		mBox2DDynamicItems.remove(item);
	}
}

void box2DPhysicsEngine::drawDebugRobot(model::RobotModel* const robot)
{
	mDebugRobotItem->setPos(positionToScene(mBox2DRobots[robot]->body->GetPosition()));
	mDebugRobotItem->setRotation(angleToScene(mBox2DRobots[robot]->body->GetAngle()));
	mDebugWheel1Item->setPos(positionToScene(mLeftWheels[robot]->body->GetPosition()));
	mDebugWheel1Item->setRotation(angleToScene(mLeftWheels[robot]->body->GetAngle()));
	myDebugWheel2Item->setPos(positionToScene(mRightWheels[robot]->body->GetPosition()));
	myDebugWheel2Item->setRotation(angleToScene(mRightWheels[robot]->body->GetAngle()));
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

qreal box2DPhysicsEngine::computeDensity(const QPolygonF &shape, qreal mass)
{
	return mass / pxToM(pxToM(mathUtils::Geometry::square(shape)));
}

qreal box2DPhysicsEngine::computeDensity(qreal radius, qreal mass)
{
	return mass / pxToM(pxToM(mathUtils::pi * radius * radius));
}
