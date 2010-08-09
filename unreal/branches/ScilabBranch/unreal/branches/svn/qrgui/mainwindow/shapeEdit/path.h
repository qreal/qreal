#pragma once
#include "item.h"
#include <QtCore/QPair>

class Path : public Item
{
public:
	Path(QPainterPath const &path);
	virtual QRectF boundingRect() const;
	virtual void drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0) ;
	virtual QPair<QDomElement, Item::DomElementTypes> generateItem(QDomDocument &document, QPointF const &topLeftPicture);
	virtual void drawExtractionForItem(QPainter* painter);
	virtual void drawFieldForResizeItem(QPainter* painter);

private:
	QPainterPath mPath;
};
