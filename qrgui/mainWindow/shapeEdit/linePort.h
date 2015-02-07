#pragma once

#include "mainWindow/shapeEdit/item.h"
#include "mainWindow/shapeEdit/line.h"

class LinePort : public Line
{
public:
	LinePort(qreal x1, qreal y1, qreal x2, qreal y2, Line* parent = 0);
	LinePort(const LinePort &other);
	virtual Item* clone();

	virtual QPair<QDomElement, Item::DomElementTypes> generateItem(QDomDocument &document, const QPoint &topLeftPicture);

	void setType(const QString &type);
	QString getType() const;

private:
	QString mType;
};
