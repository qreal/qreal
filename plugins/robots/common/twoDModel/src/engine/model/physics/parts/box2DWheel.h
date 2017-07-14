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

#pragma once

#include <qrutils/mathUtils/math.h>
#include <twoDModel/engine/model/robotModel.h>

#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Dynamics/b2Fixture.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>

namespace twoDModel {
namespace model {
namespace physics {
namespace parts {

class box2DWheel
{
public:
	b2Body* mBody;
	const float maxLateralImpulse = 1.5f;
	float32 prevSpeed = 0;
	b2Body* robot;

	box2DWheel(b2World* world
			   , const float32 wheelWidth
			   , const float32 wheelHeight
			   , const float32 wheelFriction
			   , const float32 wheelDensity
			   , const b2Vec2 coords
			   , b2Body *robot)
	{
		this->robot = robot;
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position = coords;
		mBody = world->CreateBody(&bodyDef);

		b2FixtureDef fixtureDef;
		fixtureDef.friction = wheelFriction;
		b2PolygonShape polygonShape;
		polygonShape.SetAsBox( wheelHeight / 2, wheelWidth / 2 );
		fixtureDef.shape = &polygonShape;
		fixtureDef.density = wheelDensity;

		mBody->CreateFixture( &fixtureDef );

		mBody->SetUserData( this );
		prevSpeed = 0;
	}

	b2Vec2 getLateralVelocity() {
		  b2Vec2 currentRightNormal = mBody->GetWorldVector( b2Vec2(0, 1) );
		  return b2Dot( currentRightNormal, mBody->GetLinearVelocity() ) * currentRightNormal;
	}

	b2Vec2 getForwardVelocity() {
		b2Vec2 currentForwardNormal = mBody->GetWorldVector( b2Vec2(1, 0) );
		return b2Dot( currentForwardNormal, mBody->GetLinearVelocity() ) * currentForwardNormal;
	}

	void keepConstantSpeed(float32 speed) {

		if (!mathUtils::Math::eq(speed, prevSpeed, mathUtils::EPS)){
			robot->ApplyForceToCenter(mBody->GetWorldVector(b2Vec2(0.01f, 0)), true);
			prevSpeed = speed;
		}
		b2Vec2 impulse = mBody->GetMass() * -getLateralVelocity();
		mBody->ApplyLinearImpulse( impulse, mBody->GetWorldCenter(), true );

		b2Vec2 currentForwardNormal = getForwardVelocity();
		float32 currentForwardSpeed = currentForwardNormal.Normalize();
		float32 speedChange = currentForwardSpeed - speed;
		b2Vec2 forwardImpulse = speedChange * mBody->GetMass() * -currentForwardNormal;
		mBody->ApplyLinearImpulse(forwardImpulse, mBody->GetWorldCenter(), true );
	}
	~box2DWheel() {
		mBody->GetWorld()->DestroyBody(mBody);
	}

//	Box2DWheel(const QPointF &position, b2Body *robotBody);
};

}
}
}
}
