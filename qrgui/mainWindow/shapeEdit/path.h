#pragma once

#include <QtCore/QPair>

#include "mainWindow/shapeEdit/item.h"

class Path : public Item
{
public:
	Path(const QPainterPath &path);
	Path(const Path &other);
	virtual Item* clone();
	virtual QRectF boundingRect() const;
	virtual void drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0) ;
	virtual QPair<QDomElement, Item::DomElementTypes> generateItem(QDomDocument &document
			, const QPoint &topLeftPicture);

	virtual void drawExtractionForItem(QPainter* painter);
	virtual void drawFieldForResizeItem(QPainter* painter);
	virtual void drawScalingRects(QPainter* painter);

private:
	QPainterPath mPath;
};
