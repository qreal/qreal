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
	mainLayout->addWidget(mFirstHash, 0, 1, 1, 2);
	mainLayout->addWidget(mScndHashLabel, 0, 2);
	mainLayout->addWidget(mScndHash, 0, 3, 1, 2);
	mainLayout->addLayout(buttonsLayout,2, 4, 1, 3);
	setLayout(mainLayout);

	setWindowTitle(tr("DiffBetween options"));
	setFixedSize(600,200);
}

QString DiffBetweenDialog::firstHash()
{
	mFirstHash->currentText();
}

QString DiffBetweenDialog::secondHash()
{
	mScndHash->currentText();
}
