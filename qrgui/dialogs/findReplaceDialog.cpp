#include "findReplaceDialog.h"
#include "ui_findReplaceDialog.h"

FindReplaceDialog::FindReplaceDialog(qrRepo::LogicalRepoApi const &logicalRepoApi, QWidget *parent)
	: QDialog(parent)
	, mUi(new Ui::FindReplaceDialog)
	, mCommonApi(logicalRepoApi)
{
	mUi->setupUi(this);

	mCheckBoxes.append(mUi->mByNameBox);
	mCheckBoxes.append(mUi->mByTypeBox);
	mCheckBoxes.append(mUi->mByPropertyBox);
	mCheckBoxes.append(mUi->mByContentBox);

	mCheckBoxes.first()->setChecked(true);

	connect(mUi->mFindButton, SIGNAL(clicked()), this, SLOT(findClicked()));
	connect(mUi->mReplaceButton, SIGNAL(clicked()), this, SLOT(replaceHandler()));

	setMode(true);

	setWindowTitle(tr("Search"));
}

void FindReplaceDialog::setMode(bool visibility)
{
	mUi->mFindEdit->clear();
	mUi->mReplaceEdit->clear();
	mUi->mReplaceEdit->setVisible(!visibility);
	mUi->mReplaceLabel->setVisible(!visibility);
	mCheckBoxes[0]->setChecked(true);
	mCheckBoxes[1]->setChecked(false);
	mCheckBoxes[2]->setChecked(false);
	mCheckBoxes[3]->setChecked(false);
	mCheckBoxes[1]->setVisible(visibility);
	mCheckBoxes[2]->setVisible(visibility);
	mUi->mListWidget->setVisible(true);
	mUi->mListWidget->clear();
}

FindReplaceDialog::~FindReplaceDialog()
{
	delete mUi;
}

void FindReplaceDialog::findClicked()
{
	if (mUi->mReplaceEdit->isVisible())
		setMode(true);
	if (mUi->mFindEdit->text().length() != 0) {
		QStringList searchData;
		foreach (QCheckBox *current, mCheckBoxes)
			if (current->isChecked())
				searchData.append(current->text());
		if (!searchData.isEmpty()) {
			searchData.push_front(mUi->mFindEdit->text());
			emit findModelByName(searchData);
		}
	}
}

void FindReplaceDialog::replaceHandler()
{
	if (!mUi->mReplaceEdit->isVisible())
		setMode(false);
	if ((mUi->mFindEdit->text().length() != 0) && (mUi->mReplaceEdit->text().length() != 0)) {
		QStringList searchData;
		foreach (QCheckBox *current, mCheckBoxes)
			if (current->isChecked())
				searchData.append(current->text());
		if (!searchData.isEmpty()) {
			searchData.push_front(mUi->mReplaceEdit->text());
			searchData.push_front(mUi->mFindEdit->text());
			hide();
			emit replaceClicked(searchData);
			show();
		}
	}
}

void FindReplaceDialog::itemChosen(QListWidgetItem *item)
{
	emit chosenElement(qReal::Id::loadFromString(item->data(Qt::ToolTipRole).toString()));
}

void FindReplaceDialog::initIds(QMap<QString, QString> foundData)
{
	mUi->mListWidget->clear();

	foreach (QString currentId, foundData.keys()) {
		qReal::Id parentId = mCommonApi.parent(qReal::Id::loadFromString(currentId));
		QString parentName = mCommonApi.name(parentId);
		if (!parentName.contains("qrm:/")) {
			QListWidgetItem *item = new QListWidgetItem();
			item->setText(parentName + tr(" / ") +
				mCommonApi.name(qReal::Id::loadFromString(currentId)) + foundData[currentId]);
			item->setData(Qt::ToolTipRole, currentId);
			mUi->mListWidget->addItem(item);
		}
	}

	QObject::connect(mUi->mListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(itemChosen(QListWidgetItem*)));
}
