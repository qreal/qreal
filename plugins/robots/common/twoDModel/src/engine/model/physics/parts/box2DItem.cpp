/* Copyright 2017 CyberTech Labs Ltd.
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
#include "box2DItem.h"

#include <Box2D/Box2D.h>

#include "src/engine/model/physics/box2DPhysicsEngine.h"
#include "src/engine/items/solidItem.h"

using namespace twoDModel::model::physics;
using namespace twoDModel::items;
using namespace parts;

#include <QDebug>
Box2DItem::Box2DItem(twoDModel::model::physics::Box2DPhysicsEngine *engine
		, twoDModel::items::SolidItem &item, const b2Vec2 &pos, float angle)
	: mIsCircle(false)
	, mItem(item)
	, mEngine(*engine)
{
	b2BodyDef bodyDef;
	bodyDef.position = pos;
	bodyDef.angle = angle;
	if (item.bodyType() == SolidItem::DYNAMIC) {
		bodyDef.type = b2_dynamicBody;
	} else if (item.bodyType() == SolidItem::KINEMATIC) {
		bodyDef.type = b2_kinematicBody;
	} else if (item.bodyType() == SolidItem::STATIC) {
		bodyDef.type = b2_staticBody;
	}

	mBody = this->mEngine.box2DWorld().CreateBody(&bodyDef);
	mBody->SetAngularDamping(item.angularDamping());
	mBody->SetLinearDamping(item.linearDamping());

	b2FixtureDef fixture;
	fixture.restitution = 0.5;
	b2CircleShape circleShape;
	b2PolygonShape polygonShape;
	QPolygonF collidingPolygon = item.collidingPolygon();
	QPointF localCenter = collidingPolygon.boundingRect().center();
	if (item.isCircle()) {
		mIsCircle = true;
		qreal radius = collidingPolygon.boundingRect().height() / 2;
		circleShape.m_radius = this->mEngine.pxToM(radius);
		fixture.shape = &circleShape;
		fixture.density = engine->computeDensity(radius, item.mass());
	} else {
		if (collidingPolygon.isClosed()) {
			collidingPolygon.removeLast();
		}

		mPolygon = new b2Vec2[collidingPolygon.size()];
		for (int i = 0; i < collidingPolygon.size(); ++i) {
			mPolygon[i] = engine->positionToBox2D(collidingPolygon.at(i) - localCenter);
		}

		polygonShape.Set(mPolygon, collidingPolygon.size());
		fixture.shape = &polygonShape;
		fixture.density = engine->computeDensity(collidingPolygon, item.mass());
	}

	fixture.friction = item.friction();
	mBody->CreateFixture(&fixture);
	mBody->SetUserData(this);
}

Box2DItem::~Box2DItem()
{
	mBody->GetWorld()->DestroyBody(mBody);
	if (!mIsCircle) {
		delete[] mPolygon;
	}
}

void Box2DItem::moveToPosition(const b2Vec2 &pos)
{
	mBody->SetTransform(pos, mBody->GetAngle());
}

void Box2DItem::setRotation(float angle)
{
	mBody->SetTransform(mBody->GetPosition(), angle);
}

const b2Vec2 &Box2DItem::getPosition() const
{
	return mBody->GetPosition();
}

float Box2DItem::getRotation() const
{
	return mBody->GetAngle();
}

b2Body *Box2DItem::getBody() const
{
	return mBody;
}
