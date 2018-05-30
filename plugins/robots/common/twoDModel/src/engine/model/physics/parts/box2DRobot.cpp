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
#include "box2DRobot.h"

#include <Box2D/Box2D.h>
#include <QDebug>

#include "src/engine/model/physics/box2DPhysicsEngine.h"
#include "src/engine/view/scene/sonarSensorItem.h"
#include "twoDModel/engine/model/robotModel.h"
#include "twoDModel/engine/model/constants.h"
#include "box2DWheel.h"
#include "box2DItem.h"

using namespace twoDModel::model::physics;
using namespace parts;

Box2DRobot::Box2DRobot(Box2DPhysicsEngine *engine, twoDModel::model::RobotModel * const robotModel
		, b2Vec2 pos, float angle)
	: mModel(robotModel)
	, mEngine(engine)
	, mWorld(engine->box2DWorld())
{
	b2BodyDef bodyDef;
	bodyDef.position = pos;
	bodyDef.angle = angle;
	bodyDef.type = b2_dynamicBody;
	mBody = mWorld.CreateBody(&bodyDef);

	b2FixtureDef robotFixture;
	b2PolygonShape polygonShape;
	QPolygonF collidingPolygon = mModel->info().collidingPolygon();
	QPointF localCenter = collidingPolygon.boundingRect().center();
	mPolygon = new b2Vec2[collidingPolygon.size()];
	for (int i = 0; i < collidingPolygon.size(); ++i) {
		mPolygon[i] = engine->positionToBox2D(collidingPolygon.at(i) - localCenter);
	}

	polygonShape.Set(mPolygon, collidingPolygon.size());
	robotFixture.shape = &polygonShape;
	robotFixture.density = engine->computeDensity(collidingPolygon, mModel->info().mass());
	robotFixture.friction = mModel->info().friction();
	robotFixture.restitution = 0.6;

	mBody->CreateFixture(&robotFixture);
	mBody->SetUserData(this);
	mBody->SetAngularDamping(1.0f);
	mBody->SetLinearDamping(1.0f);
	connectWheels();

	for (int i = 0; i < polygonShape.GetVertexCount(); ++i) {
		mDebuggingDrawPolygon.append(engine->positionToScene(polygonShape.GetVertex(i) + mBody->GetPosition()));
	}

	if (!mDebuggingDrawPolygon.isEmpty() & !mDebuggingDrawPolygon.isClosed()) {
		mDebuggingDrawPolygon.append(mDebuggingDrawPolygon.first());
	}
}

Box2DRobot::~Box2DRobot() {
	for (auto i = mBody->GetJointList(); i; i = i->next) {
		mWorld.DestroyJoint(i->joint);
	}

	for (auto wheel : mWheels) {
		delete wheel;
	}

	for (auto sensor : mSensors) {
		delete sensor;
	}

	mWorld.DestroyBody(mBody);
	delete[] mPolygon;
}

void Box2DRobot::stop()
{
	if (mIsStopping){
		mBody->SetLinearVelocity(b2Vec2(0, 0));
		mBody->SetAngularVelocity(0);
	}
}

void Box2DRobot::startStopping()
{
	mIsStopping = true;
}

void Box2DRobot::finishStopping()
{
	mIsStopping = false;
}

bool Box2DRobot::isStopping()
{
	return mIsStopping;
}

void Box2DRobot::addSensor(twoDModel::view::SensorItem *sensor)
{
	QPolygonF collidingPolygon = sensor->collidingPolygon();
	QPointF localCenter = collidingPolygon.boundingRect().center();
	b2Vec2 pos = mEngine->positionToBox2D(sensor->scenePos() - localCenter);
	float32 angle = mEngine->angleToBox2D(sensor->rotation());
	mSensors[sensor] = new Box2DItem(mEngine, *sensor, pos, angle);
	reinit();
}

void Box2DRobot::removeSensor(twoDModel::view::SensorItem *sensor)
{
	delete mSensors[sensor];
	mSensors.remove(sensor);
	reinit();
}

void Box2DRobot::reinit()
{
	mJoints.clear();
	for (auto i = mBody->GetJointList(); i; i = i->next) {
		mWorld.DestroyJoint(i->joint);
	}

	for (auto wheel : mWheels) {
		connectWheel(*wheel);
	}

	for (twoDModel::view::SensorItem *sensor : mSensors.keys()) {
		const b2Vec2 pos = mEngine->positionToBox2D(
				sensor->scenePos() - sensor->collidingPolygon().boundingRect().center());
		mSensors[sensor]->moveToPosition(pos);
		connectSensor(*mSensors[sensor]);
	}
}

void Box2DRobot::applyForceToCenter(const b2Vec2 &force, bool wake)
{
	mBody->ApplyForceToCenter(force, wake);
}

b2Body *Box2DRobot::getBody()
{
	return mBody;
}

twoDModel::model::RobotModel *Box2DRobot::getRobotModel() const
{
	return mModel;
}

Box2DWheel *Box2DRobot::getWheelAt(int i) const
{
	return mWheels.at(i);
}

void Box2DRobot::connectWheels() {
	QPointF leftUpCorner = QPointF(-mModel->info().size().width() / 2, -mModel->info().size().height() / 2);

	///@todo: adapt it for more than 2 wheels
	QPointF posLeftWheelFromRobot = mModel->info().wheelsPosition().first();
	QPointF posRightWheelFromRobot = mModel->info().wheelsPosition().last();

	b2Vec2 posLeftWheel = mBody->GetWorldPoint(mEngine->positionToBox2D(posLeftWheelFromRobot + leftUpCorner));
	b2Vec2 posRightWheel = mBody->GetWorldPoint(mEngine->positionToBox2D(posRightWheelFromRobot + leftUpCorner));

	Box2DWheel *leftWheel = new Box2DWheel(mEngine, posLeftWheel, mBody->GetAngle(), *this);
	Box2DWheel *rightWheel = new Box2DWheel(mEngine, posRightWheel, mBody->GetAngle(), *this);
	mWheels.push_back(leftWheel);
	mWheels.push_back(rightWheel);
	connectWheel(*leftWheel);
	connectWheel(*rightWheel);
}

void Box2DRobot::connectWheel(Box2DWheel &wheel){
	b2RevoluteJointDef revDef;
	revDef.bodyA = wheel.getBody();
	revDef.bodyB = mBody;
	revDef.collideConnected = false;

	revDef.localAnchorA = wheel.getBody()->GetLocalCenter();
	revDef.localAnchorB = mBody->GetLocalPoint(wheel.getBody()->GetWorldCenter());

	revDef.referenceAngle = 0;
	revDef.enableLimit = true;
	revDef.lowerAngle = 0;
	revDef.upperAngle = 0;

	b2Joint *joint = mWorld.CreateJoint(&revDef);
	mJoints.push_back(joint);
}

void Box2DRobot::connectSensor(const Box2DItem &sensor)
{
	b2WeldJointDef jointDef;
	jointDef.bodyA = mBody;
	jointDef.bodyB = sensor.getBody();
	jointDef.referenceAngle = sensor.getBody()->GetAngle() - mBody->GetAngle();

	jointDef.localAnchorA = mBody->GetLocalCenter();
	jointDef.localAnchorB = sensor.getBody()->GetLocalPoint(mBody->GetWorldCenter());

	auto joint = mWorld.CreateJoint(&jointDef);
	mJoints.append(joint);
}
