/* Copyright 2015 CyberTech Labs Ltd.
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

#include <QtCore/QObject>
#include <QtGui/QPen>

#include "utils/utilsDeclSpec.h"

class QPainter;

namespace utils {

/// A helper entity for storing point properties and then getting them via Qt reflection.
class ROBOTS_UTILS_EXPORT CanvasObject : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QColor color READ color WRITE setColor)
	Q_PROPERTY(int thickness READ thickness WRITE setThickness)

public:
	explicit CanvasObject(QObject *parent = 0);
	CanvasObject(const QColor &color = Qt::black, int thickness = 1, QObject *parent = 0);

	/// Returns the color of the point.
	QColor color() const;

	/// Sets the color of the point.
	void setColor(const QColor &color);

	/// Returns the thickess of the object`s pen in pixels.
	int thickness() const;

	/// Sets the size of the object`s pen in pixels.
	void setThickness(int size);

	/// Must be redefined to paint an object via \a painter. Base implementation
	/// must be called to set pen and brush correctly.
	virtual void paint(QPainter *painter);

	/// Override must serialize current canvas object state into the instance of JSON object.
	virtual QJsonObject toJson() const = 0;

private:
	QPen mPen;
};

}
