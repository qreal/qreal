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

/// A helper entity for storing ellipse arc properties and then getting them via Qt reflection.
class ROBOTS_UTILS_EXPORT ArcObject : public CanvasObject
{
	Q_OBJECT
	Q_PROPERTY(QPoint ellipseCenter READ ellipseCenter WRITE setEllipseCenter)
	Q_PROPERTY(QRect ellipseRect READ ellipseRect WRITE setEllipseRect)
	Q_PROPERTY(int startAngle READ startAngle WRITE setStartAngle)
	Q_PROPERTY(int spanAngle READ spanAngle WRITE setSpanAngle)
	Q_PROPERTY(QRect boundingRect READ boundingRect)

public:
	explicit ArcObject(QObject *parent = 0);
	ArcObject(const QRect &rect
			, int startAngle
			, int spanAngle
			, const QColor &color = Qt::black
			, int thickness = 1
			, QObject *parent = 0);

	/// Returns the coordinates of the central point of arc`s ellipse in pixels.
	QPoint ellipseCenter() const;

	/// Sets the coordinates of the central point of arc`s ellipse  in pixels.
	void setEllipseCenter(const QPoint &center);

	/// Returns the bounding rectangle of arc`s ellipse.
	QRect ellipseRect() const;

	/// Returns the bounding rectangle of arc`s ellipse.
	void setEllipseRect(const QRect &rect);

	/// Returns the angle on ellipse where this arc starts.
	int startAngle() const;

	/// Sets the angle on ellipse where this arc starts.
	void setStartAngle(int angle);

	/// Returns the count of degrees that this arc occupies on ellipse.
	int spanAngle() const;

	/// Sets the count of degrees that this arc occupies on ellipse.
	void setSpanAngle(int angle);

	/// Returns the bounding box of the whole ellipse of this arc.
	/// @warning This is not the exact bounding rectangle of the arc.
	QRect boundingRect() const;

	void paint(QPainter *painter) override;
	QJsonObject toJson() const override;

private:
	QRect mRect;
	int mStartAngle;
	int mSpanAngle;
};

}
