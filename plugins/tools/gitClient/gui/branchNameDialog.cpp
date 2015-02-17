#include "branchNameDialog.h"
#include <QGridLayout>

using namespace git::ui;

BranchNameDialog::BranchNameDialog(const QString &actionDialogName, QWidget *parent) :
	QDialog(parent)
{
	QLabel *label = new QLabel(tr("Enter branch name: "));
	branchNameLineEdit = new QLineEdit();
	QPushButton *ok = new QPushButton(tr("OK"), this);
	QPushButton *cancel = new QPushButton(tr("Cancel"), this);

	QHBoxLayout *buttonsLayout = new QHBoxLayout;
	buttonsLayout->addStretch();
	buttonsLayout->addWidget(ok);
	buttonsLayout->addWidget(cancel);

	connect(ok, SIGNAL(clicked()), this, SLOT(accept()));
	connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));

	QGridLayout *mainLayout = new QGridLayout;
	mainLayout->addWidget(label, 0, 0);
	mainLayout->addWidget(branchNameLineEdit, 0, 1, 1, 2);
	mainLayout->addLayout(buttonsLayout,2, 0, 1, 3);
	setLayout(mainLayout);

	setWindowTitle(actionDialogName);
	setMinimumSize(500, 100);
}


QString BranchNameDialog::getBranchName()
{
	return branchNameLineEdit->text();
}
