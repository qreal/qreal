#pragma once

#include <QtCore/QList>
#include <QtWidgets/QGraphicsItem>
#include <QtGui/QPainter>
#include <QtXml/QDomDocument>

#include <qrutils/graphicsUtils/abstractItem.h>
#include <qrutils/graphicsUtils/stylusImpl.h>

#include "lineItem.h"

namespace twoDModel {
namespace items {

class StylusItem : public QObject, public ColorFieldItem
{
	Q_OBJECT

public:
	StylusItem(qreal x1, qreal y1);

	void addLine(qreal x2, qreal y2);

	QRectF boundingRect() const override;
	QPainterPath shape() const override;

	void drawItem(QPainter* painter, QStyleOptionGraphicsItem const *option, QWidget *widget = 0) override;
	void drawExtractionForItem(QPainter* painter) override;
	void drawFieldForResizeItem(QPainter* painter) override;
	void drawScalingRects(QPainter* painter);

	void setPenStyle(QString const &text) override;
	void setPenWidth(int width) override;
	void setPenColor(QString const &text) override;
	void setBrushStyle(QString const &text) override;
	void setBrushColor(QString const &text) override;

	void resizeItem(QGraphicsSceneMouseEvent *event) override;
	QDomElement serialize(QDomDocument &document, QPoint const &topLeftPicture) override;
	void deserialize(QDomElement const &element) override;

private:
	void recalculateProperties();

	qreal mTmpX1;
	qreal mTmpY1;
	QList<AbstractItem *> mAbstractListLine;
	graphicsUtils::StylusImpl mStylusImpl;
	QRectF mBoundingRect;
};

}
}
