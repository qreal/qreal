#include <QtWidgets/QHBoxLayout>

#include "logDialog.h"

using namespace git::ui;

LogDialog::LogDialog(QWidget *parent) : QDialog(parent)
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

	setWindowTitle(tr("Commits: "));
	setFixedSize(400,300);
}

void LogDialog::message(QString message)
{
	mMessageTextEdit->setText(message);
}


