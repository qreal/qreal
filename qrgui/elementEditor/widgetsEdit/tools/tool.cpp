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
	, mProxy(new ToolProxy(new QWidget))
	, mSelected(false)
	, mPropertyManager(NULL)
{
	setWidget(widget);
}

void Tool::onLoaded()
{
	mPropertyManager = new PropertyManager(mProxy);
	connect(mProxy, SIGNAL(maximumSizeChanged(QSize))
			, this, SLOT(syncMaximumSize(QSize)));
	connect(mProxy, SIGNAL(minimumSizeChanged(QSize))
			, this, SLOT(syncMinimumSize(QSize)));
	connect(this, SIGNAL(propertyChanged(QString,QVariant))
			, mPropertyManager, SLOT(changeProperty(QString,QVariant)));
	connect(mProxy, SIGNAL(propertyChanged(QString,QVariant))
			, mPropertyManager, SLOT(changeProperty(QString,QVariant)));
	mProxy->setTransparent(true);
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

QString Tool::tag() const
{
	return mTag;
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

qReal::PropertyProxyBase *Tool::propertyProxy() const
{
	return mProxy;
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
			QString const tagName = child->tag();
			QDomElement childElement = document.createElement(tagName);
			element.appendChild(childElement);
			child->generateXml(childElement, document);
		}
	}
	// It is important to invoke it here because children must be generated before
	if (mPropertyManager) {
		mPropertyManager->generateXml(element, document, mProxy->outerBindings());
	}
}

void Tool::deserializeWidget(QWidget *parent, const QDomElement &element
		 , QList<PropertyEditorInterface *> &editors)
{
	QMap<QString, QString> outerBindings;
	for (int i = 0; i < element.childNodes().count(); ++i) {
		QDomNode const node = element.childNodes().at(i);
		QDomElement const childElem = node.toElement();
		QString const type = childElem.tagName();
		if (type == "property") {
			mPropertyManager->deserializeProperty(childElem, outerBindings);
		}
		if (type == "layoutAttachedProperty") {
			LayoutTool::deserializeAttachedProperty(parent, widget(), childElem);
		}
	}
	mProxy->setOuterBindings(outerBindings);
	mProxy->generateBinders(editors);
}

void Tool::load(LayoutTool *parent, QDomElement const &element)
{
	if (parent) {
		setParentItem(parent);
	}
	QMap<QString, QString> outerBindings;
	for (int i = 0; i < element.childNodes().count(); ++i) {
		QDomNode const node = element.childNodes().at(i);
		QDomElement const childElem = node.toElement();
		QString const type = childElem.tagName();
		if (type == "property") {
			mPropertyManager->deserializeProperty(childElem, outerBindings);
		}
		if (type == "layoutAttachedProperty" && parent) {
			parent->loadAttachedProperty(this, childElem);
		}
	}
	mProxy->setOuterBindings(outerBindings);
}

QRectF Tool::resizeRect(DragState state)
{
	QRectF const itemBoundingRect = boundingRect();
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
	dataStream << tag() << hotSpot;

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

void Tool::removeChild(Tool *child)
{
	childItems().removeOne(child);
}

void Tool::syncMaximumSize(QSize const &size)
{
	setMaximumSize(size);
}

void Tool::syncMinimumSize(QSize const &size)
{
	setMinimumSize(size);
}

ToolProxy::ToolProxy(QWidget *widget)
	: qReal::PropertyProxyBase(widget), mWidget(widget)
{
}

bool ToolProxy::isEnabled() const
{
	return mWidget->isEnabled();
}

QRect ToolProxy::widgetGeometry() const
{
	return mWidget->geometry();
}

QSize ToolProxy::widgetMaximumSize() const
{
	return mWidget->maximumSize();
}

QSize ToolProxy::widgetMinimumSize() const
{
	return mWidget->minimumSize();
}

QSize ToolProxy::sizeIncrement() const
{
	return mWidget->sizeIncrement();
}

QString ToolProxy::toolTip() const
{
	return mWidget->toolTip();
}

bool ToolProxy::isTransparent() const
{
	return mWidget->testAttribute(Qt::WA_NoSystemBackground);
}

QSizePolicy ToolProxy::sizePolicy() const
{
	return mWidget->sizePolicy();
}

void ToolProxy::setEnabled(bool const enabled)
{
	mWidget->setEnabled(enabled);
}

void ToolProxy::setWidgetGeometry(const QRect &rect)
{
	// To avoid infinite loop in outer bindings
	if (mOldGeometry != rect) {
		mOldGeometry = rect;
//		// One more hack. Else outer bindings do not work
//		if (NodeElement *node = getNode()) {
//			node->storeGeometry();
//		}
		onPropertyChanged("geometry", rect);
		mWidget->setGeometry(rect);
	}
}

void ToolProxy::setWidgetMaximumSize(QSize const &size)
{
	mWidget->setMaximumSize(size);
}

void ToolProxy::setWidgetMinimumSize(QSize const &size)
{
	mWidget->setMinimumSize(size);
}

void ToolProxy::setSizeIncrement(QSize const &size)
{
	mWidget->setSizeIncrement(size);
}

void ToolProxy::setToolTip(QString const &toolTip)
{
	mWidget->setToolTip(toolTip);
}

void ToolProxy::setTransparent(const bool transparent)
{
	mWidget->setAttribute(Qt::WA_NoSystemBackground, transparent);
}

void ToolProxy::setSizePolicy(QSizePolicy const &policy)
{
	mWidget->setSizePolicy(policy);
}
