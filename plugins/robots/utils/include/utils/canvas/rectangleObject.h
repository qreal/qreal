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

/// A helper entity for storing rectangle properties and then getting them via Qt reflection.
class ROBOTS_UTILS_EXPORT RectangleObject : public CanvasObject
{
	Q_OBJECT
	Q_PROPERTY(QRect shape READ shape)
	Q_PROPERTY(QRect boundingRect READ boundingRect)
	Q_PROPERTY(bool filled READ filled WRITE setFilled)

public:
	explicit RectangleObject(QObject *parent = 0);
	RectangleObject(const QRect &shape
			, const QColor &color = Qt::black
			, int thickness = 1
			, bool filled = false
			, QObject *parent = 0);

	/// Returns the coordinates of this rectangle.
	QRect shape() const;

	/// Returns the coordinates of this rectangle.
	QRect boundingRect() const;

	/// Returns true if this rectangle is filled with color.
	bool filled() const;

	/// Fills rectangle with its color or makes it empty.
	void setFilled(bool filled);

	void paint(QPainter *painter) override;
	QJsonObject toJson() const override;

private:
	QRect mShape;
	bool mFilled;
};

}
