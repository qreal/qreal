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
#include "box2DSensor.h"

#include <qrutils/mathUtils/math.h>
#include "twoDModel/engine/model/constants.h"
#include "box2DRobot.h"

#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Dynamics/b2Fixture.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>

using namespace twoDModel::model::physics::parts;

box2DSensor::box2DSensor(box2DRobot &robot)
	: robot(robot)
{
}

box2DSensor::~box2DSensor()
{
}
