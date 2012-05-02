#include <QtGui>
#include "findAndReplaceDialog.h"

FindAndReplaceDialog::FindAndReplaceDialog(QWidget *parent) : QDialog(parent)
{
	mCheckBoxes.append(new QCheckBox(tr("by name")));
	mCheckBoxes.append(new QCheckBox(tr("by property content")));

	mFindLabel = new QLabel(tr("Find:"));
	mFindLineEdit = new QLineEdit();
	mFindLabel->setBuddy(mFindLabel);
	mReplaceLabel = new QLabel(tr("\nReplace:"));
	mReplaceLineEdit = new QLineEdit();
	mReplaceLabel->setBuddy(mReplaceLabel);
	mButton = new QPushButton(tr("Replace"));
	mButton->setEnabled(false);
	mButton->setDefault(true);

	connect(mFindLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(enableReplaceButton(const QString &)));
	connect(mButton, SIGNAL(clicked()), this, SLOT(replaceHandler()));

	QVBoxLayout *leftLayout = new QVBoxLayout;
	leftLayout->addWidget(mFindLabel);
	leftLayout->addWidget(mReplaceLabel);
	leftLayout->addStretch();
	QVBoxLayout *middleLayout = new QVBoxLayout;
	middleLayout->addWidget(mFindLineEdit);
	middleLayout->addWidget(mReplaceLineEdit);
	foreach (QCheckBox *current, mCheckBoxes)
		middleLayout->addWidget(current);
	QVBoxLayout *rightLayout = new QVBoxLayout;
	rightLayout->addWidget(mButton);
	rightLayout->addStretch();
	QHBoxLayout *mainLayout = new QHBoxLayout;
	mainLayout->addLayout(leftLayout);
	mainLayout->addLayout(middleLayout);
	mainLayout->addLayout(rightLayout);
	setLayout(mainLayout);

	setWindowTitle(tr("Find and replace"));
	setFixedHeight(sizeHint().height());
}

FindAndReplaceDialog::~FindAndReplaceDialog()
{
	delete mFindLabel;
	delete mFindLineEdit;
	delete mReplaceLabel;
	delete mReplaceLineEdit;
	delete mButton;
}

void FindAndReplaceDialog::replaceHandler()
{
	QStringList searchData;
	foreach (QCheckBox *current, mCheckBoxes)
		if (current->isChecked())
			searchData.append(current->text());
	if (!searchData.isEmpty()) {
		searchData.push_front(mReplaceLineEdit->text());
		searchData.push_front(mFindLineEdit->text());
		emit replaceClicked(searchData);
		this->close();
	}
}

void FindAndReplaceDialog::enableReplaceButton(const QString &text)
{
	mButton->setEnabled(!text.isEmpty());
}


