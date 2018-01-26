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
	: mRobot(robot)
	, mEngine(engine)
	, wheelHeightM(engine->pxToM(twoDModel::robotWheelDiameterInPx / 2))
	, wheelWidthM(engine->pxToM(twoDModel::robotWheelDiameterInPx))
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = positionBox2D;
	bodyDef.angle = rotationBox2D;
	mBody = engine->box2DWorld().CreateBody( &bodyDef );

	b2FixtureDef fixtureDef;
	fixtureDef.friction = wheelFriction;
	b2PolygonShape polygonShape;
	polygonShape.SetAsBox( wheelWidthM / 2, wheelHeightM / 2 );
	fixtureDef.shape = &polygonShape;
	fixtureDef.density = engine->computeDensity(
			QPolygonF(QRectF(0, 0, twoDModel::robotWheelDiameterInPx / 2, twoDModel::robotWheelDiameterInPx))
			, wheelMass);

	mBody->CreateFixture( &fixtureDef );

	mBody->SetUserData(this);
	prevSpeed = 0;

	for (int i = 0; i < polygonShape.GetVertexCount(); ++i) {
		mDebuggingDrawPolygon.append(mEngine->positionToScene(polygonShape.GetVertex(i) + mBody->GetPosition()));
	}
	if (!mDebuggingDrawPolygon.isEmpty() & !mDebuggingDrawPolygon.isClosed()) {
		mDebuggingDrawPolygon.append(mDebuggingDrawPolygon.first());
	}
}

Box2DWheel::~Box2DWheel() {
	mBody->GetWorld()->DestroyBody(mBody);
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
	float speedPiece = abs(speed) / 20;
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
