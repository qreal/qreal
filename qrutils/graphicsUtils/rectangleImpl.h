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
#include <QtGui/QImage>

#include "qrutils/utilsDeclSpec.h"

namespace graphicsUtils
{

class QRUTILS_EXPORT RectangleImpl
{
public:
	RectangleImpl();
	QPainterPath shape(qreal x1, qreal y1, qreal x2, qreal y2, const int drift) const;
	QRectF boundingRect(qreal x1, qreal y1, qreal x2, qreal y2, const int scalingDrift) const;
	void drawRectItem(QPainter *painter, qreal x1, qreal y1, qreal x2, qreal y2);
	void drawEllipseItem(QPainter *painter, qreal x1, qreal y1, qreal x2, qreal y2);
	void drawImageItem(QPainter *painter, qreal x1, qreal y1, qreal x2, qreal y2, const QImage &image);
	void drawImageItemWithMirrored(QPainter *painter, qreal x1, qreal y1, qreal x2, qreal y2, const QImage &myImage);

private:
	QRectF calcRect(qreal x1, qreal y1, qreal x2, qreal y2) const;
};

}
