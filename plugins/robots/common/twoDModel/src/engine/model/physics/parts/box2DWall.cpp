/* Copyright 2017 Gleb Zakharov
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
#include "box2DWall.h"

#include <QPointF>
#include <QVector2D>
#include <QtMath>
#include <Box2D/Box2D.h>

#include "src/engine/model/physics/box2DPhysicsEngine.h"
#include "src/engine/items/wallItem.h"
#include "qrutils/mathUtils/math.h"

using namespace twoDModel::model::physics;
using namespace parts;

box2DWall::box2DWall(twoDModel::model::physics::box2DPhysicsEngine *engine
		, twoDModel::items::WallItem &wallItem)
		: item(wallItem), engine(*engine)
{
	QPointF beg = item.begin();
	QPointF end = item.end();
	QVector2D vect = QVector2D(end - beg);
	float32 length = vect.length();
	float32 cosOfAngle = QVector2D::dotProduct(vect, QVector2D(1, 0)) / length;
	float32 angle = mathUtils::Math::eq(length, 0) ? 0 : qAcos(cosOfAngle);
	angle *= end.y() > beg.y() ? -1 : 1;

	b2BodyDef bodyDef;
	b2Vec2 pos = this->engine.positionToBox2D(item.boundingRect().center());
	bodyDef.position = pos;
	bodyDef.angle = angle;
	bodyDef.type = b2_staticBody;

	body = this->engine.box2DWorld().CreateBody(&bodyDef);
	b2FixtureDef fixture;
	b2PolygonShape shape;
	QPointF sceneSize = QPointF(length + item.width(), item.width());

	shape.SetAsBox(this->engine.pxToM(sceneSize.x()) / 2, this->engine.pxToM(sceneSize.y()) / 2);
	fixture.shape = &shape;
	fixture.density = 1.0f;
	body->CreateFixture(&fixture);
	body->SetUserData( this );
}

box2DWall::~box2DWall(){
	body->GetWorld()->DestroyBody(body);
}
