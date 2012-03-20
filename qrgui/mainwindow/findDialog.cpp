#include <QtGui>
#include "findDialog.h"

FindDialog::FindDialog(QWidget *parent) : QDialog(parent)
{
	mLabel = new QLabel(tr("Model name:"));
	mLineEdit = new QLineEdit();
	mLabel->setBuddy(mLineEdit);
	mFindButton = new QPushButton(tr("Find"));
	mFindButton->setDefault(true);
	mFindButton->setEnabled(false);

	connect(mLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(enableFindButton(const QString &)));
	connect(mFindButton, SIGNAL(clicked()), this, SLOT(findClicked()));

	QHBoxLayout *topLeftLayout = new QHBoxLayout;
	topLeftLayout->addWidget(mLabel);
	topLeftLayout->addWidget(mLineEdit);
	QVBoxLayout *leftLayout = new QVBoxLayout;
	leftLayout->addLayout(topLeftLayout);
	QVBoxLayout *rightLayout = new QVBoxLayout;
	rightLayout->addWidget(mFindButton);
	rightLayout->addStretch();
	QHBoxLayout *mainLayout = new QHBoxLayout;
	mainLayout->addLayout(leftLayout);
	mainLayout->addLayout(rightLayout);
	setLayout(mainLayout);

	setWindowTitle(tr("Find model"));
	setFixedHeight(sizeHint().height());
}

void FindDialog::findClicked()
{
	this->close(); // temporary
	QString text = mLineEdit->text();
	emit findModelByName(text);
}

void FindDialog::enableFindButton(const QString &text)
{
	mFindButton->setEnabled(!text.isEmpty());
}

