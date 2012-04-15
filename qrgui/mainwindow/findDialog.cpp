#include <QtGui>
#include "findDialog.h"

FindDialog::FindDialog(QWidget *parent) : QDialog(parent)
{
	mCheckBoxes.append(new QCheckBox(tr("by name")));
	mCheckBoxes.append(new QCheckBox(tr("by type")));
	mCheckBoxes.append(new QCheckBox(tr("by property")));

	mLabel = new QLabel(tr("Find:"));
	mLineEdit = new QLineEdit();
	mLabel->setBuddy(mLineEdit);
	mFindButton = new QPushButton(tr("Find"));
	mFindButton->setDefault(true);
	mFindButton->setEnabled(false);

	connect(mLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(enableFindButton(const QString &)));
	connect(mFindButton, SIGNAL(clicked()), this, SLOT(findClicked()));

	QVBoxLayout *leftLayout = new QVBoxLayout;
	leftLayout->addWidget(mLabel);
	leftLayout->addStretch();
	QVBoxLayout *middleLayout = new QVBoxLayout;
	middleLayout->addWidget(mLineEdit);
	foreach (QCheckBox *current, mCheckBoxes)
		middleLayout->addWidget(current);
	QVBoxLayout *rightLayout = new QVBoxLayout;
	rightLayout->addWidget(mFindButton);
	rightLayout->addStretch();
	QHBoxLayout *mainLayout = new QHBoxLayout;
	mainLayout->addLayout(leftLayout);
	mainLayout->addLayout(middleLayout);
	mainLayout->addLayout(rightLayout);
	setLayout(mainLayout);

	setWindowTitle(tr("Search"));
	setFixedHeight(sizeHint().height());
}

FindDialog::~FindDialog()
{
	delete mLabel;
	delete mLineEdit;
	delete mFindButton;
}

void FindDialog::findClicked()
{
	QStringList searchData;
	foreach (QCheckBox *current, mCheckBoxes)
		if (current->isChecked())
			searchData.append(current->text());
	if (!searchData.isEmpty()) {
		searchData.push_front(mLineEdit->text());
		emit findModelByName(searchData);
		this->close();
	}
}

void FindDialog::enableFindButton(const QString &text)
{
	mFindButton->setEnabled(!text.isEmpty());
}

