#pragma once
#include <QtCore/QString>
#include <QtGui/QGraphicsTextItem>

class ElementTitleInterface : public QGraphicsTextItem
{
public:
	virtual void setBackground(Qt::GlobalColor const &background) = 0;
	virtual void setScaling(bool scalingX, bool scalingY) = 0;
};

class ElementTitleFactoryInterface
{
public:
	virtual ElementTitleInterface *createTitle(qreal x, qreal y, QString const &text) = 0;
	virtual ElementTitleInterface *createTitle(qreal x, qreal y, QString const &binding, bool readOnly) = 0;
};
