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

#include "mainWindow/shapeEdit/item/line.h"
#include "mainWindow/shapeEdit/item/typedEntity.h"
#include "mainWindow/shapeEdit/item/item.h"

namespace qReal {
namespace shapeEdit {

class LinePort : public Line, public TypedEntity
{
public:
	LinePort(qreal x1, qreal y1, qreal x2, qreal y2, Line* parent = 0);
	LinePort(const LinePort &other);
	virtual Item* clone();

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event, Scene *scene) override;

    // Port entities must have persistent appearance. So implement these methods as empty.
    virtual void setPenStyle(const QString &text);
    virtual void setPenWidth(int width);
    virtual void setPenColor(const QString &text);
    virtual void setBrushStyle(const QString &text);
    virtual void setBrushColor(const QString &text);
    virtual void setBrush(const QString &brushStyle, const QString &brushColor);
    virtual void setPen(const QString &penStyle, int width, const QString &penColor);
    virtual void setPenBrush(const QString &penStyle, int width, const QString &penColor, const QString &brushStyle
            , const QString &brushColor);

	virtual QPair<QDomElement, Item::DomElementTypes> generateItem(QDomDocument &document
			, const QPoint &topLeftPicture);

protected:
    virtual void customizeButton(CreateItemPushButton *button) const;
};

}
}
