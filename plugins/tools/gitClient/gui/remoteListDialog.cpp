#include <QtWidgets/QHBoxLayout>

#include "remoteListDialog.h"

using namespace git::ui;

RemoteListDialog::RemoteListDialog(QWidget *parent) : QDialog(parent)
{
	mEnterMessageLabel = new QLabel(tr("Enter message here:"));
	mMessageTextEdit = new QTextEdit(this);
	mMessageTextEdit->setReadOnly(true);

	QHBoxLayout *buttonsLayout = new QHBoxLayout;
	buttonsLayout->addStretch();


	QGridLayout *mainLayout = new QGridLayout;
	mainLayout->addWidget(mMessageTextEdit, 0, 0);
	mainLayout->addLayout(buttonsLayout, 1, 0);
	setLayout(mainLayout);

	setWindowTitle(tr("List of remotes: "));
	setFixedSize(400,300);
}

void RemoteListDialog::message(QString message)
{
	mMessageTextEdit->setText(message);
}



