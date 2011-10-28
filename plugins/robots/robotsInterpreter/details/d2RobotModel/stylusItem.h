#pragma once
#include <QtCore/QList>
#include <QtGui/QGraphicsItem>
#include <QtGui/QPainter>
#include <QtXml/QDomDocument>
#include "lineItem.h"
#include "../../../../../qrutils/graphicsUtils/abstractItem.h"
#include "../../../../../qrutils/graphicsUtils/stylusImpl.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace d2Model {

class StylusItem : public QObject, public ColorFieldItem
{
	Q_OBJECT
public:
	StylusItem(qreal x1, qreal y1);
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

        virtual void resizeItem(QGraphicsSceneMouseEvent *event);
        virtual QDomElement serialize(QDomDocument &document, QPoint const &topLeftPicture);
        virtual void deserialize(QDomElement const &element);

private:
	qreal mTmpX1;
	qreal mTmpY1;
	QList<AbstractItem *> mAbstractListLine;
	graphicsUtils::StylusImpl mStylusImpl;
};
}
}
}
}
}
