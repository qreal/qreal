/* Copyright 2007-2015 QReal Research Group
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

#include "physicsEngineBase.h"

namespace twoDModel {
namespace model {
namespace physics {

/// An implementation of 2D model physical engine without any realistic effects
class SimplePhysicsEngine : public PhysicsEngineBase
{
public:
	explicit SimplePhysicsEngine(const WorldModel &worldModel);

	void recalculateParams(qreal timeInterval, qreal speed1, qreal speed2
			, bool engine1Break, bool engine2Break
			, const QPointF &rotationCenter, qreal robotAngle
			, const QPainterPath &robotBoundingPath) override;
};

}
}
}
