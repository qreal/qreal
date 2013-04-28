#include <QtGui/QMouseEvent>
#include <QtGui/QDrag>

#include "toolList.h"
#include "toolFactory.h"

using namespace qReal::widgetsEdit;

ToolList::ToolList(QWidget *parent)
	: QListWidget(parent)
{
	loadItems();
	setDefaultDropAction(Qt::CopyAction);
	viewport()->setAcceptDrops(true);
	setDropIndicatorShown(true);
}

void ToolList::mousePressEvent(QMouseEvent *event)
{
	QListWidgetItem *item = itemAt(event->pos());
	if (!item || !mItemToolMap.contains(item)) {
		return;
	}
	Tool *tool = mItemToolMap[item];

	QByteArray data;
	QDataStream dataStream(&data, QIODevice::WriteOnly);
	dataStream << tool->tag() << QPointF(0, 0);

	QMimeData *mimeData = new QMimeData;
	mimeData->setData("application/x-qreal/widgetEditor", data);

	QDrag *drag = new QDrag(this);
	drag->setMimeData(mimeData);

	QPixmap const pixmap = ToolFactory::instance()->widgetPixmap(tool->tag());
	if (!pixmap.isNull()) {
		drag->setPixmap(pixmap);
	}

	drag->exec(Qt::CopyAction | Qt::MoveAction);
}

void ToolList::keyPressEvent(QKeyEvent *event)
{
	QListWidget::keyPressEvent(event);
	if (!event->isAccepted()) {
		emit keyPressed(event);
	}
}

void ToolList::loadItems()
{
	QListIterator<Tool *> iterator = ToolFactory::instance()->itemsIterator();
	while (iterator.hasNext()) {
		Tool *tool = iterator.next();
		QListWidgetItem *item = new QListWidgetItem(tool->icon(), tool->title());
		mItemToolMap.insert(item, tool);
		addItem(item);
	}
}
