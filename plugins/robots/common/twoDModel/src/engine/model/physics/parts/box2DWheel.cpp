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

box2DWheel::box2DWheel(const b2Vec2 &positionBox2D, const float rotationBox2D, box2DRobot &robot)
	: robot(robot)
	, wheelHeightM(robot.engine->pxToM(twoDModel::robotWheelDiameterInPx / 2))
	, wheelWidthM(robot.engine->pxToM(twoDModel::robotWheelDiameterInPx))
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = positionBox2D;
	bodyDef.angle = rotationBox2D;
	body = robot.world.CreateBody( &bodyDef );

	b2FixtureDef fixtureDef;
	fixtureDef.friction = wheelFriction;
	b2PolygonShape polygonShape;
	polygonShape.SetAsBox( wheelWidthM / 2, wheelHeightM / 2 );
	fixtureDef.shape = &polygonShape;
	fixtureDef.density = wheelDensity;

	body->CreateFixture( &fixtureDef );

	body->SetUserData( this );
	prevSpeed = 0;
}

box2DWheel::~box2DWheel() {
	body->GetWorld()->DestroyBody(body);
}

b2Vec2 box2DWheel::getLateralVelocity() const {
	b2Vec2 currentRightNormal = body->GetWorldVector( b2Vec2(0, 1) );
	return b2Dot( currentRightNormal, body->GetLinearVelocity() ) * currentRightNormal;
}

b2Vec2 box2DWheel::getForwardVelocity() const {
	b2Vec2 currentForwardNormal = body->GetWorldVector( b2Vec2(1, 0) );
	return b2Dot( currentForwardNormal, body->GetLinearVelocity() ) * currentForwardNormal;
}

void box2DWheel::keepConstantSpeed(float speed, float time) {

	if (!mathUtils::Math::eq(prevSpeed, speed)){
		robot.body->ApplyForceToCenter(body->GetWorldVector(b2Vec2(0.001f * mathUtils::Math::sign(speed), 0)), true);
		prevSpeed = speed;
	}

	b2Vec2 lateralImpulse = body->GetMass() * -getLateralVelocity();
	body->ApplyLinearImpulse(lateralImpulse, body->GetWorldCenter(), true );

	b2Vec2 forwardNormal = getForwardVelocity();
	float scalar = b2Dot(forwardNormal, body->GetWorldVector(b2Vec2(1, 0))) < 0 ? -1 : 1;
	float currentForwardSpeed = forwardNormal.Normalize() * scalar;
	forwardNormal = body->GetWorldVector(b2Vec2(1, 0));

	float desiredSpeed = currentForwardSpeed;
	float speedPiece = 0.01f;
	if (currentForwardSpeed < speed) {
		desiredSpeed += speedPiece;
	}
	else if (currentForwardSpeed > speed) {
		desiredSpeed -= speedPiece;
	}
	else return;

	float speedDiff = desiredSpeed - currentForwardSpeed;
	b2Vec2 linearImpulse = speedDiff * body->GetMass() * forwardNormal;
	body->ApplyLinearImpulse(linearImpulse, body->GetWorldCenter(), true);
}
