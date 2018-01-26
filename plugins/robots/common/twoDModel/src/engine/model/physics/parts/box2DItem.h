/* Copyright 2018 CyberTech Labs Ltd.
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
class QPolygonF;

class b2Body;
class b2Vec2;

namespace twoDModel{
namespace items {
	class SolidItem;
}

namespace model {
namespace physics {
	class Box2DPhysicsEngine;

namespace parts {


class Box2DItem
{
public:
	Box2DItem(twoDModel::model::physics::Box2DPhysicsEngine *mEngine
			, twoDModel::items::SolidItem &mItem, const b2Vec2 &pos, float angle);

	~Box2DItem();

	void moveToPosition(const b2Vec2 &pos);
	void setRotation(float angle);

	const b2Vec2 &getPosition() const;
	float getRotation() const;

	b2Body *getBody() const;

private:
	b2Body *mBody; // Takes ownership
	b2Vec2 *mPolygon; // Takes ownership

	bool mIsCircle;

	twoDModel::items::SolidItem &mItem; // Doesn't take ownership
	twoDModel::model::physics::Box2DPhysicsEngine &mEngine; // Doesn't take ownership
};

}
}
}
}
