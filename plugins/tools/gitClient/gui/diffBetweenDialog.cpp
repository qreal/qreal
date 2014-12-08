#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QFileDialog>

#include "diffBetweenDialog.h"
#include "../../../qrkernel/settingsManager.h"


using namespace git::ui;

DiffBetweenDialog::DiffBetweenDialog(QWidget *parent)
	: QDialog(parent)
{
	mFirstHashLabel = new QLabel(tr("New hash: "));
	mScndHashLabel = new QLabel(tr("Old hash: (write nothing to diff between new hash and current project)"));
	mFirstHash = new QComboBox();
	mFirstHash->setEditable(true);
	mScndHash = new QComboBox();
	mScndHash->setEditable(true);
	QPushButton *ok = new QPushButton(tr("OK"), this);

	QHBoxLayout *buttonsLayout = new QHBoxLayout;
	buttonsLayout->addStretch();
	buttonsLayout->addWidget(ok);

	connect(ok, SIGNAL(clicked()), this, SLOT(accept()));

	QGridLayout *mainLayout = new QGridLayout;
	mainLayout->addWidget(mFirstHashLabel, 0, 0);
	mainLayout->addWidget(mFirstHash, 1, 0, 1, 2);
	mainLayout->addWidget(mScndHashLabel, 2, 0);
	mainLayout->addWidget(mScndHash, 3, 0, 1, 2);
	mainLayout->addLayout(buttonsLayout, 4, 2, 1, 3);
	setLayout(mainLayout);

	setWindowTitle(tr("DiffBetween options"));
	setMinimumSize(600,200);
}

QString DiffBetweenDialog::firstHash()
{
	return mFirstHash->currentText();
}

QString DiffBetweenDialog::secondHash()
{
	return mScndHash->currentText();
}
