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

#include <Box2D/Box2D.h>

class QPolygonF;

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
			, const twoDModel::items::SolidItem &mItem, const b2Vec2 &pos, float angle);

	~Box2DItem();

	/// Transform item to a new position, IMPORTANT: \a pos is a center point of box2d object.
	void moveToPosition(const b2Vec2 &pos);
	void setRotation(float angle);

	const b2Vec2 &getPosition();
	float getRotation();

	b2Body *getBody() const;

	/// Returns true if the position or angle were changed since previous get or set calls.
	bool angleOrPositionChanged() const;

private:
	b2Body *mBody; // Takes ownership
	b2Vec2 *mPolygon; // Takes ownership

	bool mIsCircle;

	b2Vec2 mPreviousPosition;
	float mPreviousRotation;

	twoDModel::model::physics::Box2DPhysicsEngine &mEngine; // Doesn't take ownership
};

}
}
}
}
