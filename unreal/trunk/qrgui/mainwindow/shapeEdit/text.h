#pragma once

#include <QtGui/QGraphicsTextItem>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QFont>

class Text : public QGraphicsTextItem
{
public:
	Text(qreal x, qreal y, QString const &text = "text", bool isDynamic = false, QGraphicsTextItem *parent = 0);
	bool isDynamicText();
protected:
	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
	void drawExtractionForItem(QPainter* painter);
	void drawForDynamicText(QPainter* painter);
private:
	bool mIsDynamicText;
	QRectF mRect;
	QFont mFont;
};
