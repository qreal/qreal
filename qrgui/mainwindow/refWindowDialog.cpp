#include <QtGui>
#include "refWindowDialog.h"

RefWindowDialog::RefWindowDialog(QWidget *parent) : QDialog(parent)
{
	mListWidget = new QListWidget();
	mMainLayout = new QHBoxLayout();
}

void RefWindowDialog::init(qReal::IdList ids, QStringList names)
{
	mListWidget->clear();
	mMainLayout->removeWidget(mListWidget);
	QString l = tr("");
	for (int i = 0; i < ids.length(); i++) {
		QListWidgetItem *item = new QListWidgetItem();
		QVariant val = ids[i].toString();
		item->setText(names[i]);
		item->setData(Qt::ToolTipRole, val);
		mListWidget->addItem(item);l = l + tr("1");
	}

	QObject::connect(mListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(itemChosen(QListWidgetItem*)));

	mMainLayout->addWidget(mListWidget);
	setLayout(mMainLayout);

	setWindowTitle(tr("Found elements:") + l);
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
