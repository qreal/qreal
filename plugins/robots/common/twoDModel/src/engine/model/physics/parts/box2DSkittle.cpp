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
#include "box2DSkittle.h"

#include <Box2D/Box2D.h>

#include "src/engine/model/physics/box2DPhysicsEngine.h"
#include "src/engine/items/skittleItem.h"

using namespace twoDModel::model::physics;
using namespace parts;


#include <QDebug>

box2DSkittle::box2DSkittle(twoDModel::model::physics::box2DPhysicsEngine *engine
		, twoDModel::items::SkittleItem &skittleItem)
	: item(skittleItem)
	, engine(*engine)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	QRectF boundingRect = QRectF(item.x(), item.y(), item.boundingRect().width(), item.boundingRect().height());
//	qDebug() << boundingRect;
	b2Vec2 pos = this->engine.positionToBox2D(boundingRect.center());
	bodyDef.position = pos;
	bodyDef.bullet = true;

	body = this->engine.box2DWorld().CreateBody(&bodyDef);
	b2FixtureDef fixture;
	b2PolygonShape shape;

	shape.SetAsBox(this->engine.pxToM(boundingRect.height()) * 0.5, this->engine.pxToM(boundingRect.width()) * 0.5);
	fixture.shape = &shape;
	fixture.density = 1.0f;
	fixture.friction = 0.3f;
	body->CreateFixture(&fixture);
	body->SetUserData(this);
}

box2DSkittle::~box2DSkittle()
{
	body->GetWorld()->DestroyBody(body);
}
