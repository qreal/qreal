/* Copyright 2017 Gleb Zakharov, QReal Research Group
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

class b2Body;
class b2Vec2;

namespace twoDModel {
namespace model {
namespace physics {
namespace parts {

class box2DRobot;

class box2DWheel
{
public:
	b2Body *body;
	box2DRobot &robot;
	const float wheelHeightM;
	const float wheelWidthM;
	const float wheelFriction = 1.0f;
	const float wheelDensity = 1.0f;

	box2DWheel(const b2Vec2 &positionBox2D, const float rotationBox2D, box2DRobot &robot);
	~box2DWheel();
	b2Vec2 getLateralVelocity() const;
	b2Vec2 getForwardVelocity() const;
	void keepConstantSpeed(float speed, float time);

protected:
	float prevSpeed = 0;
};

}
}
}
}
