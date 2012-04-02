#include <QtGui>
#include "refWindowDialog.h"

RefWindowDialog::RefWindowDialog(qrRepo::LogicalRepoApi const &logicalRepoApi, QWidget *parent)
	: QDialog(parent)
	, mApi(logicalRepoApi)
{
	mListWidget = new QListWidget();
	mMainLayout = new QHBoxLayout();
}

void RefWindowDialog::initIds(qReal::IdList ids)
{
	mListWidget->clear();
	mMainLayout->removeWidget(mListWidget);
	foreach (qReal::Id currentId, ids) {
		QListWidgetItem *item = new QListWidgetItem();
		QVariant val = currentId.toString();
		item->setText(mApi.name(currentId));
		item->setData(Qt::ToolTipRole, val);
		mListWidget->addItem(item);
	}

	QObject::connect(mListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(itemChosen(QListWidgetItem*)));

	mMainLayout->addWidget(mListWidget);
	setLayout(mMainLayout);

	setWindowTitle(tr("Found elements:"));
	setFixedHeight(sizeHint().height());
}

RefWindowDialog::~RefWindowDialog()
{
	delete mListWidget;
	delete mMainLayout;
}

void RefWindowDialog::itemChosen(QListWidgetItem *item)
{
	emit chosenElement(qReal::Id::loadFromString(item->data(Qt::ToolTipRole).toString()));
}
