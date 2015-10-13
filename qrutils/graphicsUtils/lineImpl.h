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

#include <QtGui/QPainter>

#include "qrutils/utilsDeclSpec.h"

class QDomElement;

namespace graphicsUtils {

class QRUTILS_EXPORT LineImpl
{
public:
	LineImpl();
	QLineF line(qreal x1, qreal y1, qreal x2, qreal y2) const;
	QPainterPath shape(const int drift, qreal x1, qreal y1, qreal x2, qreal y2) const;
	QRectF boundingRect(qreal x1, qreal y1, qreal x2, qreal y2, qreal penWidth, const int drift) const;
	QRectF realBoundingRectWithoutScene(qreal x1, qreal y1, qreal x2, qreal y2, qreal penWidth, const int drift) const;
	void drawItem(QPainter* painter, qreal x1, qreal y1, qreal x2, qreal y2);
	void drawPointExtractionForItem(QPainter* painter, qreal x1, qreal y1, qreal x2, qreal y2);
	void drawExtractionForItem(QPainter* painter, qreal x1, qreal y1, qreal x2, qreal y2, const int drift);
	void drawFieldForResizeItem(QPainter* painter, const int resizeDrift, qreal x1, qreal y1, qreal x2, qreal y2);
	QPair<qreal, qreal> reshapeRectWithShiftForLine(qreal x1, qreal y1, qreal x2, qreal y2, qreal differenceX
			, qreal differenceY, qreal size);

	/// Serializes line`s begin and end into the given XML element.
	void serialize(QDomElement &element, qreal x1, qreal y1, qreal x2, qreal y2) const;

	/// Parses contents of the given XML element, returns parsed begin and the end points of the line.
	QPair<QPointF, QPointF> deserialize(const QDomElement &element) const;

private:
	QPointF deserializePoint(const QString &string) const;
};
}
