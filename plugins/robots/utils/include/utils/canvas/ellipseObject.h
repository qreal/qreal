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

#include <QtCore/QRect>

#include "utils/canvas/canvasObject.h"

namespace utils {

/// A helper entity for storing ellipse properties and then getting them via Qt reflection.
class ROBOTS_UTILS_EXPORT EllipseObject : public CanvasObject
{
	Q_OBJECT
	Q_PROPERTY(QPoint center READ center WRITE setCenter)
	Q_PROPERTY(int w READ semiDiameterX WRITE setSemiDiameterX)
	Q_PROPERTY(int h READ semiDiameterY WRITE setSemiDiameterY)
	Q_PROPERTY(QRect rect READ rect)

public:
	explicit EllipseObject(QObject *parent = 0);
	EllipseObject(const QPoint &center
			, int semiDiameterX
			, int semiDiameterY
			, const QColor &color = Qt::black
			, int thickness = 1
			, QObject *parent = 0);

	/// Returns the coordinates of the central point in pixels.
	QPoint center() const;

	/// Sets the coordinates of the central point in pixels.
	void setCenter(const QPoint &center);

	/// Returns the semi-axis length by the x-dimension of the ellipse.
	int semiDiameterX() const;

	/// Sets the semi-axis length by the x-dimension of the ellipse.
	void setSemiDiameterX(int a);

	/// Returns the semi-axis length by the y-dimension of the ellipse.
	int semiDiameterY() const;

	/// Sets the semi-axis length by the y-dimension of the ellipse.
	void setSemiDiameterY(int b);

	/// Returns the bounding rectangle of this ellipse.
	QRect rect() const;

	void paint(QPainter *painter) override;

private:
	QPoint mCenter;
	int mSemiDiameterX;
	int mSemiDiameterY;
};

}
