#include "errorListWidget.h"
#include"mainWindow.h"

ErrorListWidget::ErrorListWidget(QWidget *parent)
{
	Q_UNUSED(parent);
	connect(this, SIGNAL(clicked(QModelIndex)), this, SLOT(clickList()));
}

void ErrorListWidget::clickList()
{
	highlightElement(currentItem());
}

void ErrorListWidget::highlightElement(QListWidgetItem* const item)
{
	qReal::Id const id = qReal::Id::loadFromString(item->data(Qt::ToolTipRole).toString());
	if (item->isSelected())
		mMainWindow->selectItemWithError(id);
}

void ErrorListWidget::init(qReal::MainWindow* mainWindow)
{
	mMainWindow = mainWindow;
}
