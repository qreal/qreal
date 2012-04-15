#include <QtGui>
#include "refWindowDialog.h"

RefWindowDialog::RefWindowDialog(qrRepo::LogicalRepoApi const &logicalRepoApi, QWidget *parent)
	: QDialog(parent)
	, mCommonApi(logicalRepoApi)
{
	mListWidget = new QListWidget();
	mMainLayout = new QHBoxLayout();
}

bool RefWindowDialog::initIds(QMap<QString, QString> foundData)
{
	mListWidget->clear();
	mMainLayout->removeWidget(mListWidget);
	foreach (QString currentId, foundData.keys()) {
		qReal::Id parentId = mCommonApi.parent(qReal::Id::loadFromString(currentId));
		QString parentName = mCommonApi.name(parentId);
		if (parentName.contains(" ")) {
			QListWidgetItem *item = new QListWidgetItem();
			item->setText(parentName + tr(" / ") +
				mCommonApi.name(qReal::Id::loadFromString(currentId)) + foundData[currentId]);
			item->setData(Qt::ToolTipRole, currentId);
			mListWidget->addItem(item);
		}
	}

	QObject::connect(mListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(itemChosen(QListWidgetItem*)));

	mMainLayout->addWidget(mListWidget);
	setLayout(mMainLayout);

	setWindowTitle(tr("Found elements:"));
	setFixedHeight(sizeHint().height());

	return (!mListWidget->count() == 0);
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
