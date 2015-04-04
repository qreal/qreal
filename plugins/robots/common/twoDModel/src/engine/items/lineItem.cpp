#include <QtGui/QPainter>
#include <QtWidgets/QStyle>
#include <QtWidgets/QStyleOptionGraphicsItem>

#include <qrkernel/settingsManager.h>
#include "lineItem.h"
#include "wallItem.h"

using namespace twoDModel::items;
using namespace qReal;
using namespace graphicsUtils;

LineItem::LineItem(const QPointF &begin, const QPointF &end, int cornerRadius)
	: mLineImpl()
	, mCornerRadius(cornerRadius)
	, mCellNumbX1(0)
	, mCellNumbY1(0)
	, mCellNumbX2(0)
	, mCellNumbY2(0)
{
	setX1(begin.x());
	setY1(begin.y());
	setX2(end.x());
	setY2(end.y());
	setFlags(ItemIsSelectable | ItemIsMovable);
	setPrivateData();
}

AbstractItem *LineItem::clone() const
{
	const auto cloned = new LineItem({x1(), y1()}, {x2(), y2()}, mCornerRadius);
	AbstractItem::copyTo(cloned);
	cloned->mCellNumbX1 = mCellNumbX1;
	cloned->mCellNumbY1 = mCellNumbY1;
	cloned->mCellNumbX2 = mCellNumbX2;
	cloned->mCellNumbY2 = mCellNumbY2;
	return cloned;
}

void LineItem::setPrivateData()
{
	QPen pen(this->pen());
	pen.setColor(Qt::green);
	pen.setStyle(Qt::SolidLine);
	setPen(pen);
	mSerializeName = "line";
}

QRectF LineItem::boundingRect() const
{
	return mLineImpl.boundingRect(x1(), y1(), x2(), y2(), pen().width(), drift);
}

void LineItem::drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	mLineImpl.drawItem(painter, x1(), y1(), x2(), y2());
}

void LineItem::drawExtractionForItem(QPainter* painter)
{
	mLineImpl.drawPointExtractionForItem(painter, x1(), y1(), x2(), y2());
	setPenBrushDriftRect(painter);
	mLineImpl.drawExtractionForItem(painter, x1(), y1(), x2(), y2(), drift);
	mLineImpl.drawFieldForResizeItem(painter, resizeDrift, x1(), y1(), x2(), y2());

	painter->setPen(QPen(Qt::red));
	painter->setBrush(QBrush(Qt::SolidPattern));
}

QPainterPath LineItem::shape() const
{
	return mLineImpl.shape(drift, x1(), y1(), x2(), y2());
}

void LineItem::resizeItem(QGraphicsSceneMouseEvent *event)
{
	if (event->modifiers() & Qt::ShiftModifier) {
		setX2(event->scenePos().x());
		setY2(event->scenePos().y());
		reshapeRectWithShift();
	} else {
		if (SettingsManager::value("2dShowGrid").toBool()
				&& (dragState() == TopLeft || dragState() == BottomRight)
				&& dynamic_cast<WallItem *>(this))  /// @todo Oh sh~
		{
			calcResizeItem(event, SettingsManager::value("2dGridCellSize").toInt());
		} else {
			if (dragState() == TopLeft || dragState() == BottomRight) {
				AbstractItem::resizeItem(event);
			} else {
				setFlag(QGraphicsItem::ItemIsMovable, true);
			}
		}
	}
}

void LineItem::calcResizeItem(QGraphicsSceneMouseEvent *event, int indexGrid)
{
	const qreal x = mapFromScene(event->scenePos()).x();
	const qreal y = mapFromScene(event->scenePos()).y();
	if (dragState() != None) {
		setFlag(QGraphicsItem::ItemIsMovable, false);
	}
	if (dragState() == TopLeft) {
		setX1(x);
		setY1(y);
		resizeBeginWithGrid(indexGrid);
	} else if (dragState() == BottomRight) {
		setX2(x);
		setY2(y);
		reshapeEndWithGrid(indexGrid);
	}
}

void LineItem::reshapeRectWithShift()
{
	qreal differenceX = abs(x2() - x1());
	qreal differenceY = abs(y2() - y1());
	qreal differenceXY = abs(differenceX - differenceY);
	qreal size = qMax(differenceX, differenceY);
	const int delta = size / 2;
	if (differenceXY > delta) {
		QPair<qreal, qreal> res = mLineImpl.reshapeRectWithShiftForLine(x1(), y1(), x2(), y2()
				, differenceX, differenceY, size);
		setX2(res.first);
		setY2(res.second);
	} else
		AbstractItem::reshapeRectWithShift();
}

void LineItem::resizeBeginWithGrid(int indexGrid)
{
	const int coefX = static_cast<int>(x1()) / indexGrid;
	const int coefY = static_cast<int>(y1()) / indexGrid;

	setX1(alignedCoordinate(x1(), coefX, indexGrid));
	setY1(alignedCoordinate(y1(), coefY, indexGrid));

	mCellNumbX1 = x1() / indexGrid;
	mCellNumbY1 = y1() / indexGrid;
}

void LineItem::reshapeEndWithGrid(int indexGrid)
{
	const int coefX = static_cast<int>(x2()) / indexGrid;
	const int coefY = static_cast<int>(y2()) / indexGrid;

	setX2(alignedCoordinate(x2(), coefX, indexGrid));
	setY2(alignedCoordinate(y2(), coefY, indexGrid));

	mCellNumbX2 = x2() / indexGrid;
	mCellNumbY2 = y2() / indexGrid;
}

void LineItem::reshapeBeginWithGrid(int indexGrid)
{
	const int coefX = static_cast<int> (x1()) / indexGrid;
	const int coefY = static_cast<int> (y1()) / indexGrid;
	setX1(alignedCoordinate(x1(), coefX, indexGrid));
	setY1(alignedCoordinate(y1(), coefY, indexGrid));
	mCellNumbX1 = x1() / indexGrid;
	mCellNumbY1 = y1() / indexGrid;
}

void LineItem::alignTheWall(int indexGrid)
{
	if (x1() != x2() && y1() != y2()) {
		countCellNumbCoordinates(indexGrid);
	}

	setBeginCoordinatesWithGrid(indexGrid);
	setEndCoordinatesWithGrid(indexGrid);
}

void LineItem::countCellNumbCoordinates(int indexGrid)
{
	mCellNumbX1 = x1() / indexGrid;
	mCellNumbY1 = y1() / indexGrid;

	if (qAbs(y2() - y1()) > qAbs(x2() - x1())) {
		mCellNumbX2 = mCellNumbX1;
		mCellNumbY2 = y2() / indexGrid;
	} else {
		mCellNumbX2 = x2() / indexGrid;
		mCellNumbY2 = mCellNumbY1;
	}
}

void LineItem::setBeginCoordinatesWithGrid(int indexGrid)
{
	setX1(mCellNumbX1 * indexGrid);
	setY1(mCellNumbY1 * indexGrid);
}

void LineItem::setEndCoordinatesWithGrid(int indexGrid)
{
	setX2(mCellNumbX2 * indexGrid);
	setY2(mCellNumbY2 * indexGrid);
}

void LineItem::setDraggedEnd(qreal x, qreal y)
{
	setX2(x1() - x);
	setY2(y1() - y);
}

qreal LineItem::alignedCoordinate(qreal coord, int coef, const int indexGrid) const
{
	const int coefSign = coef ? coef / qAbs(coef) : 0;

	if (qAbs(qAbs(coord) - qAbs(coef) * indexGrid) <= indexGrid / 2) {
		return coef * indexGrid;
	} else if (qAbs(qAbs(coord) - (qAbs(coef) + 1) * indexGrid) <= indexGrid / 2) {
		return (coef + coefSign) * indexGrid;
	}

	return coord;
}

QDomElement LineItem::serialize(QDomDocument &document, const QPoint &topLeftPicture)
{
	QDomElement lineNode = setPenBrushToDoc(document, mSerializeName);
	AbstractItem::serialize(lineNode);
	lineNode.setAttribute("begin", QString::number(x1() + scenePos().x() - topLeftPicture.x())
			+ ":" + QString::number(y1() + scenePos().y() - topLeftPicture.y()));
	lineNode.setAttribute("end", QString::number(x2() + scenePos().x() - topLeftPicture.x())
			+ ":" + QString::number(y2() + scenePos().y() - topLeftPicture.y()));
	return lineNode;
}

void LineItem::deserialize(const QDomElement &element)
{
	AbstractItem::deserialize(element);
	const QString beginStr = element.attribute("begin", "0:0");
	QStringList splittedStr = beginStr.split(":");
	int x = splittedStr[0].toInt();
	int y = splittedStr[1].toInt();
	const QPointF begin = QPointF(x, y);

	const QString endStr = element.attribute("end", "0:0");
	splittedStr = endStr.split(":");
	x = splittedStr[0].toInt();
	y = splittedStr[1].toInt();
	const QPointF end = QPointF(x, y);

	setX1(begin.x());
	setY1(begin.y());
	setX2(end.x());
	setY2(end.y());

	deserializePenBrush(element);
}

void LineItem::deserializePenBrush(const QDomElement &element)
{
	readPenBrush(element);
}

void LineItem::setSerializeName(const QString &name)
{
	mSerializeName = name;
}
