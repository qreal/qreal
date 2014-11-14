#include <QtGui/QPainter>
#include <QtWidgets/QStyle>
#include <QtWidgets/QStyleOptionGraphicsItem>

#include <qrkernel/settingsManager.h>
#include "lineItem.h"
#include "wallItem.h"

using namespace twoDModel::items;
using namespace qReal;
using namespace graphicsUtils;

LineItem::LineItem(QPointF const &begin, QPointF const &end, int cornerRadius)
	: mLineImpl()
	, mCornerRadius(cornerRadius)
	, mCellNumbX1(0)
	, mCellNumbY1(0)
	, mCellNumbX2(0)
	, mCellNumbY2(0)
{
	mX1 = begin.x();
	mY1 = begin.y();
	mX2 = end.x();
	mY2 = end.y();
	setFlags(ItemIsSelectable | ItemIsMovable);
	setPrivateData();
}

void LineItem::setPrivateData()
{
	mPen.setColor(Qt::green);
	mPen.setStyle(Qt::SolidLine);
	mSerializeName = "line";
}

QRectF LineItem::boundingRect() const
{
	return mLineImpl.boundingRect(mX1, mY1, mX2, mY2, mPen.width(), drift);
}

void LineItem::drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	mLineImpl.drawItem(painter, mX1, mY1, mX2, mY2);
}

void LineItem::drawExtractionForItem(QPainter* painter)
{
	mLineImpl.drawPointExtractionForItem(painter, mX1, mY1, mX2, mY2);
	setPenBrushDriftRect(painter);
	mLineImpl.drawExtractionForItem(painter, mX1, mY1, mX2, mY2, drift);
	mLineImpl.drawFieldForResizeItem(painter, resizeDrift, mX1, mY1, mX2, mY2);

	painter->setPen(QPen(Qt::red));
	painter->setBrush(QBrush(Qt::SolidPattern));
}

QPainterPath LineItem::shape() const
{
	return mLineImpl.shape(drift, mX1, mY1, mX2, mY2);
}

void LineItem::resizeItem(QGraphicsSceneMouseEvent *event)
{
	if (event->modifiers() & Qt::ShiftModifier) {
		mX2=event->scenePos().x();
		mY2=event->scenePos().y();
		reshapeRectWithShift();
	} else {
		if (SettingsManager::value("2dShowGrid").toBool()
				&& (mDragState == TopLeft || mDragState == BottomRight)
				&& dynamic_cast<WallItem *>(this))
		{
			calcResizeItem(event, SettingsManager::value("2dGridCellSize").toInt());
		} else {
			if (mDragState == TopLeft || mDragState == BottomRight) {
				AbstractItem::resizeItem(event);
			} else {
				setFlag(QGraphicsItem::ItemIsMovable, true);
			}
		}
	}
}

void LineItem::calcResizeItem(QGraphicsSceneMouseEvent *event, int indexGrid)
{
	qreal const x = mapFromScene(event->scenePos()).x();
	qreal const y = mapFromScene(event->scenePos()).y();
	if (mDragState != None) {
		setFlag(QGraphicsItem::ItemIsMovable, false);
	}
	if (mDragState == TopLeft) {
		mX1 = x;
		mY1 = y;
		resizeBeginWithGrid(indexGrid);
	} else if (mDragState == BottomRight) {
		mX2 = x;
		mY2 = y;
		reshapeEndWithGrid(indexGrid);
	}
}

void LineItem::reshapeRectWithShift()
{
	qreal differenceX = abs(mX2 - mX1);
	qreal differenceY = abs(mY2 - mY1);
	qreal differenceXY = abs(differenceX - differenceY);
	qreal size = qMax(differenceX, differenceY);
	const int delta = size / 2;
	if (differenceXY > delta) {
		QPair<qreal, qreal> res = mLineImpl.reshapeRectWithShiftForLine(mX1, mY1, mX2, mY2, differenceX, differenceY, size);
		setX2andY2(res.first, res.second);
	} else
		AbstractItem::reshapeRectWithShift();
}

void LineItem::resizeBeginWithGrid(int indexGrid)
{
	int const coefX = static_cast<int>(mX1) / indexGrid;
	int const coefY = static_cast<int>(mY1) / indexGrid;

	if (qAbs(mY2 - mY1) > qAbs(mX2 - mX1)) {
		setX1andY1(mX2, alignedCoordinate(mY1, coefY, indexGrid));
	} else {
		setX1andY1(alignedCoordinate(mX1, coefX, indexGrid), mY2);
	}

	mCellNumbX1 = mX1 / indexGrid;
	mCellNumbY1 = mY1 / indexGrid;
}

void LineItem::reshapeEndWithGrid(int indexGrid)
{
	int const coefX = static_cast<int>(mX2) / indexGrid;
	int const coefY = static_cast<int>(mY2) / indexGrid;

	if (qAbs(mY2 - mY1) > qAbs(mX2 - mX1)) {
		setX2andY2(mX1, alignedCoordinate(mY2, coefY, indexGrid));
	} else {
		setX2andY2(alignedCoordinate(mX2, coefX, indexGrid), mY1);
	}

	mCellNumbX2 = mX2 / indexGrid;
	mCellNumbY2 = mY2 / indexGrid;
}

void LineItem::reshapeBeginWithGrid(int indexGrid)
{
	int const coefX = static_cast<int> (mX1) / indexGrid;
	int const coefY = static_cast<int> (mY1) / indexGrid;
	setX1andY1(alignedCoordinate(mX1, coefX, indexGrid), alignedCoordinate(mY1, coefY, indexGrid));
	mCellNumbX1 = mX1 / indexGrid;
	mCellNumbY1 = mY1 / indexGrid;
}

void LineItem::alignTheWall(int indexGrid)
{
	//countCellNumbCoordinates(indexGrid);
	setBeginCoordinatesWithGrid(indexGrid);
	setEndCoordinatesWithGrid(indexGrid);
}

//void LineItem::countCellNumbCoordinates(int indexGrid)
//{
//	mCellNumbX1 = mX1 / indexGrid;
//	mCellNumbY1 = mY1 / indexGrid;

//	if (qAbs(mY2 - mY1) > qAbs(mX2 - mX1)) {
//		mCellNumbX2 = mCellNumbX1;
//		mCellNumbY2 = mY2 / indexGrid;
//	} else {
//		mCellNumbX2 = mX2 / indexGrid;
//		mCellNumbY2 = mCellNumbY1;
//	}
//}

void LineItem::setBeginCoordinatesWithGrid(int indexGrid)
{
	setX1andY1(mCellNumbX1 * indexGrid, mCellNumbY1 * indexGrid);
}

void LineItem::setEndCoordinatesWithGrid(int indexGrid)
{
	setX2andY2(mCellNumbX2 * indexGrid, mCellNumbY2 * indexGrid);
}

void LineItem::setDraggedEnd(qreal x, qreal y)
{
	setX2andY2(mX1 - x, mY1 - y);
}

qreal LineItem::alignedCoordinate(qreal coord, int coef, int const indexGrid) const
{
	int const coefSign = coef ? coef / qAbs(coef) : 0;

	if (qAbs(qAbs(coord) - qAbs(coef) * indexGrid) <= indexGrid / 2) {
		return coef * indexGrid;
	} else if (qAbs(qAbs(coord) - (qAbs(coef) + 1) * indexGrid) <= indexGrid / 2) {
		return (coef + coefSign) * indexGrid;
	}

	return coord;
}

QDomElement LineItem::serialize(QDomDocument &document, QPoint const &topLeftPicture)
{
	QDomElement lineNode = setPenBrushToDoc(document, mSerializeName);
	lineNode.setAttribute("begin", QString::number(mX1 + scenePos().x() - topLeftPicture.x())
			+ ":" + QString::number(mY1 + scenePos().y() - topLeftPicture.y()));
	lineNode.setAttribute("end", QString::number(mX2 + scenePos().x() - topLeftPicture.x())
			+ ":" + QString::number(mY2 + scenePos().y() - topLeftPicture.y()));
	return lineNode;
}

void LineItem::deserialize(QDomElement const &element)
{
	QString const beginStr = element.attribute("begin", "0:0");
	QStringList splittedStr = beginStr.split(":");
	int x = splittedStr[0].toInt();
	int y = splittedStr[1].toInt();
	QPointF const begin = QPointF(x, y);

	QString const endStr = element.attribute("end", "0:0");
	splittedStr = endStr.split(":");
	x = splittedStr[0].toInt();
	y = splittedStr[1].toInt();
	QPointF const end = QPointF(x, y);

	mX1 = begin.x();
	mY1 = begin.y();
	mX2 = end.x();
	mY2 = end.y();

	deserializePenBrush(element);
}

void LineItem::deserializePenBrush(QDomElement const &element)
{
	readPenBrush(element);
}

void LineItem::setSerializeName(QString const &name)
{
	mSerializeName = name;
}
