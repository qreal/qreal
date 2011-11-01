#pragma once
#include "item.h"
#include "line.h"
#include <QtCore/QList>

class Stylus : public Item
{
private:
	qreal mTmpX1;
	qreal mTmpY1;
	QRectF searchMaxMinCoord() const;

public:
	QList<Line *> mListLine;
	Stylus(qreal x1, qreal y1, Item* parent);
	Stylus(Stylus const &other);
	virtual Item* clone();
	void addLine(qreal x2, qreal y2);

	virtual QRectF boundingRect() const;
	QPainterPath shape() const;
	virtual void drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
	virtual void drawExtractionForItem(QPainter* painter);
	virtual void drawFieldForResizeItem(QPainter* painter);
	virtual void drawScalingRects(QPainter* painter);
	virtual void setPenStyle(const QString& text);
	virtual void setPenWidth(int width);
	virtual void setPenColor(const QString& text);
	virtual void setBrushStyle(const QString& text);
	virtual void setBrushColor(const QString& text);
	virtual QPair<QDomElement, Item::DomElementTypes> generateItem(QDomDocument &document, QPoint const &topLeftPicture);
};
