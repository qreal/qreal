#pragma once

#include <QtGui/QGraphicsItem>
#include <QtGui/QPen>
#include <QtGui/QGraphicsSceneMouseEvent>
#include <QtGui/QPainter>
#include <QtXml/QDomDocument>
#include <QtCore/QPair>
#include <QtCore/QList>

const int drift = 15;
const int resizeDrift = 10;
const int scalingDrift = 10;
const int scalingRect = 6;

class Item : public QGraphicsItem
{
public:
	enum DragState {
		None,
		TopLeft,
		TopRight,
		BottomLeft,
		BottomRight,
		Ctrl
	};
	enum DomElementTypes {
		noneType,
		pictureType,
		labelType,
		portType
	};

	enum ScalingPointState {
		noneScale,
		topLeftX,
		topRightX,
		bottomLeftX,
		bottomRightX,
		topLeftY,
		topRightY,
		bottomLeftY,
		bottomRightY
	};

	QRectF mBoundingRect;
	Item(QGraphicsItem* parent = 0);
	static int sign(int x);
	static qreal length(QPointF const &point1, QPointF const &point2);
	virtual QRectF boundingRect() const = 0;
	virtual QRectF realBoundingRect() const;
	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
	virtual void drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0) = 0;
	virtual void drawExtractionForItem(QPainter* painter);
	virtual void drawFieldForResizeItem(QPainter* painter);
	virtual void drawScalingRects(QPainter* painter);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void setPenBrushDriftRect(QPainter* painter);

	static void swap(qreal &x, qreal &y);
	static QStringList getPenStyleList();
	static QStringList getBrushStyleList();
	QPen pen() const;
	QBrush brush() const;
	void setBrush(const QBrush& brush);
	void setPen(const QPen& pen);
	virtual void setPenStyle(const QString& text);
	virtual void setPenWidth(int width);
	virtual void setPenColor(const QString& text);
	virtual void setBrushStyle(const QString& text);
	virtual void setBrushColor(const QString& text);
	void setPenBrush(const QString& penStyle, int width, const QString& penColor, const QString& brushStyle, const QString& brushColor);

	void setX1andY1(qreal x, qreal y);
	void setX1andY2(qreal x, qreal y);
	void setX2andY1(qreal x, qreal y);
	void setX2andY2(qreal x, qreal y);
	void setNoneDragState();
	void reshapeRectWithShift();
	virtual void changeDragState(qreal x, qreal y);
	DragState getDragState() const;
	void setListScalePoint(QList<QPair<ScalingPointState, QColor> > list);

	void initListScalePoint();
	void calcForChangeScalingState(QPointF const&pos, QPointF const& point1, QPointF const& point2, int const &correction);
	virtual void changeScalingPointState(qreal x, qreal y);
	ScalingPointState getScalingPointState() const;
	QColor changeScaleColor(QPair<Item::ScalingPointState, QColor> point);
	void setScalingPointColor();

	virtual void calcResizeItem(QGraphicsSceneMouseEvent *event);
	virtual void resizeItem(QGraphicsSceneMouseEvent *event);

	//for save to xml
	QString setScaleForDoc(int i, QRectF const &rect);
	QString setSingleScaleForDoc(int i, qreal x, qreal y);
	void setXandY(QDomElement& dom, QRectF const &rect);
	QDomElement setPenBrushToDoc(QDomDocument &document, QString const &domName);
	virtual QRectF sceneBoundingRectCoord(QPoint const &topLeftPicture);
	virtual QPair<QDomElement, Item::DomElementTypes> generateItem(QDomDocument &document, QPoint const &topLeftPicture) = 0;

	void readPenBrush(QDomElement const &docItem);

protected:
	QList<QPair<ScalingPointState, QColor> > mListScalePoint;
	bool mNeedScalingRect;
	DomElementTypes mDomElementType;
	DragState mDragState;
	ScalingPointState mScalingState;
	QPen mPen;
	QBrush mBrush;
	QRectF mRect;
	qreal mX1;
	qreal mY1;
	qreal mX2;
	qreal mY2;
};
