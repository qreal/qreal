#include <QtGui>
#include "refWindowDialog.h"

RefWindowDialog::RefWindowDialog(QWidget *parent) : QDialog(parent)
{
	mButtons = new QList<QPushButton*>();
	mRefButtonsMapper = new QSignalMapper();
}

void RefWindowDialog::init(QStringList names)
{
	QHBoxLayout *mainLayout = new QHBoxLayout;

	foreach (QString name, names) {
		mButtons->append(new QPushButton(name));

		mainLayout->addWidget(mButtons->last());

		QObject::connect(mButtons->last(), SIGNAL(clicked()), mRefButtonsMapper, SLOT(map()));
		mRefButtonsMapper->setMapping(mButtons->last(), name);
	}

	QObject::connect(mRefButtonsMapper, SIGNAL(mapped(const QString)), this, SLOT(refClicked(const QString)));

	setLayout(mainLayout);

	setWindowTitle(tr("Found elements:"));
	setFixedHeight(sizeHint().height());
}

RefWindowDialog::~RefWindowDialog()
{
	delete mRefButtonsMapper;
	delete mButtons;
}

void RefWindowDialog::refClicked(const QString &refName)
{
	emit chosenElement(refName);
}
