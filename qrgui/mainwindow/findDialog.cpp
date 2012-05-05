#include <QtGui>
#include "findDialog.h"

FindDialog::FindDialog(qrRepo::LogicalRepoApi const &logicalRepoApi, QWidget *parent)
	: QDialog(parent)
	, mCommonApi(logicalRepoApi)
{
	mListWidget = new QListWidget();

	mCheckBoxes.append(new QCheckBox(tr("by name")));
	mCheckBoxes.append(new QCheckBox(tr("by type")));
	mCheckBoxes.append(new QCheckBox(tr("by property")));
	mCheckBoxes.append(new QCheckBox(tr("by property content")));

	mCheckBoxes.first()->setChecked(true);

	mLabel = new QLabel(tr("Find:"));
	mLineEdit = new QLineEdit();
	mLabel->setBuddy(mLineEdit);
	mReplaceButton = new QPushButton(tr("Find and replace"));
	mReplaceButton->setDefault(true);
	mReplaceButton->setEnabled(true);
	mFindButton = new QPushButton(tr("Find"));
	mFindButton->setDefault(true);
	mFindButton->setEnabled(false);

	connect(mLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(enableFindButton(const QString &)));
	connect(mFindButton, SIGNAL(clicked()), this, SLOT(findClicked()));
	connect(mReplaceButton, SIGNAL(clicked()), this, SLOT(replaceClicked()));

	QVBoxLayout *headLayout = new QVBoxLayout;
	QVBoxLayout *leftLayout = new QVBoxLayout;
	leftLayout->addWidget(mLabel);
	leftLayout->addStretch();
	QVBoxLayout *middleLayout = new QVBoxLayout;
	middleLayout->addWidget(mLineEdit);
	foreach (QCheckBox *current, mCheckBoxes)
		middleLayout->addWidget(current);
	QVBoxLayout *rightLayout = new QVBoxLayout;
	rightLayout->addWidget(mFindButton);
	rightLayout->addWidget(mReplaceButton);
	rightLayout->addStretch();
	QHBoxLayout *mainLayout = new QHBoxLayout;
	mainLayout->addLayout(leftLayout);
	mainLayout->addLayout(middleLayout);
	mainLayout->addLayout(rightLayout);
	headLayout->addLayout(mainLayout);
	headLayout->addWidget(mListWidget);
	setLayout(headLayout);

	setWindowTitle(tr("Search"));
}

FindDialog::~FindDialog()
{
	delete mLabel;
	delete mLineEdit;
	delete mFindButton;
	delete mReplaceButton;
	delete mListWidget;
}

void FindDialog::replaceClicked()
{
	this->close();
	emit replaceStarted();
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
	}
}

void FindDialog::enableFindButton(const QString &text)
{
	mFindButton->setEnabled(!text.isEmpty());
}

void FindDialog::initIds(QMap<QString, QString> foundData)
{
	hide();
	mListWidget->clear();
	mListWidget->hide();
	this->layout()->removeWidget(mListWidget);
	foreach (QString currentId, foundData.keys()) {
		qReal::Id parentId = mCommonApi.parent(qReal::Id::loadFromString(currentId));
		QString parentName = mCommonApi.name(parentId);
		if (parentName.contains(" ")) {
			QListWidgetItem *item = new QListWidgetItem();
			item->setText(parentName + tr(" / ") +
				mCommonApi.name(qReal::Id::loadFromString(currentId)) + foundData[currentId]);
			item->setData(Qt::ToolTipRole, currentId);
			mListWidget->addItem(item);
		}
	}

	QObject::connect(mListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(itemChosen(QListWidgetItem*)));

	if (!mListWidget->count() == 0) {
		layout()->addWidget(mListWidget);
		mListWidget->show();
	}

	show();
}

void FindDialog::itemChosen(QListWidgetItem *item)
{
	emit chosenElement(qReal::Id::loadFromString(item->data(Qt::ToolTipRole).toString()));
}
