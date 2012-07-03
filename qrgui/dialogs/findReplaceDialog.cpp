#include "findReplaceDialog.h"
#include "ui_findReplaceDialog.h"

FindReplaceDialog::FindReplaceDialog(qrRepo::LogicalRepoApi const &logicalRepoApi, QWidget *parent)
	: QDialog(parent)
	, mCommonApi(logicalRepoApi)
	, mUi(new Ui::FindReplaceDialog)
{
	mUi->setupUi(this);

	mCheckBoxes.append(mUi->mByNameBox);
	mCheckBoxes.append(mUi->mByTypeBox);
	mCheckBoxes.append(mUi->mByPropertyBox);
	mCheckBoxes.append(mUi->mByContentBox);
	mCheckBoxes.append(mUi->mSensitivity);
    mCheckBoxes.append(mUi->mByRegExp);

	mCheckBoxes.first()->setChecked(true);

	foreach (QCheckBox *current, mCheckBoxes) {
		connect(current, SIGNAL(clicked()), this, SLOT(tryEnableReplaceButton()));
	}

	connect(mUi->mFindButton, SIGNAL(clicked()), this, SLOT(findClicked()));
	connect(mUi->mReplaceButton, SIGNAL(clicked()), this, SLOT(replaceHandler()));

	stateClear();

	setWindowTitle(tr("Search"));
}

void FindReplaceDialog::tryEnableReplaceButton()
{
    mUi->mReplaceButton->setEnabled((mUi->mByNameBox->isChecked() || mUi->mByContentBox->isChecked())
            && (!mUi->mByPropertyBox->isChecked()) && (!mUi->mByTypeBox->isChecked()));
}

void FindReplaceDialog::stateClear()
{
	mUi->mFindEdit->clear();
	mUi->mReplaceEdit->clear();
	mCheckBoxes[0]->setChecked(true);
	mCheckBoxes[1]->setChecked(false);
	mCheckBoxes[2]->setChecked(false);
	mCheckBoxes[3]->setChecked(false);
	mUi->mReplaceButton->setEnabled(true);
	mUi->mListWidget->clear();
}

FindReplaceDialog::~FindReplaceDialog()
{
	delete mUi;
}

void FindReplaceDialog::findClicked()
{
	if (mUi->mFindEdit->text().length() != 0) {
		QStringList searchData;
		foreach (QCheckBox *current, mCheckBoxes) {
			if (current->isChecked()) {
				searchData.append(current->text());
			}
		}
		if (!searchData.isEmpty()) {
			searchData.push_front(mUi->mFindEdit->text());
			emit findModelByName(searchData);
		}
	}
}

void FindReplaceDialog::replaceHandler()
{
	if ((mUi->mFindEdit->text().length() != 0) && (mUi->mReplaceEdit->text().length() != 0)) {
		QStringList searchData;
		foreach (QCheckBox *current, mCheckBoxes) {
			if (current->isChecked()) {
				searchData.append(current->text());
			}
		}
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
