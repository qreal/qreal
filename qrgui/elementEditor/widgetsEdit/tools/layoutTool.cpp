#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsSceneDragDropEvent>
#include <QtGui/QPainter>
#include <QtGui/QGraphicsLinearLayout>

#include "layoutTool.h"
#include "../private/toolFactory.h"
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

LayoutHelperFactory *LayoutTool::layoutFactory() const
{
	return mFactory;
}

void LayoutTool::setLayoutHelper(LayoutHelperBase *helper)
{
	mHelper = helper;
	makeChildrenResizable(helper == NULL);
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

void LayoutTool::deserializeWidget(QWidget *parent, const QDomElement &element)
{
	Tool::deserializeWidget(parent, element);
	QString const layoutType = element.attribute("layout", "invalid");
	QLayout *widgetLayout;
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

void LayoutTool::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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
	QString title;
	QPointF hotSpot;
	QByteArray data = event->mimeData()->data("application/x-qreal/widgetEditor");
	QDataStream dataStream(&data, QIODevice::ReadOnly);
	dataStream >> title >> hotSpot;

	Tool *tool = mController->draggedChild();
	if (!tool) {
		tool = ToolFactory::instance()->makeItem(title, mController);
		if (!tool) {
			event->ignore();
			return;
		}
		mController->addChild(tool);
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
