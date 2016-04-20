/* Copyright 2016 Polina Tarasova
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

#include <metaMetaModel/portHelpers.h>

class QPainter;
class QRectF;

namespace qReal {
namespace gui {
namespace editor {

/// Circular port description.
class StatCircular
{
public:
	/// Circular port.
	struct CircularPort
	{
		/// X coordinate of circle center.
		qreal x;

		/// Y coordinate of circle center.
		qreal y;

		/// Radius on the X axis.
		qreal rx;

		/// Radius on the Y axis.
		qreal ry;
	};

	explicit StatCircular(const CircularPortInfo &info);

	void paint(QPainter *painter, const QRectF &contents) const;

	/// Recalculates port parameters so that it fits into given rectangle.
	CircularPort transformForContents(const QRectF &contents) const;

	/// Returns type of this port to filter out links that can be connected to it.
	QString type() const;

private:
	/// Center of the port.
	QPointF mCenter;

	/// True if port shall not be scalable on X axis.
	const bool mPropX;

	/// True if port shall not be scalable on Y axis.
	const bool mPropY;

	/// Initial radius of a port.
	const qreal mR;

	/// Initial width of a port bounding rect. Despite it being circular bounding rectangle may not be a square, and
	/// initial width and height are used to transform port for required bounding rect when resizing.
	int mInitWidth;

	/// Initial height of a port bounding rect. Despite it being circular bounding rectangle is not nessesarily a
	/// square, and initial width and height are used to transform port for required bounding rect when resizing.
	int mInitHeight;

	QString mType;
};

}
}
}
