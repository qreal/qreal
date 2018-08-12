/* Copyright 2015 Dmitry Mordvinov
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

#include <QtGui/QPolygonF>
#include <QtCore/qglobal.h>

namespace twoDModel {
namespace items {

/// Provides information for physics such as friction, mass, form, etc.
class SolidItem
{

public:
	/// static: zero mass, zero velocity, may be manually moved
	/// kinematic: zero mass, non-zero velocity set by user
	/// dynamic: positive mass, non-zero velocity determined by forces
	enum BodyType
	{
		DYNAMIC,
		STATIC,
		KINEMATIC
	};

	virtual ~SolidItem(){}

	/// Returns body form as polygon.
	virtual QPolygonF collidingPolygon() const = 0;

	/// Returns true if body form is circle, in such case, radius is a half of collidingPolygon() bounding rect size
	virtual bool isCircle() const;

	/// Returns body's mass in kg.
	virtual qreal mass() const = 0;

	/// Returns body's friction.
	virtual qreal friction() const = 0;

	/// Returns body's type.
	virtual BodyType bodyType() const = 0;

	/// Returns body's angular damping.
	virtual qreal angularDamping() const;

	/// Returns body's linear damping.
	virtual qreal linearDamping() const;
};

}
}
