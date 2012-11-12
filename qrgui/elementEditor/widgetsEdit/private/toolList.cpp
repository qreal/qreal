#include <QtGui/QMouseEvent>

#include "toolList.h"
#include "toolFactory.h"
#include "../tools/tool.h"

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
	if (!item) {
		return;
	}

	QByteArray data;
	QDataStream dataStream(&data, QIODevice::WriteOnly);
	dataStream << item->text() << QPointF(0, 0);

	QMimeData *mimeData = new QMimeData;
	mimeData->setData("application/x-qreal/widgetEditor", data);

	QDrag *drag = new QDrag(this);
	drag->setMimeData(mimeData);

	QPixmap const pixmap = ToolFactory::instance()->widgetPixmap(item->text());
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
		addItem(item);
	}
}
