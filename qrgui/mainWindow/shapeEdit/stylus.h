#pragma once

#include <QtCore/QList>

#include <qrutils/graphicsUtils/stylusImpl.h>

#include "mainWindow/shapeEdit/item.h"
#include "mainWindow/shapeEdit/line.h"

class Stylus : public Item
{
public:
	QList<Line *> mListLine;
	Stylus(qreal x1, qreal y1, Item* parent);
	Stylus(Stylus const &other);
	virtual Item* clone();
	void addLine(qreal x2, qreal y2);
	void addLineInList(Line *line);

	virtual QRectF boundingRect() const;
	virtual QPainterPath shape() const;
	virtual void drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
	virtual void drawExtractionForItem(QPainter* painter);
	virtual void drawFieldForResizeItem(QPainter* painter);
	virtual void drawScalingRects(QPainter* painter);
	virtual void setPenStyle(QString const &text);
	virtual void setPenWidth(int width);
	virtual void setPenColor(QString const &text);
	virtual void setBrushStyle(QString const &text);
	virtual void setBrushColor(QString const &text);
	virtual QPair<QDomElement, Item::DomElementTypes> generateItem(QDomDocument &document, QPoint const &topLeftPicture);

private:
	qreal mTmpX1;
	qreal mTmpY1;
	QList<AbstractItem *> mAbstractListLine;
	graphicsUtils::StylusImpl mStylusImpl;
};
