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

#include "abstractItem.h"
#include "qrutils/utilsDeclSpec.h"

namespace graphicsUtils {

class QRUTILS_EXPORT StylusImpl
{
public :
	StylusImpl();

	QRectF boundingRect(const QList<AbstractItem *> &listLine) const;
	QPainterPath shape(const QList<AbstractItem *> &listLine) const;

	void drawItem(const QList<AbstractItem *> &listLine, QPainter *painter, const QStyleOptionGraphicsItem *option
			, QWidget *widget = 0);

	void drawExtractionForItem(const QList<AbstractItem *> &listLine, QPainter *painter);
	void drawFieldForResizeItem(QPainter* painter);
	void drawScalingRects(QPainter* painter);

	void setPenStyle(const QList<AbstractItem *> &listLine, const QString &text);
	void setPenWidth(const QList<AbstractItem *> &listLine, int width);
	void setPenColor(const QList<AbstractItem *> &listLine, const QString &text);
	void setBrushStyle(const QList<AbstractItem *> &listLine, const QString &text);
	void setBrushColor(const QList<AbstractItem *> &listLine, const QString &text);

	void resizeItem(QGraphicsSceneMouseEvent *event);

private:
	QRectF searchMaxMinCoord(const QList<AbstractItem *> &listLine) const;
};

}
