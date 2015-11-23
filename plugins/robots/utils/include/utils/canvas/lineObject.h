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

/// A helper entity for storing segment properties and then getting them via Qt reflection.
class ROBOTS_UTILS_EXPORT LineObject : public CanvasObject
{
	Q_OBJECT
	Q_PROPERTY(QPoint begin READ begin WRITE setBegin)
	Q_PROPERTY(QPoint end READ end WRITE setEnd)
	Q_PROPERTY(QRect boundingRect READ boundingRect)

public:
	explicit LineObject(QObject *parent = 0);
	LineObject(const QPoint &begin
			, const QPoint &end
			, const QColor &color = Qt::black
			, int thickness = 1
			, QObject *parent = 0);

	/// Returns the first end of the segment.
	QPoint begin() const;

	/// Sets the first end of the segment.
	void setBegin(const QPoint &begin);

	/// Returns the second end of the segment.
	QPoint end() const;

	/// Sets the second end of the segment.
	void setEnd(const QPoint &end);

	/// Returns the bounding box of this line.
	QRect boundingRect() const;

	void paint(QPainter *painter) override;
	QJsonObject toJson() const override;

private:
	QPoint mBegin;
	QPoint mEnd;
};

}
