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
#include <QDebug>

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
	twoDModel::model::RobotModel * const model; // Doesn't take ownership

	box2DRobot(b2World* world
			   , twoDModel::model::RobotModel * const robotModel
			   , box2DWheel &leftWheel
			   , box2DWheel &rightWheel
			   , const qreal pixelsInCm)
		: leftWheel(leftWheel)
		, rightWheel(rightWheel)
		, model(robotModel)
	{
		b2Vec2 positionCm = b2Vec2(static_cast<float32>(model->position().x() / pixelsInCm)
			, static_cast<float32>(model->position().y() / pixelsInCm));
		float32 widthCm = static_cast<float32>(model->info().size().width() / pixelsInCm);
		float32 heightCm = static_cast<float32>(model->info().size().height() / pixelsInCm);

		qDebug() << "robot constructor box2D begins";

		b2BodyDef bodyDef;
		bodyDef.position = positionCm;
		bodyDef.angle = model->rotation();
		bodyDef.type = b2_dynamicBody;

		mBody = world->CreateBody(&bodyDef);

		b2FixtureDef robotFixture;
		b2PolygonShape polygonShape;
		polygonShape.SetAsBox( widthCm / 2.0f, heightCm / 2.0f );

		robotFixture.shape = &polygonShape;
		robotFixture.density = model->info().mass() / heightCm * widthCm;
		robotFixture.friction = model->info().friction();

		mBody->CreateFixture(&robotFixture);
		mBody->SetUserData( this );

		qDebug() << "Pre connect wheels";

		connectWheel(*world, leftWheel, -widthCm);
		connectWheel(*world, rightWheel, widthCm);
	}

	~box2DRobot() {
		leftWheel.mBody->GetWorld()->DestroyJoint(leftJoint);
		rightWheel.mBody->GetWorld()->DestroyJoint(rightJoint);
		leftWheel.mBody->GetWorld()->DestroyBody(leftWheel.mBody);
		rightWheel.mBody->GetWorld()->DestroyBody(rightWheel.mBody);
		mBody->GetWorld()->DestroyBody(mBody);
	}

private:
	void connectWheel(b2World &world, box2DWheel &wheel, float32 widthCm){
		b2RevoluteJointDef revDef;
		revDef.bodyA = wheel.mBody;
		revDef.bodyB = mBody;
		revDef.collideConnected = false;

		revDef.localAnchorA = wheel.mBody->GetLocalCenter();
		revDef.localAnchorB = b2Vec2(0, widthCm / 3 * 2);

		revDef.referenceAngle = 0;
		revDef.enableLimit = true;
		revDef.lowerAngle = 0;
		revDef.upperAngle = 0;

		qDebug() << "wheel created";

		world.CreateJoint(&revDef);
	}
};

}
}
}
}
