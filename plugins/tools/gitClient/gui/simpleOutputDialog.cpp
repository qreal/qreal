#include <QtWidgets/QHBoxLayout>

#include "simpleOutputDialog.h"

using namespace git::ui;

SimpleOutputDialog::SimpleOutputDialog(QString const &outputDialogName, QWidget *parent) : QDialog(parent)
{
	mMessageText = new QTextBrowser(this);
	mMessageText->setReadOnly(true);

	QHBoxLayout *buttonsLayout = new QHBoxLayout;
	buttonsLayout->addStretch();


	QGridLayout *mainLayout = new QGridLayout;
	mainLayout->addWidget(mMessageText, 0, 0);
	mainLayout->addLayout(buttonsLayout, 1, 0);
	setLayout(mainLayout);

	setWindowTitle(outputDialogName);
	setMinimumSize(400,300);
}

void SimpleOutputDialog::message(QString message)
{
	mMessageText->setText(message);
}


