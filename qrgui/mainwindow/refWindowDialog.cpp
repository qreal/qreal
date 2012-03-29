#include <QtGui>
#include "refWindowDialog.h"

RefWindowDialog::RefWindowDialog(QWidget *parent) : QDialog(parent)
{
	mReferences = new QListWidget();
}

void RefWindowDialog::init(QStringList names)
{
	QHBoxLayout *mainLayout = new QHBoxLayout;

	foreach (QString name, names) {
		QListWidgetItem *item = new QListWidgetItem();
		item->setText(name);
		mReferences->addItem(item);
	}
	QObject::connect(mReferences, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(refClicked(QListWidgetItem*)));

	mainLayout->addWidget(mReferences);
	setLayout(mainLayout);

	setWindowTitle(tr("Found elements:"));
	setFixedHeight(sizeHint().height());
}

RefWindowDialog::~RefWindowDialog()
{
	delete mReferences;
}

void RefWindowDialog::refClicked(QListWidgetItem *ref)
{
	emit chosenElement(ref->text());
}
