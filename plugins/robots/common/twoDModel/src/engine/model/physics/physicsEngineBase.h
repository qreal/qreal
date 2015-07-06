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

#include <QtGui/QVector2D>
#include <QtGui/QPainterPath>

namespace twoDModel {
namespace model {

class WorldModel;

namespace physics {

/// An interface for 2D model physical engine strategy. Calling recalculateParams() method
/// counts position shift and angle modification for the given time interval that can be get
/// with shift() and rotation() methods correspondingly.
class PhysicsEngineBase
{
public:
	explicit PhysicsEngineBase(const WorldModel &worldModel);
	virtual ~PhysicsEngineBase();

	/// Returns robot transition vector for the time interval given in the last recalculateParams() call
	QVector2D shift() const;

	/// Returns robot angle modification for the time interval given in the last recalculateParams() call
	qreal rotation() const;

	/// Counts robot`s parameters modifications for the given time interval
	virtual void recalculateParams(qreal timeInterval, qreal speed1, qreal speed2
			, bool engine1Break, bool engine2Break
			, const QPointF &rotationCenter, qreal robotAngle
			, const QPainterPath &robotBoundingPath) = 0;

protected:
	const WorldModel &mWorldModel;
	QVector2D mPositionShift;
	qreal mRotation;
};

}
}
}
