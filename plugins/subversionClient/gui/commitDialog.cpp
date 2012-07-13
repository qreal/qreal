#include "commitDialog.h"

using namespace versioning::ui;

CommitDialog::CommitDialog(QWidget *parent) : QDialog(parent)
{
	mEnterMessageLabel = new QLabel(tr("Enter message here:"));
	mOkButton = new QPushButton(tr("Ok"), this);
	mCancel = new QPushButton(tr("Cancel"), this);
	mMessageTextEdit = new QTextEdit(this);

	QHBoxLayout *buttonsLayout = new QHBoxLayout;
	buttonsLayout->addStretch();
	buttonsLayout->addWidget(mOkButton);
	buttonsLayout->addWidget(mCancel);

	QObject::connect(mOkButton, SIGNAL(clicked()), this, SLOT(accept()));
	QObject::connect(mCancel, SIGNAL(clicked()), this, SLOT(reject()));

	QGridLayout *mainLayout = new QGridLayout;
	mainLayout->addWidget(mMessageTextEdit, 0, 0);
	mainLayout->addLayout(buttonsLayout, 1, 0);
	setLayout(mainLayout);

	setWindowTitle(tr("Enter commit message"));
	setFixedSize(400,300);
}

QString CommitDialog::message() const
{
	return mMessageTextEdit->toPlainText();
}
