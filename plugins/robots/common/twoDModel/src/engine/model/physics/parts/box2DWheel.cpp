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

#include "box2DWheel.h"

#include <qrutils/mathUtils/math.h>
#include "twoDModel/engine/model/constants.h"
#include "box2DRobot.h"

#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Dynamics/b2Fixture.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>

using namespace twoDModel::model::physics::parts;

Box2DWheel::Box2DWheel(Box2DPhysicsEngine *engine
		, const b2Vec2 &positionBox2D, const float rotationBox2D, Box2DRobot &robot)
	: prevSpeed(0.0)
	, mRobot(robot)
	, mEngine(engine)
	, wheelHeightM(engine->pxToM(twoDModel::robotWheelDiameterInPx / 2))
	, wheelWidthM(engine->pxToM(twoDModel::robotWheelDiameterInPx))
	, mPolygon(new b2Vec2[8])
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = positionBox2D;
	bodyDef.angle = rotationBox2D;
	mBody = engine->box2DWorld().CreateBody( &bodyDef );

	b2FixtureDef fixtureDef;
	fixtureDef.restitution = 0.5;
	fixtureDef.friction = wheelFriction;
	b2PolygonShape polygonShape;

	b2Vec2 center = b2Vec2(0.5 * wheelWidthM, 0.5 * wheelHeightM);
	mPolygon[0] = b2Vec2(0.2 * wheelWidthM, wheelHeightM) - center;
	mPolygon[1] = b2Vec2(0.8 * wheelWidthM, wheelHeightM) - center;
	mPolygon[2] = b2Vec2(wheelWidthM, 0.6 * wheelHeightM) - center;
	mPolygon[3] = b2Vec2(wheelWidthM, 0.4 * wheelHeightM) - center;
	mPolygon[4] = b2Vec2(0.8 * wheelWidthM, 0.0) - center;
	mPolygon[5] = b2Vec2(0.2 * wheelWidthM, 0.0) - center;
	mPolygon[6] = b2Vec2(0.0, 0.4 * wheelHeightM) - center;
	mPolygon[7] = b2Vec2(0.0, 0.6 * wheelHeightM) - center;

	polygonShape.Set(mPolygon, 8);
	fixtureDef.shape = &polygonShape;
	fixtureDef.density = engine->computeDensity(
			QPolygonF(QRectF(0, 0, twoDModel::robotWheelDiameterInPx / 2, twoDModel::robotWheelDiameterInPx))
			, wheelMass);

	mBody->CreateFixture( &fixtureDef );

	mBody->SetUserData(this);

	for (int i = 0; i < polygonShape.GetVertexCount(); ++i) {
		mDebuggingDrawPolygon.append(mEngine->positionToScene(polygonShape.GetVertex(i) + mBody->GetPosition()));
	}

	if (!mDebuggingDrawPolygon.isEmpty() & !mDebuggingDrawPolygon.isClosed()) {
		mDebuggingDrawPolygon.append(mDebuggingDrawPolygon.first());
	}
}

Box2DWheel::~Box2DWheel() {
	mBody->GetWorld()->DestroyBody(mBody);
	delete[] mPolygon;
}

b2Vec2 Box2DWheel::getLateralVelocity() const {
	b2Vec2 currentRightNormal = mBody->GetWorldVector( b2Vec2(0, 1) );
	return b2Dot( currentRightNormal, mBody->GetLinearVelocity() ) * currentRightNormal;
}

b2Vec2 Box2DWheel::getForwardVelocity() const {
	b2Vec2 currentForwardNormal = mBody->GetWorldVector( b2Vec2(1, 0) );
	return b2Dot( currentForwardNormal, mBody->GetLinearVelocity() ) * currentForwardNormal;
}

void Box2DWheel::keepConstantSpeed(float speed) {
	const int acceleration = 20;

	if (!mathUtils::Math::eq(prevSpeed, speed)){
		mRobot.applyForceToCenter(mBody->GetWorldVector(b2Vec2(0.1f * mathUtils::Math::sign(speed), 0)), true);
		prevSpeed = speed;
	}

	b2Vec2 lateralImpulse = mBody->GetMass() * -getLateralVelocity();
	mBody->ApplyLinearImpulse(lateralImpulse, mBody->GetWorldCenter(), true );

	b2Vec2 forwardNormal = getForwardVelocity();
	float scalar = b2Dot(forwardNormal, mBody->GetWorldVector(b2Vec2(1, 0))) < 0 ? -1 : 1;
	float currentForwardSpeed = forwardNormal.Normalize() * scalar;
	forwardNormal = mBody->GetWorldVector(b2Vec2(1, 0));

	float desiredSpeed = currentForwardSpeed;
	float speedPiece = fabs(speed) / acceleration;
	if (currentForwardSpeed < speed) {
		desiredSpeed += speedPiece;
	} else if (currentForwardSpeed > speed) {
		desiredSpeed -= speedPiece;
	} else {
		return;
	}

	float speedDiff = desiredSpeed - currentForwardSpeed;
	b2Vec2 linearImpulse = speedDiff * mBody->GetMass() * forwardNormal;
	mBody->ApplyLinearImpulse(linearImpulse, mBody->GetWorldCenter(), true);
}

b2Body *Box2DWheel::getBody()
{
	return mBody;
}
