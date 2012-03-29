#include <QtGui>
#include "refWindowDialog.h"

RefWindowDialog::RefWindowDialog(QWidget *parent) : QDialog(parent)
{
	mReferences = new QListWidget();
	mMainLayout = new QHBoxLayout();
}

void RefWindowDialog::init(QStringList names)
{
	mReferences->clear();
	mMainLayout->removeWidget(mReferences);

	foreach (QString name, names) {
		QListWidgetItem *item = new QListWidgetItem();
		item->setText(name);
		mReferences->addItem(item);
	}
	QObject::connect(mReferences, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(refClicked(QListWidgetItem*)));

	mMainLayout->addWidget(mReferences);
	setLayout(mMainLayout);

	setWindowTitle(tr("Found elements:"));
	setFixedHeight(sizeHint().height());
}

RefWindowDialog::~RefWindowDialog()
{
	delete mReferences;
	delete mMainLayout;
}

void RefWindowDialog::refClicked(QListWidgetItem *ref)
{
	emit chosenElement(ref->text());
}
