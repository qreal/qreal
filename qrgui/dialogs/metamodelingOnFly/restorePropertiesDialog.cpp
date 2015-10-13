/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "restorePropertiesDialog.h"
#include "ui_restorePropertiesDialog.h"

using namespace qReal;

RestorePropertiesDialog::RestorePropertiesDialog(QWidget *parent
		, const EditorManagerInterface &interpreterEditorManager)
	: QDialog(parent)
	, mUi(new Ui::RestorePropertiesDialog)
	, mInterpreterEditorManager(interpreterEditorManager)
{
	mUi->setupUi(this);
	mUi->sameNamePropertiesTW->insertColumn(0);
	mUi->sameNamePropertiesTW->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Property name")));
	mUi->sameNamePropertiesTW->insertColumn(1);
	mUi->sameNamePropertiesTW->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("State")));
	mUi->sameNamePropertiesTW->insertColumn(2);
	mUi->sameNamePropertiesTW->setHorizontalHeaderItem(2, new QTableWidgetItem(tr("Type")));
	mUi->sameNamePropertiesTW->insertColumn(3);
	mUi->sameNamePropertiesTW->setHorizontalHeaderItem(3, new QTableWidgetItem(tr("Default value")));
	mUi->sameNamePropertiesTW->adjustSize();
	this->setMinimumSize(mUi->sameNamePropertiesTW->size());
	mUi->sameNamePropertiesTW->horizontalHeader()->setStretchLastSection(true);
	connect(mUi->restoreButton, &QPushButton::clicked, this, &RestorePropertiesDialog::restoreButtonClicked);
	connect(mUi->createNewButton, &QPushButton::clicked, this, &RestorePropertiesDialog::createButtonClicked);
}

RestorePropertiesDialog::~RestorePropertiesDialog()
{
	delete mUi;
}

void RestorePropertiesDialog::fillSameNamePropertiesTW(const IdList &propertiesWithTheSameNameList
		, const QString &propertyName)
{
	mPropertiesWithTheSameNameList = propertiesWithTheSameNameList;
	for (int i = 0; i < mPropertiesWithTheSameNameList.count(); i++) {
		mUi->sameNamePropertiesTW->insertRow(i);
		QStringList propertyParams = mInterpreterEditorManager.getSameNamePropertyParams(
				mPropertiesWithTheSameNameList[i], propertyName);
		for (int j = 0; j < propertyParams.count(); j++) {
			mUi->sameNamePropertiesTW->setItem(i, j, new QTableWidgetItem(propertyParams[j]));
		}
	}
}

void RestorePropertiesDialog::restoreButtonClicked()
{
	if (!mUi->sameNamePropertiesTW->selectedItems().isEmpty()) {
		int firstSelectedRow = mUi->sameNamePropertiesTW->selectedItems().first()->row();
		QString state = mUi->sameNamePropertiesTW->item(firstSelectedRow, 1)->text();
		QString propertyPreviousName = mUi->sameNamePropertiesTW->item(firstSelectedRow, 0)->text();
		if (state == tr("Deleted")) {
			mInterpreterEditorManager.restoreRemovedProperty(mPropertiesWithTheSameNameList[firstSelectedRow]
					, propertyPreviousName);
		} else if (state != tr("Existed")) {
			// Restore renamed property
			mInterpreterEditorManager.restoreRenamedProperty(mPropertiesWithTheSameNameList[firstSelectedRow]
					, propertyPreviousName);
		}
		done(QDialog::Accepted);
	}
}

void RestorePropertiesDialog::createButtonClicked()
{
	emit createNewChosen();
	done(QDialog::Accepted);
}
