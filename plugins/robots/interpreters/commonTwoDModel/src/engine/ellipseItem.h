#pragma once
#include <QtCore/QList>
#include <QtWidgets/QGraphicsItem>
#include <QtGui/QPainter>
#include <QtXml/QDomDocument>

#include <qrutils/graphicsUtils/abstractItem.h>
#include <qrutils/graphicsUtils/rectangleImpl.h>
#include "colorFieldItem.h"

namespace twoDModel {

class EllipseItem : public QObject, public ColorFieldItem
{
	Q_OBJECT
public:
	EllipseItem(QPointF begin, QPointF end);

	virtual QRectF boundingRect() const;
	virtual QRectF calcNecessaryBoundingRect() const;
	virtual void drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
	virtual QDomElement serialize(QDomDocument &document, QPoint const &topLeftPicture);
	virtual void deserialize(QDomElement const &element);

private:
	graphicsUtils::RectangleImpl mEllipseImpl;
	void setPrivateData();
};

}
