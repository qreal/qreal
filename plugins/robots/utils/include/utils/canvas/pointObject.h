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

#include <QtCore/QPoint>

#include "utils/canvas/canvasObject.h"

namespace utils {

/// A helper entity for storing point properties and then getting them via Qt reflection.
class ROBOTS_UTILS_EXPORT PointObject : public CanvasObject
{
	Q_OBJECT
	Q_PROPERTY(int x READ x WRITE setX)
	Q_PROPERTY(int y READ y WRITE setY)
	Q_PROPERTY(QPoint pos READ pos)

public:
	explicit PointObject(QObject *parent = 0);
	PointObject(int x, int y, const QColor &color = Qt::black, int thickness = 1, QObject *parent = 0);

	/// Returns the x-coordinate of the point.
	int x() const;

	/// Sets the x-coordinate of the point.
	void setX(int x);

	/// Returns the y-coordinate of the point.
	int y() const;

	/// Sets the y-coordinate of the point.
	void setY(int y);

	/// Returns the coordinates of the point.
	QPoint pos() const;

	void paint(QPainter *painter) override;
	QJsonObject toJson() const override;

private:
	int mX;
	int mY;
};

}
