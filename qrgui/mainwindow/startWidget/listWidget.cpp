#include "listWidget.h"

using namespace qReal;

ListWidget::ListWidget(QWidget *parent)
		: QWidget(parent)
		, mListWidget(new QListWidget())
{
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(mListWidget);

	setLayout(mainLayout);

	connect(mListWidget, SIGNAL(itemClicked(QListWidgetItem*))
			, this, SLOT(doubleClickHandler(QListWidgetItem*)));
}

void ListWidget::addItem(QString const &text, QString const &userData, QString const &toolTip)
{
	QListWidgetItem *currentItem = new QListWidgetItem(text, mListWidget);
	currentItem->setData(Qt::UserRole, userData);
	currentItem->setToolTip(toolTip);
	mListWidget->addItem(currentItem);
}

int ListWidget::count()
{
	return mListWidget->count();
}

void ListWidget::highlightFirstItem()
{
	if (count() == 0) {
		return;
	}
	mListWidget->setCurrentRow(0);
}

void ListWidget::doubleClickHandler(QListWidgetItem *item)
{
	emit userDataSelected(userData(item));
}

QString ListWidget::userData(QListWidgetItem *item)
{
	return item->data(Qt::UserRole).toString();
}
