#include <QtGui/QVBoxLayout>
#include "listWidget.h"

using namespace qReal;

ListWidget::ListWidget(QWidget *parent)
	: QWidget(parent)
	, mListWidget(new QListWidget(this))
	, mOkButton(new QPushButton(tr("&OK"), this))
{
	mOkButton->setDisabled(true);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(mListWidget);
	mainLayout->addWidget(mOkButton);

	setLayout(mainLayout);

	connect(mListWidget, SIGNAL(itemSelectionChanged()), this, SLOT(okActivate()));
	connect(mOkButton, SIGNAL(clicked()), this, SLOT(okButtonHandler()));
	connect(mListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
			this, SLOT(doubleClickHandler(QListWidgetItem*)));
}

void ListWidget::addItem(QString const &text, QString const &userData)
{
	QListWidgetItem *currentItem = new QListWidgetItem(text, mListWidget);
	currentItem->setToolTip(userData);
	mListWidget->addItem(currentItem);
}

void ListWidget::okButtonHandler()
{
	emit userDataSelected(mListWidget->currentItem()->toolTip());
}

void ListWidget::doubleClickHandler(QListWidgetItem *item)
{
	emit userDataSelected(item->toolTip());
}

void ListWidget::okActivate()
{
	mOkButton->setEnabled(true);
}
