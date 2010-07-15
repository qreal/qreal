#pragma once

#include <QtGui/QGraphicsItem>
#include <QtGui/QPen>
#include <QtGui/QGraphicsSceneMouseEvent>
#include <QtGui/QPainter>
#include <QtXml/QDomDocument>
#include <QtCore/QPair>

class Item : public QGraphicsItem
{
public:
	enum DragState {
		None,
		TopLeft,
		TopRight,
		BottomLeft,
		BottomRight
	};
	enum DomElementTypes {
		noneType,
		pictureType,
		labelType,
		portType
	};

	Item(QGraphicsItem* parent = 0);
	virtual QRectF boundingRect() const = 0;
	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
	virtual void drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0) = 0;
	virtual void drawExtractionForItem(QPainter* painter);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

	static void swap(qreal &x, qreal &y);
	static QStringList getPenStyleList();
	static QStringList getBrushStyleList();
	QPen pen() const;
	QBrush brush() const;
	void setBrush(const QBrush& brush);
	void setPen(const QPen& pen);
	void setPenStyle(const QString& text);
	void setPenWidth(int width);
	void setPenColor(const QString& text);
	void setBrushStyle(const QString& text);
	void setBrushColor(const QString& text);
	void setPenBrush(const QString& penStyle, int width, const QString& penColor, const QString& brushStyle, const QString& brushColor);

	void setX1andY1(qreal x, qreal y);
	void setX1andY2(qreal x, qreal y);
	void setX2andY1(qreal x, qreal y);
	void setX2andY2(qreal x, qreal y);
	void setNoneDragState();
	void reshapeRectWithShift();
	virtual void changeDragState(qreal x, qreal y);
	DragState getDragState() const;
	virtual void calcResizeItem(QGraphicsSceneMouseEvent *event);
	virtual void resizeItem(QGraphicsSceneMouseEvent *event);

	//for save to xml
	void setXandY(QDomElement& dom, QRectF const &rect);
	QDomElement setPenBrushToDoc(QDomDocument &document, QString const &domName);
	QRectF sceneBoundingRectCoord(QPointF const &topLeftPicture);
	virtual QPair<QDomElement, Item::DomElementTypes> generateItem(QDomDocument &document, QPointF const &topLeftPicture) = 0;

	void readPenBrush(QDomElement const &docItem);

protected:
	DomElementTypes mDomElementType;
	DragState mDragState;
	QPen mPen;
	QBrush mBrush;
	QRectF mRect;
	qreal mX1;
	qreal mY1;
	qreal mX2;
	qreal mY2;
};
