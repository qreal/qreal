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
	explicit SimplePhysicsEngine(const WorldModel &worldModel, const QList<RobotModel *> robots);

	QVector2D positionShift(RobotModel &robot) const override;
	qreal rotation(RobotModel &robot) const override;
	void recalculateParameters(qreal timeInterval) override;

private:
	void recalculateParameters(qreal timeInterval, RobotModel &robot);

	QMap<RobotModel *, QVector2D> mPositionShift;
	QMap<RobotModel *, qreal> mRotation;
};

}
}
}
