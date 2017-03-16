/* Copyright 2016 Dmitry Mordvinov
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

#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Dynamics/Joints/b2RevoluteJoint.h>

#include "twoDModel/engine/model/robotModel.h"
#include "box2DWheel.h"

//class twoDModel::model::physics::Box2DPhysicsEngineNew;

namespace twoDModel {
namespace model {
namespace physics {
namespace parts {

class box2DRobot{
public:
	b2Body* mBody;
	box2DWheel &leftWheel, &rightWheel;
	b2RevoluteJoint *leftJoint, *rightJoint;

	box2DRobot(b2World* world, box2DWheel &leftWheel
			   , box2DWheel &rightWheel
			   , const b2Vec2 &positionCm
			   , const qreal rotation
			   , const float32 widthCm
			   , const float32 heightCm
			   , const qreal mass
			   , const qreal friction)
		: leftWheel(leftWheel)
		, rightWheel(rightWheel)
	{
		b2BodyDef bodyDef;

		bodyDef.position = positionCm;
		bodyDef.angle = rotation;
		bodyDef.type = b2_dynamicBody;

		mBody = world->CreateBody(&bodyDef);

		b2FixtureDef robotFixture;
		b2PolygonShape polygonShape;
		polygonShape.SetAsBox( widthCm / 2.0f, heightCm / 2.0f );

		robotFixture.shape = &polygonShape;
		robotFixture.density = mass / heightCm * widthCm;
		robotFixture.friction = friction;

		mBody->CreateFixture(&robotFixture);
		mBody->SetUserData( this );
//		leftWheel.mBody->SetTransform(b2Vec2(-2.0f - 1.0f, -0.5f), 0);
//		rightWheel.mBody->SetTransform(b2Vec2(2.0f + 1.0f, -0.5f), 0);

		connectWheel(*world, leftWheel);
		connectWheel(*world, rightWheel);
	}

	~box2DRobot() {
		leftWheel.mBody->GetWorld()->DestroyJoint(leftJoint);
		rightWheel.mBody->GetWorld()->DestroyJoint(rightJoint);
		leftWheel.mBody->GetWorld()->DestroyBody(leftWheel.mBody);
		rightWheel.mBody->GetWorld()->DestroyBody(rightWheel.mBody);
		mBody->GetWorld()->DestroyBody(mBody);
	}

private:
	void connectWheel(b2World &world, box2DWheel &wheel){
		b2RevoluteJointDef revDef;
		revDef.bodyA = wheel.mBody;
		revDef.bodyB = mBody;
		revDef.collideConnected = false;

		revDef.localAnchorA = wheel.mBody->GetLocalCenter();
		revDef.localAnchorB = mBody->GetLocalCenter();

		revDef.referenceAngle = 0;
		revDef.enableLimit = true;
		revDef.lowerAngle = 0;
		revDef.upperAngle = 0;

		world.CreateJoint(&revDef);
	}
};

}
}
}
}
