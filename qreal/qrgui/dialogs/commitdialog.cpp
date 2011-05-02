#include "commitdialog.h"

CommitDialog::CommitDialog(QWidget *parent) : QDialog(parent)
{
	enterMessageLabel = new QLabel(tr("Enter message here:"));
	ok = new QPushButton(tr("Ok"), this);
	cancel = new QPushButton(tr("Cancel"), this);
	messageTextEdit = new QTextEdit(this);

	QHBoxLayout *buttonsLayout = new QHBoxLayout;
	buttonsLayout->addStretch();
	buttonsLayout->addWidget(ok);
	buttonsLayout->addWidget(cancel);

	QObject::connect(ok, SIGNAL(clicked()), this, SLOT(accept()));
	QObject::connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));

	QGridLayout *mainLayout = new QGridLayout;
	mainLayout->addWidget(messageTextEdit, 0, 0);
	mainLayout->addLayout(buttonsLayout, 1, 0);
	setLayout(mainLayout);

	setWindowTitle(tr("Enter commit message"));
	setFixedSize(400,300);
}

QString CommitDialog::message()
{
	return messageTextEdit->toPlainText();
}
