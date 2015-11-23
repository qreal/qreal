/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "abstractItem.h"

#include <QtGui/QPainter>
#include <QtWidgets/QMenu>
#include <QtWidgets/QStyle>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QStyleOptionGraphicsItem>
#include <QtWidgets/QGraphicsSceneMouseEvent>

using namespace graphicsUtils;

AbstractItem::AbstractItem(QGraphicsItem* parent)
	: QGraphicsObject(parent)
	, mDragState(None)
	, mX1(0)
	, mY1(0)
	, mX2(0)
	, mY2(0)
	, mEditable(true)
{
	setFlags(ItemIsSelectable | ItemIsMovable | ItemSendsGeometryChanges);
	mBrush.setColor(mPen.color());
}

QRectF AbstractItem::calcNecessaryBoundingRect() const
{
	return boundingRect();
}

QRectF AbstractItem::realBoundingRect() const
{
	return mapToScene(calcNecessaryBoundingRect()).boundingRect();
}

QPainterPath AbstractItem::realShape() const
{
	return mapToScene(shape());
}

void AbstractItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(widget);
	painter->setPen(mPen);
	painter->setBrush(mBrush);
	drawItem(painter, option, widget);
	if (option->state & QStyle::State_Selected) {
		painter->save();
		setPenBrushForExtraction(painter, option);
		drawExtractionForItem(painter);
		painter->restore();
	}
}

void AbstractItem::drawExtractionForItem(QPainter* painter)
{
	painter->drawPoint(x1(), y1());
	painter->drawPoint(x1(), y2());
	painter->drawPoint(x2(), y1());
	painter->drawPoint(x2(), y2());
	drawFieldForResizeItem(painter);
}

void AbstractItem::drawFieldForResizeItem(QPainter* painter)
{
	QRectF itemBoundingRect = calcNecessaryBoundingRect();
	const qreal x1 = itemBoundingRect.left();
	const qreal x2 = itemBoundingRect.right();
	const qreal y1 = itemBoundingRect.top();
	const qreal y2 = itemBoundingRect.bottom();

	setPenBrushDriftRect(painter);
	painter->drawRect(x1, y1, resizeDrift, resizeDrift);
	painter->drawRect(x2 - resizeDrift, y2 - resizeDrift, resizeDrift, resizeDrift);
	painter->drawRect(x1, y2 - resizeDrift, resizeDrift, resizeDrift);
	painter->drawRect(x2 - resizeDrift, y1, resizeDrift, resizeDrift);
}

void AbstractItem::setPenBrushForExtraction(QPainter *painter, const QStyleOptionGraphicsItem *option)
{
	Q_UNUSED(option);
	QPen pen(Qt::red);
	pen.setWidth(3);
	painter->setPen(pen);
}

void AbstractItem::setPenBrushDriftRect(QPainter *painter)
{
	QPen pen(QColor("whitesmoke"));
	pen.setStyle(Qt::SolidLine);
	pen.setWidth(0);
	QBrush brush;
	brush.setStyle(Qt::NoBrush);
	brush.setColor(Qt::white);
	painter->setPen(pen);
	painter->setBrush(brush);
}

QPen AbstractItem::pen() const
{
	return mPen;
}

QBrush AbstractItem::brush() const
{
	return mBrush;
}

void AbstractItem::setBrush(const QBrush &brush)
{
	mBrush = brush;
	emit brushChanged(mBrush);
}

void AbstractItem::setPen(const QPen &pen)
{
	mPen = pen;
	emit penChanged(pen);
}

void AbstractItem::setCoordinates(const QRectF &pos)
{
	setX1(pos.left());
	setY1(pos.top());
	setX2(pos.right());
	setY2(pos.bottom());
	update();
}

void AbstractItem::reshapeRectWithShift()
{
	const qreal size = qMax(qAbs(x2() - x1()), qAbs(y2() - y1()));
	setX2(x2() > x1() ? x1() + size : x1() - size);
	setY2(y2() > y1() ? y1() + size : y1() - size);
}

void AbstractItem::changeDragState(qreal x, qreal y)
{
	if (QRectF(mapToScene(x1(), y1()), QSizeF(0, 0)).adjusted(-resizeDrift, -resizeDrift, resizeDrift
			, resizeDrift).contains(QPointF(x, y)))
	{
		mDragState = TopLeft;
	} else if (QRectF(mapToScene(x2(), y2()), QSizeF(0, 0)).adjusted(-resizeDrift, -resizeDrift, resizeDrift
			, resizeDrift).contains(QPointF(x, y)))
	{
		mDragState = BottomRight;
	} else if (QRectF(mapToScene(x2(), y1()), QSizeF(0, 0)).adjusted(-resizeDrift, -resizeDrift, resizeDrift
			, resizeDrift).contains(QPointF(x, y)))
	{
		mDragState = TopRight;
	} else if (QRectF(mapToScene(x1(), y2()), QSizeF(0, 0)).adjusted(-resizeDrift, -resizeDrift, resizeDrift
			, resizeDrift).contains(QPointF(x, y)))
	{
		mDragState = BottomLeft;
	} else {
		mDragState = None;
	}
}

AbstractItem::DragState AbstractItem::dragState() const
{
	return mDragState;
}

void AbstractItem::setDragState(AbstractItem::DragState dragState)
{
	mDragState = dragState;
}

void AbstractItem::calcResizeItem(QGraphicsSceneMouseEvent *event)
{
	const qreal x = mapFromScene(event->scenePos()).x();
	const qreal y = mapFromScene(event->scenePos()).y();
	if (mDragState != None) {
		setFlag(QGraphicsItem::ItemIsMovable, false);
	}

	if (mDragState == TopLeft) {
		setX1(x);
		setY1(y);
	} else if (mDragState == TopRight) {
		setX2(x);
		setY1(y);
	} else if (mDragState == BottomLeft) {
		setX1(x);
		setY2(y);
	} else if (mDragState == BottomRight) {
		setX2(x);
		setY2(y);
	}
}

void AbstractItem::resizeItem(QGraphicsSceneMouseEvent *event)
{
	if (mDragState != None) {
		calcResizeItem(event);
	} else {
		setFlag(QGraphicsItem::ItemIsMovable, true);
	}
}

void AbstractItem::reverseOldResizingItem(const QPointF &begin, const QPointF &end)
{
	if (mDragState == TopLeft) {
		setX1(begin.x());
		setY1(begin.y());
	} else if (mDragState == TopRight) {
		setX2(end.x());
		setY1(begin.y());
	} else if (mDragState == BottomLeft) {
		setX1(begin.x());
		setY2(end.y());
	} else if (mDragState == BottomRight) {
		setX2(end.x());
		setY2(end.y());
	}
}

void AbstractItem::setPenStyle(const QString &text)
{
	if (text == "Solid") {
		mPen.setStyle(Qt::SolidLine);
	} else if (text == "Dot") {
		mPen.setStyle(Qt::DotLine);
	} else if (text == "Dash") {
		mPen.setStyle(Qt::DashLine);
	} else if (text == "DashDot") {
		mPen.setStyle(Qt::DashDotLine);
	} else if (text == "DashDotDot") {
		mPen.setStyle(Qt::DashDotDotLine);
	} else if (text == "None") {
		mPen.setStyle(Qt::NoPen);
	}

	emit penChanged(mPen);
}

void AbstractItem::setPenWidth(int width)
{
	mPen.setWidth(width);
	emit penChanged(mPen);
}

void AbstractItem::setPenColor(const QString &text)
{
	mPen.setColor(QColor(text));
	emit penChanged(mPen);
}

void AbstractItem::setBrushStyle(const QString &text)
{
	if (text == "Solid") {
		mBrush.setStyle(Qt::SolidPattern);
	} else if (text == "None") {
		mBrush.setStyle(Qt::NoBrush);
	}

	emit brushChanged(mBrush);
}

void AbstractItem::setBrushColor(const QString &text)
{
	mBrush.setColor(QColor(text));
	emit brushChanged(mBrush);
}

void AbstractItem::setPen(const QString &penStyle, int width, const QString &penColor)
{
	setPenStyle(penStyle);
	setPenWidth(width);
	setPenColor(penColor);
}

void AbstractItem::setBrush(const QString &brushStyle, const QString &brushColor)
{
	setBrushStyle(brushStyle);
	setBrushColor(brushColor);
}

void AbstractItem::setPenBrush(const QString &penStyle, int width, const QString &penColor
		, const QString &brushStyle, const QString &brushColor)
{
	setPen(penStyle, width, penColor);
	setBrush(brushStyle, brushColor);
}

qreal AbstractItem::x1() const
{
	return mX1;
}

qreal AbstractItem::x2() const
{
	return mX2;
}

qreal AbstractItem::y1() const
{
	return mY1;
}

qreal AbstractItem::y2() const
{
	return mY2;
}

void AbstractItem::setX1(qreal x1)
{
	mX1 = x1;
	emit x1Changed(x1);
}

void AbstractItem::setY1(qreal y1)
{
	mY1 = y1;
	emit y1Changed(y1);
}

void AbstractItem::setX2(qreal x2)
{
	mX2 = x2;
	emit x2Changed(x2);
}

void AbstractItem::setY2(qreal y2)
{
	mY2 = y2;
	emit y2Changed(y2);
}

void AbstractItem::setXandY(QDomElement& dom, const QRectF &rect)
{
	dom.setAttribute("y1", QString::number(rect.top()));
	dom.setAttribute("x1", QString::number(rect.left()));
	dom.setAttribute("y2", QString::number(rect.bottom()));
	dom.setAttribute("x2", QString::number(rect.right()));
}

QDomElement AbstractItem::setPenBrushToDoc(QDomDocument &document, const QString &domName) const
{
	QDomElement dom = document.createElement(domName);
	dom.setAttribute("fill", mBrush.color().name(QColor::HexArgb));

	if (mBrush.style() == Qt::NoBrush) {
		dom.setAttribute("fill-style", "none");
	}

	if (mBrush.style() == Qt::SolidPattern) {
		dom.setAttribute("fill-style", "solid");
	}

	dom.setAttribute("stroke", mPen.color().name(QColor::HexArgb));
	dom.setAttribute("stroke-width", mPen.width());

	QString penStyle;
	switch (mPen.style()) {
	case Qt::SolidLine:
		penStyle = "solid";
		break;
	case Qt::DotLine:
		penStyle = "dot";
		break;
	case Qt::DashLine:
		penStyle = "dash";
		break;
	case Qt::DashDotLine:
		penStyle =  "dashdot";
		break;
	case Qt::DashDotDotLine:
		penStyle = "dashdotdot";
		break;
	case Qt::NoPen:
		penStyle = "none";
		break;
	default:
		break;
	}

	dom.setAttribute("stroke-style", penStyle);

	return dom;
}

QRectF AbstractItem::sceneBoundingRectCoord(const QPoint &topLeftPicture)
{
	QRectF itemBoundingRect = calcNecessaryBoundingRect();
	const qreal x1 = scenePos().x() + itemBoundingRect.x() - topLeftPicture.x();
	const qreal y1 = scenePos().y() + itemBoundingRect.y() - topLeftPicture.y();
	return QRectF(x1, y1, itemBoundingRect.width(), itemBoundingRect.height());
}

void AbstractItem::readPenBrush(const QDomElement &docItem)
{
	QString brushStyle = docItem.attribute("fill-style", "");
	if (brushStyle == "solid") {
		mBrush.setStyle(Qt::SolidPattern);
	} else if (brushStyle == "none") {
		mBrush.setStyle(Qt::NoBrush);
	}

	mBrush.setColor(QColor(docItem.attribute("fill", "")));
	mPen.setColor(QColor(docItem.attribute("stroke", "")));
	mPen.setWidth((docItem.attribute("stroke-width", "")).toDouble());

	QString penStyle = docItem.attribute("stroke-style", "");
	if (penStyle == "solid") {
		mPen.setStyle(Qt::SolidLine);
	} else if (penStyle == "dot") {
		mPen.setStyle(Qt::DotLine);
	} else if (penStyle == "dash") {
		mPen.setStyle(Qt::DashLine);
	} else if (penStyle == "dashdot") {
		mPen.setStyle(Qt::DashDotLine);
	} else if (penStyle == "dashdotdot") {
		mPen.setStyle(Qt::DashDotDotLine);
	} else if (penStyle == "none") {
		mPen.setStyle(Qt::NoPen);
	}

	emit penChanged(mPen);
}

QStringList AbstractItem::getPenStyleList()
{
	return { "Solid", "Dot", "Dash", "DashDot", "DashDotDot", "None" };
}

QStringList AbstractItem::getBrushStyleList()
{
	return { "None", "Solid" };
}

void AbstractItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	// When selecting a robot item on the scene than display widget may appear.
	// After that scene would be shrinked and mouse move event would be generated (but actually
	// mouse cursor is not moved). Because of that selected robot item moves which should not be because you
	// only select robot item and not move it. It also happens when you unselect robot item by selecting another
	// item (not only robot item). In this case new selected item moves.
	if (event->lastScreenPos() != event->screenPos()) {
		QGraphicsItem::mouseMoveEvent(event);
	}
}

QString AbstractItem::id() const
{
	return mId;
}

void AbstractItem::setId(const QString &id)
{
	mId = id;
}

void AbstractItem::setEditable(bool editable)
{
	mEditable = editable;
	setFlag(QGraphicsItem::ItemIsMovable, editable);
}

bool AbstractItem::editable() const
{
	return mEditable;
}

void AbstractItem::serialize(QDomElement &element) const
{
	element.setAttribute("id", id());
}

void AbstractItem::deserialize(const QDomElement &element)
{
	setId(element.attribute("id", id()));
}

void AbstractItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	if (!mEditable) {
		return;
	}

	if (!isSelected()) {
		scene()->clearSelection();
		setSelected(true);
	}

	event->accept();
	QMenu *menu = new QMenu();
	QAction *removeAction = menu->addAction(QObject::tr("Remove"));
	QAction *selectedAction = menu->exec(event->screenPos());
	delete menu;
	if (selectedAction == removeAction) {
		emit deletedWithContextMenu();
	}
}

QVariant AbstractItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
	if (change == QGraphicsItem::ItemPositionChange) {
		emit positionChanged(value.toPointF());
	}

	return QGraphicsItem::itemChange(change, value);
}

void AbstractItem::copyTo(AbstractItem * const other) const
{
	other->mDragState = mDragState;
	other->mPen = mPen;
	other->mBrush = mBrush;
	other->mX1 = mX1;
	other->mY1 = mY1;
	other->mX2 = mX2;
	other->mY2 = mY2;
	other->mEditable = mEditable;
	other->setPos(pos());
	connect(this, &AbstractItem::positionChanged
			, other, static_cast<void(AbstractItem::*)(const QPointF &)>(&AbstractItem::setPos));
	connect(this, &AbstractItem::x1Changed, other, &AbstractItem::setX1);
	connect(this, &AbstractItem::y1Changed, other, &AbstractItem::setY1);
	connect(this, &AbstractItem::x2Changed, other, &AbstractItem::setX2);
	connect(this, &AbstractItem::y2Changed, other, &AbstractItem::setY2);
	connect(this, &AbstractItem::penChanged, other
			, static_cast<void(AbstractItem::*)(const QPen &)>(&AbstractItem::setPen));
	connect(this, &AbstractItem::brushChanged, other
			, static_cast<void(AbstractItem::*)(const QBrush &)>(&AbstractItem::setBrush));
}
