#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsSceneMouseEvent>
#include <QtGui/QKeyEvent>
#include <QtGui/QPainter>

#include "tool.h"
#include "root.h"
#include "propertyManager.h"
#include "../private/toolFactory.h"

using namespace qReal::widgetsEdit;

Tool::Tool(QWidget *widget, ToolController *controller, QGraphicsItem *parent)
	: QGraphicsProxyWidget(parent)
	, mMovable(true), mResizable(true)
	, mController(controller)
	, mSelected(false)
	, mPropertyManager(NULL)
{
	setWidget(widget);
	setTransparent(true);
}

void Tool::onLoaded()
{
	mPropertyManager = new PropertyManager(this);
	connect(this, SIGNAL(propertyChanged(QString,QVariant))
		, mPropertyManager, SLOT(changeProperty(QString,QVariant)));
	emit loaded();
}

QIcon Tool::icon() const
{
	return mIcon;
}

QString Tool::title() const
{
	return mTitle;
}

bool Tool::movable() const
{
	return mMovable;
}

void Tool::setMovable(bool movable)
{
	mMovable = movable;
}

void Tool::setResizable(bool resizable)
{
	mResizable = resizable;
}

void Tool::select()
{
	mSelected = true;
	update(boundingRect());
}

void Tool::unselect()
{
	mSelected = false;
	update(boundingRect());
}

bool Tool::selected() const
{
	return mSelected;
}

PropertyManager *Tool::propertyManager() const
{
	return mPropertyManager;
}

void Tool::paint(QPainter *painter, const QStyleOptionGraphicsItem *option
		, QWidget *widget)
{
	QGraphicsProxyWidget::paint(painter, option, widget);
	if (mSelected) {
		drawFieldForResizeItem(painter);
	}
}

void Tool::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsProxyWidget::mousePressEvent(event);
	event->accept();
	mController->select(this);
	mClickPos = event->pos();
	mDragState = getDragState(mClickPos);
}

void Tool::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if ((resizing() && !mResizable) || (moving() && !mMovable)) {
		return;
	}
	if (resizing()) {
		resizeTool(event);
	}
	if (moving()) {
		moveTool(event);
	}
	QGraphicsProxyWidget::mouseMoveEvent(event);
	event->accept();
}

void Tool::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsProxyWidget::mouseReleaseEvent(event);
	event->accept();
	mDragState = None;
}

void Tool::keyPressEvent(QKeyEvent *event)
{
	mController->processKeyEvent(event);
	event->accept();
}

void Tool::moveEvent(QGraphicsSceneMoveEvent *event)
{
	//geometry() method returns old geometry (when moving it
	// doesn`t refresh geometry on time)
	QRect newGeometry(event->newPos().toPoint(), this->size().toSize());
	emit propertyChanged("geometry", QVariant(newGeometry));
	event->accept();
}

void Tool::resizeEvent(QGraphicsSceneResizeEvent *event)
{
	emit propertyChanged("geometry", QVariant(geometry()));
	event->accept();
}

void Tool::generateXml(QDomElement &element, QDomDocument &document)
{
	foreach(QGraphicsItem *item, childItems()) {
		Tool *child = dynamic_cast<Tool *>(item);
		if (child) {
			QString tagName = ToolFactory::instance()->toolTitleToTagName(child->title());
			QDomElement childElement = document.createElement(tagName);
			element.appendChild(childElement);
			child->generateXml(childElement, document);
		}
	}
	//important to invoke it here because children must be generated before
	if (mPropertyManager) {
		mPropertyManager->generateXml(element, document);
	}
}

void Tool::deserializeWidget(QWidget *parent, const QDomElement &element)
{
	for (int i = 0; i < element.childNodes().count(); ++i) {
		QDomNode const node = element.childNodes().at(i);
		QDomElement const childElem = node.toElement();
		QString const type = childElem.tagName();
		if (type == "property") {
			mPropertyManager->deserializeProperty(childElem);
		}
		if (type == "layoutAttachedProperty") {
			LayoutTool::deserializeAttachedProperty(parent, widget(), childElem);
		}
	}
}

void Tool::load(LayoutTool *parent, QDomElement const &element)
{
	if (parent) {
		setParentItem(parent);
	}
	for (int i = 0; i < element.childNodes().count(); ++i) {
		QDomNode const node = element.childNodes().at(i);
		QDomElement const childElem = node.toElement();
		QString const type = childElem.tagName();
		if (type == "property") {
			mPropertyManager->deserializeProperty(childElem);
		}
		if (type == "layoutAttachedProperty" && parent) {
			parent->loadAttachedProperty(this, childElem);
		}
	}
}

QRectF Tool::resizeRect(DragState state)
{
	QRectF itemBoundingRect = boundingRect();
	qreal const x1 = itemBoundingRect.left();
	qreal const x2 = itemBoundingRect.right();
	qreal const y1 = itemBoundingRect.top();
	qreal const y2 = itemBoundingRect.bottom();

	switch (state) {
	case TopLeft:
		return QRectF(x1, y1, resizeDrift, resizeDrift);
	case Top:
		return QRectF((x1 + x2 - resizeDrift) / 2, y1, resizeDrift, resizeDrift);
	case TopRight:
		return QRectF(x2 - resizeDrift, y1, resizeDrift, resizeDrift);
	case Left:
		return QRectF(x1, (y1 + y2 - resizeDrift) / 2, resizeDrift, resizeDrift);
	case Right:
		return QRectF(x2 - resizeDrift, (y1 + y2 - resizeDrift) / 2, resizeDrift, resizeDrift);
	case BottomLeft:
		return QRectF(x1, y2 - resizeDrift, resizeDrift, resizeDrift);
	case Bottom:
		return QRectF((x1 + x2 - resizeDrift) / 2, y2 - resizeDrift, resizeDrift, resizeDrift);
	case BottomRight:
		return QRectF(x2 - resizeDrift, y2 - resizeDrift, resizeDrift, resizeDrift);
	default:
		return QRectF(0, 0, 0, 0);
	}
}

void Tool::drawFieldForResizeItem(QPainter* painter)
{
	painter->save();
	QColor const color(mResizable ? Qt::blue : Qt::gray);
	painter->setPen(color);
	painter->setBrush(color);

	painter->drawRect(resizeRect(TopLeft));
	painter->drawRect(resizeRect(Top));
	painter->drawRect(resizeRect(TopRight));
	painter->drawRect(resizeRect(Left));
	painter->drawRect(resizeRect(Right));
	painter->drawRect(resizeRect(BottomLeft));
	painter->drawRect(resizeRect(Bottom));
	painter->drawRect(resizeRect(BottomRight));
	painter->restore();
}

Tool::DragState Tool::getDragState(QPointF pos)
{
	if (resizeRect(TopLeft).contains(pos)) {
		return TopLeft;
	}
	if (resizeRect(Top).contains(pos)) {
		return Top;
	}
	if (resizeRect(TopRight).contains(pos)) {
		return TopRight;
	}
	if (resizeRect(Left).contains(pos)) {
		return Left;
	}
	if (resizeRect(Right).contains(pos)) {
		return Right;
	}
	if (resizeRect(BottomLeft).contains(pos)) {
		return BottomLeft;
	}
	if (resizeRect(Bottom).contains(pos)) {
		return Bottom;
	}
	if (resizeRect(BottomRight).contains(pos)) {
		return BottomRight;
	}
	return Moving;
}

bool Tool::moving() const
{
	return mDragState == Moving;
}

bool Tool::resizing() const
{
	return mDragState == TopLeft
		|| mDragState == Top
		|| mDragState == TopRight
		|| mDragState == Left
		|| mDragState == Right
		|| mDragState == BottomLeft
		|| mDragState == Bottom
		|| mDragState == BottomRight;
}

void Tool::moveTool(QGraphicsSceneMouseEvent *event)
{
	QPointF const hotSpot(event->pos());

	QByteArray data;
	QDataStream dataStream(&data, QIODevice::WriteOnly);
	dataStream << title() << hotSpot;

	QMimeData *mimeData = new QMimeData;
	mimeData->setData("application/x-qreal/widgetEditor", data);

	QDrag *drag = new QDrag(event->widget());
	drag->setMimeData(mimeData);
	drag->setHotSpot(hotSpot.toPoint());

	QPixmap pixmap(rect().toRect().width()
		, rect().toRect().height());
	QPainter *painter = new QPainter(&pixmap);
	scene()->render(painter, rect(), sceneBoundingRect());
	if (!pixmap.isNull()) {
		drag->setPixmap(pixmap);
	}

	mController->markChildDragged(this);
	Qt::DropAction const action = drag->exec(Qt::CopyAction | Qt::MoveAction);
	mController->unmarkChildDragged(action != Qt::IgnoreAction);
}

void Tool::resizeTool(QGraphicsSceneMouseEvent *event)
{
	QRectF newContents = boundingRect();
	QPointF parentPos = QPointF(0, 0);
	QGraphicsItem *parItem = parentItem();
	if (parItem) {
		parentPos = parItem->scenePos();
	}

	switch (mDragState) {
	case TopLeft:
		newContents.setTopLeft(event->pos());
		setPos(event->scenePos() - parentPos);
		break;
	case Top:
		newContents.setTop(event->pos().y());
		setPos(pos().x(), event->scenePos().y() - parentPos.y());
		break;
	case TopRight:
		newContents.setTopRight(event->pos());
		setPos(pos().x(), event->scenePos().y() - parentPos.y());
		break;
	case Left:
		newContents.setLeft(event->pos().x());
		setPos(event->scenePos().x() - parentPos.x(), pos().y());
		break;
	case Right:
		newContents.setRight(event->pos().x());
		break;
	case BottomLeft:
		newContents.setBottomLeft(event->pos());
		setPos(event->scenePos().x() - parentPos.x(), pos().y());
		break;
	case Bottom:
		newContents.setBottom(event->pos().y());
		break;
	case BottomRight:
		newContents.setBottomRight(event->pos());
		break;
	default:
		break;
	}

	if (event->modifiers() && Qt::ShiftModifier) {
		qreal size = std::max(newContents.width(), newContents.height());
		newContents.setWidth(size);
		newContents.setHeight(size);
	}

	resize(QSizeF(newContents.width(), newContents.height()));
}

QRect Tool::widgetGeometry() const
{
	return widget()->geometry();
}

QSize Tool::widgetMaximumSize() const
{
	return widget()->maximumSize();
}

QSize Tool::widgetMinimumSize() const
{
	return widget()->minimumSize();
}

QSize Tool::sizeIncrement() const
{
	return widget()->sizeIncrement();
}

QString Tool::toolTip() const
{
	return widget()->toolTip();
}

bool Tool::isTransparent() const
{
	return widget()->testAttribute(Qt::WA_NoSystemBackground);
}

void Tool::setWidgetGeometry(const QRect &rect)
{
	widget()->setGeometry(rect);
}

void Tool::setWidgetMaximumSize(QSize const &size)
{
	QGraphicsProxyWidget::setMaximumSize(size);
	widget()->setMaximumSize(size);
}

void Tool::setWidgetMinimumSize(QSize const &size)
{
	QGraphicsProxyWidget::setMinimumSize(size);
	widget()->setMinimumSize(size);
}

void Tool::setSizeIncrement(QSize const &size)
{
	widget()->setSizeIncrement(size);
}

void Tool::setToolTip(QString const &toolTip)
{
	widget()->setToolTip(toolTip);
}

void Tool::setTransparent(const bool transparent)
{
	widget()->setAttribute(Qt::WA_NoSystemBackground, transparent);
}

void Tool::removeChild(Tool *child)
{
	childItems().removeOne(child);
}
