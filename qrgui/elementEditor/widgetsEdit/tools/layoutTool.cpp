#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsSceneDragDropEvent>
#include <QtWidgets/QGraphicsLinearLayout>
#include <QtGui/QPainter>
#include <qrgui/elementEditor/widgetsEdit/private/toolFactory.h>

#include "layoutTool.h"
#include "layoutHelpers/layoutHelperFactory.h"

using namespace qReal::widgetsEdit;

LayoutTool::LayoutTool(QWidget *widget, ToolController *controller
		, QGraphicsItem *parent)
	: Tool(widget, controller, parent)
	, mDraggingOver(false), mHelper(NULL)
{
	mEffect = new QGraphicsColorizeEffect;
	mEffect->setColor(QColor::fromRgb(255, 0, 0));
	mEffect->setStrength(1.0);
	mEffect->setEnabled(false);
	setGraphicsEffect(mEffect);

	setFlag(QGraphicsItem::ItemClipsChildrenToShape);

	mFactory = new LayoutHelperFactory(this);
}

void LayoutTool::onLoaded()
{
	Tool::onLoaded();
	mLayoutToolProxy = dynamic_cast<LayoutToolProxy *>(mProxy);
	connect(mLayoutToolProxy, SIGNAL(leftMarginChanged(int))
			, this, SLOT(invalidateLayoutMargin()));
	connect(mLayoutToolProxy, SIGNAL(rightMarginChanged(int))
			, this, SLOT(invalidateLayoutMargin()));
	connect(mLayoutToolProxy, SIGNAL(topMarginChanged(int))
			, this, SLOT(invalidateLayoutMargin()));
	connect(mLayoutToolProxy, SIGNAL(bottomMarginChanged(int))
			, this, SLOT(invalidateLayoutMargin()));
	invalidateLayoutMargin();
}

LayoutHelperFactory *LayoutTool::layoutFactory() const
{
	return mFactory;
}

void LayoutTool::setLayoutHelper(LayoutHelperBase *helper)
{
	mHelper = helper;
	makeChildrenResizable(helper == NULL);
	invalidateLayoutMargin();
}

void LayoutTool::removeChild(Tool *child)
{
	Tool::removeChild(child);
	if (layout()) {
		setMinimumSize(layout()->minimumSize());
	}
}

void LayoutTool::startChildDrag(Tool *child)
{
	if (mHelper) {
		mHelper->startChildDrag(child);
		removeChild(child);
	}
}

void LayoutTool::finishChildDrag(bool success)
{
	if (mHelper) {
		mHelper->finishChildDrag(success);
	}
}

void LayoutTool::generateXml(QDomElement &element, QDomDocument &document)
{
	element.setAttribute("layout"
		, LayoutHelperFactory::layoutTypeToString(mFactory->currentType()));
	//important to invoke it here because children must be generated before
	Tool::generateXml(element, document);
	if (!mHelper) {
		return;
	}
	int i = 0;
	foreach(QGraphicsItem *item, childItems()) {
		Tool *child = dynamic_cast<Tool *>(item);
		QDomNode childNode = element.childNodes().at(i++);
		QDomElement layoutAttachedProperty = document.createElement("layoutAttachedProperty");
		mHelper->generateAttachedProperty(layoutAttachedProperty, child);
		childNode.appendChild(layoutAttachedProperty);
	}
}

void LayoutTool::deserializeWidget(QWidget *parent, QDomElement const &element
		, QList<PropertyEditorInterface *> &editors)
{
	Tool::deserializeWidget(parent, element, editors);
	QString const layoutType = element.attribute("layout", "invalid");
	QLayout *widgetLayout = NULL;
	if (layoutType == "Grid") {
		widgetLayout = new QGridLayout;
	} else if (layoutType == "Horizontal") {
		widgetLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	} else if (layoutType == "Vertical") {
		widgetLayout = new QBoxLayout(QBoxLayout::TopToBottom);
	} else if (layoutType == "NoLayout") {
		widgetLayout = NULL;
	}
	if (widgetLayout) {
		widget()->setLayout(widgetLayout);
	}
	mLayoutToolProxy->invalidateLayoutMargin();
}

void LayoutTool::load(LayoutTool *parent, QDomElement const &element)
{
	Tool::load(parent, element);
	QString const layoutType = element.attribute("layout", "invalid");
	if (layoutType == "Grid") {
		mFactory->setLayout(Grid);
	} else if (layoutType == "Horizontal") {
		mFactory->setLayout(Horizontal);
	} else if (layoutType == "Vertical") {
		mFactory->setLayout(Vertical);
	} else if (layoutType == "NoLayout") {
		mFactory->setLayout(NoLayout);
	}
	invalidateLayoutMargin();
}

void LayoutTool::deserializeAttachedProperty(QWidget *parent, QWidget *widget
		, const QDomElement &element)
{
	QString indexS = element.attribute("index", "invalid");
	bool ok;
	if (indexS == "invalid") {
		QString rowS = element.attribute("row", "invalid");
		QString colS = element.attribute("column", "invalid");
		if (rowS == "invalid" || colS == "invalid") {
			return;
		}
		int const row = rowS.toInt(&ok);
		if (!ok) {
			return;
		}
		int const col = colS.toInt(&ok);
		if (!ok) {
			return;
		}
		QGridLayout *gridLayout = dynamic_cast<QGridLayout *>(parent->layout());
		if (gridLayout) {
			gridLayout->addWidget(widget, row, col);
		} else {
			qDebug() << "WARNING: expected grid layout after deserializetion, got something else";
		}
	} else {
		int const index = indexS.toInt(&ok);
		if (!ok) {
			return;
		}
		QBoxLayout *boxLayout = dynamic_cast<QBoxLayout *>(parent->layout());
		if (boxLayout) {
			boxLayout->insertWidget(index, widget);
		} else {
			qDebug() << "WARNING: expected linear layout after deserializetion, got something else";
		}
	}
}

void LayoutTool::loadAttachedProperty(Tool *tool, const QDomElement &element)
{
	// TODO: reuse  deserialization mechanism
	QString indexS = element.attribute("index", "invalid");
	bool ok;
	if (indexS == "invalid") {
		QString rowS = element.attribute("row", "invalid");
		QString colS = element.attribute("column", "invalid");
		if (rowS == "invalid" || colS == "invalid") {
			return;
		}
		int const row = rowS.toInt(&ok);
		if (!ok) {
			return;
		}
		int const col = colS.toInt(&ok);
		if (!ok) {
			return;
		}
		GridLayoutHelper *helper = dynamic_cast<GridLayoutHelper *>(mHelper);
		if (helper) {
			helper->insertTool(row, col, tool);
			tool->setResizable(false);
		} else {
			qDebug() << "WARNING: expected grid layout after deserializetion, got something else";
		}
	} else {
		int const index = indexS.toInt(&ok);
		if (!ok) {
			return;
		}
		LinearLayoutHelper *helper = dynamic_cast<LinearLayoutHelper *>(mHelper);
		if (helper) {
			helper->insertTool(index, tool);
			tool->setResizable(false);
		} else {
			qDebug() << "WARNING: expected linear layout after deserializetion, got something else";
		}
	}
}

void LayoutTool::paint(QPainter *painter, const QStyleOptionGraphicsItem *option
		, QWidget *widget)
{
	Tool::paint(painter, option, widget);

	QRectF container = geometry();
	int const gridHorizontalCount = container.width() / gridSize;
	int const gridVerticalCount = container.height() / gridSize;

	painter->setPen(Qt::gray);
	for (int i = 1; i < gridHorizontalCount; ++i) {
		for (int j = 1; j < gridVerticalCount; ++j) {
			painter->drawPoint(i * gridSize, j * gridSize);
		}
	}

	if (mDraggingOver && mHelper) {
		mHelper->drawCurrentDropPosition(painter);
	}
}

void LayoutTool::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	Tool::mousePressEvent(event);
	event->accept();
}

void LayoutTool::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	Tool::mouseMoveEvent(event);
	event->accept();
}

void LayoutTool::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	Tool::mouseReleaseEvent(event);
	event->accept();
}

void LayoutTool::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
	if (event->mimeData()->hasFormat("application/x-qreal/widgetEditor")) {
		event->acceptProposedAction();
		mDraggingOver = true;
		highlightDrag();
	} else {
		event->ignore();
	}
}

void LayoutTool::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
	if (event->mimeData()->hasFormat("application/x-qreal/widgetEditor")) {
		if (mHelper) {
			mHelper->setDragPos(event->pos());
			update();
		}
		event->acceptProposedAction();
	} else {
		event->ignore();
	}
}

void LayoutTool::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
	event->accept();
	mDraggingOver = false;
	dehighlightDrag();
}

void LayoutTool::dropEvent(QGraphicsSceneDragDropEvent *event)
{
	mDraggingOver = false;
	if (!event->mimeData()->hasFormat("application/x-qreal/widgetEditor")) {
		event->ignore();
		return;
	}
	QString tag;
	QPointF hotSpot;
	QByteArray data = event->mimeData()->data("application/x-qreal/widgetEditor");
	QDataStream dataStream(&data, QIODevice::ReadOnly);
	dataStream >> tag >> hotSpot;

	Tool *tool = mController->draggedChild();
	if (!tool) {
		tool = ToolFactory::instance()->makeItem(tag, mController);
		if (!tool) {
			event->ignore();
			return;
		}
	}
	tool->setParentItem(this);
	tool->setPos(event->pos() - hotSpot);
	if (mHelper) {
		tool->setResizable(false);
		mHelper->dropItem(tool);
		setMinimumSize(layout()->minimumSize());
	} else {
		tool->setResizable(true);
	}
	event->accept();
	dehighlightDrag();
}

void LayoutTool::highlightDrag()
{
	mEffect->setEnabled(true);
}

void LayoutTool::dehighlightDrag()
{
	mEffect->setEnabled(false);
}

void LayoutTool::makeChildrenResizable(const bool resizable)
{
	foreach (QGraphicsItem *item, this->childItems()) {
		Tool *tool = dynamic_cast<Tool *>(item);
		if (tool) {
			tool->setResizable(resizable);
		}
	}
}

void LayoutTool::invalidateLayoutMargin()
{
	if (layout()) {
		layout()->setContentsMargins(mLayoutToolProxy->layoutLeftMargin()
				, mLayoutToolProxy->layoutTopMargin()
				, mLayoutToolProxy->layoutRightMargin()
				, mLayoutToolProxy->layoutBottomMargin());
	}
}

LayoutToolProxy::LayoutToolProxy(QWidget *widget)
	: ToolProxy(widget), mWidget(widget)
{
	setLayoutLeftMargin(3);
	setLayoutRightMargin(3);
	setLayoutTopMargin(3);
	setLayoutBottomMargin(3);
}

int LayoutToolProxy::layoutLeftMargin() const
{
	return mLayoutLeftMargin;
}

int LayoutToolProxy::layoutRightMargin() const
{
	return mLayoutRightMargin;
}

int LayoutToolProxy::layoutTopMargin() const
{
	return mLayoutTopMargin;
}

int LayoutToolProxy::layoutBottomMargin() const
{
	return mLayoutBottomMargin;
}

void LayoutToolProxy::setLayoutLeftMargin(int margin)
{
	mLayoutLeftMargin = margin;
	invalidateLayoutMargin();
	emit leftMarginChanged(margin);
}

void LayoutToolProxy::setLayoutRightMargin(int margin)
{
	mLayoutRightMargin = margin;
	invalidateLayoutMargin();
	emit rightMarginChanged(margin);
}

void LayoutToolProxy::setLayoutTopMargin(int margin)
{
	mLayoutTopMargin = margin;
	invalidateLayoutMargin();
	emit topMarginChanged(margin);
}

void LayoutToolProxy::setLayoutBottomMargin(int margin)
{
	mLayoutBottomMargin = margin;
	invalidateLayoutMargin();
	emit bottomMarginChanged(margin);
}

void LayoutToolProxy::invalidateLayoutMargin()
{
	if (mWidget->layout()) {
		mWidget->layout()->setContentsMargins(mLayoutLeftMargin
				, mLayoutTopMargin
				, mLayoutRightMargin
				, mLayoutBottomMargin);
	}
}
