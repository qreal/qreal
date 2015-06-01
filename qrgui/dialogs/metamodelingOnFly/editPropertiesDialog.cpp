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

#include <QtWidgets/QMessageBox>

#include "editPropertiesDialog.h"
#include "ui_editPropertiesDialog.h"

#include "dialogs/metamodelingOnFly/propertiesDialog.h"

using namespace qReal;

EditPropertiesDialog::EditPropertiesDialog(const EditorManagerInterface &interpreterEditorManager
		, qrRepo::LogicalRepoApi &api
		, const Id &id
		, QWidget *parent)
	: QDialog(parent)
	, mUi(new Ui::EditPropertiesDialog)
	, mInterpreterEditorManager(interpreterEditorManager)
	, mId(id)
	, mMode(addNew)
	, mApi(api)
	, mElementsOnDiagram(IdList())
{
	mUi->setupUi(this);
	connect(mUi->okPushButton, &QPushButton::clicked, this, &EditPropertiesDialog::okButtonClicked);
}

EditPropertiesDialog::~EditPropertiesDialog()
{
	delete mUi;
}

void EditPropertiesDialog::initDefaultValues()
{
	mUi->attributeTypeEdit->setText(mInterpreterEditorManager.typeName(mId, mPropertyName));
	mUi->defaultValueEdit->setText(mInterpreterEditorManager.defaultPropertyValue(mId, mPropertyName));
	mUi->displayedNameEdit->setText(mInterpreterEditorManager.propertyDisplayedName(mId, mPropertyName));
}

void EditPropertiesDialog::messageBoxCancel()
{
	mUi->attributeTypeEdit->setText(mInterpreterEditorManager.typeName(mId, mPropertyName));
	mUi->defaultValueEdit->setText(mInterpreterEditorManager.defaultPropertyValue(mId, mPropertyName));
}

void EditPropertiesDialog::updateProperties()
{
	mInterpreterEditorManager.updateProperties(
			mId
			, mPropertyName
			, mUi->attributeTypeEdit->text()
			, mUi->defaultValueEdit->text()
			, mUi->displayedNameEdit->text()
			);

	if (mPropertyItem != nullptr) {
		mPropertyItem->setText(mInterpreterEditorManager.propertyDisplayedName(mId, mPropertyName));
	}

	done(QDialog::Accepted);
}

void EditPropertiesDialog::acceptPropertyModifications()
{
	if (mPropertyName.isEmpty()) {
		const IdList sameNameProperties = mInterpreterEditorManager.propertiesWithTheSameName(mId, ""
				, mUi->displayedNameEdit->text());
		if (sameNameProperties.isEmpty()) {
			mPropertyName = mUi->displayedNameEdit->text();
		} else {
			mPropertyName = mUi->displayedNameEdit->text() + "_" + sameNameProperties.count();
		}

		mInterpreterEditorManager.addProperty(mId, mPropertyName);
		// set property default value for elements on diagram
		for (const auto &elementOnDiagram: mElementsOnDiagram) {
			mApi.setProperty(elementOnDiagram, mPropertyName, mUi->defaultValueEdit->text());
		}

		mElementsOnDiagram.clear();
	}

	if (mMode == editExisting
			&& mInterpreterEditorManager.typeName(mId, mPropertyName) != mUi->attributeTypeEdit->text()
			)
	{
		// TODO: Remove connects.
		QMessageBox messageBox(tr("Warning:")
				, tr("You changed the type of property. In case of incorrect conversion it may"
						" result in resetting of the existing property value.")
				, QMessageBox::Warning, QMessageBox::Ok, QMessageBox::Cancel, QMessageBox::NoButton);
		messageBox.button(QMessageBox::Ok)->setText(tr("Proceed anyway"));
		messageBox.button(QMessageBox::Cancel)->setText(tr("Cancel the type conversion"));
		connect(messageBox.button(QMessageBox::Cancel), &QAbstractButton::clicked
				, this, &EditPropertiesDialog::messageBoxCancel);
		connect(messageBox.button(QMessageBox::Ok), &QAbstractButton::clicked
				, this, &EditPropertiesDialog::updateProperties);
		messageBox.exec();
	} else {
		updateProperties();
	}
}

void EditPropertiesDialog::okButtonClicked()
{
	if (mUi->attributeTypeEdit->text().isEmpty() || mUi->displayedNameEdit->text().isEmpty()) {
		QMessageBox::critical(this, tr("Error"), tr("All required properties should be filled!"));
	} else {
		const IdList propertiesWithTheSameNameList = mInterpreterEditorManager.propertiesWithTheSameName(mId
				, mPropertyName, mUi->displayedNameEdit->text());
		if (!propertiesWithTheSameNameList.isEmpty()) {
			hide();
			mRestorePropertiesDialog = new RestorePropertiesDialog(this, mInterpreterEditorManager);
			mRestorePropertiesDialog->fillSameNamePropertiesTW(propertiesWithTheSameNameList
					, mUi->displayedNameEdit->text());
			mRestorePropertiesDialog->setWindowTitle(tr("Restore properties"));
			mRestorePropertiesDialog->setModal(true);
			mRestorePropertiesDialog->show();
			connect(mRestorePropertiesDialog, &qReal::RestorePropertiesDialog::createNewChosen
					, this, &EditPropertiesDialog::acceptPropertyModifications);
			connect(mRestorePropertiesDialog, &qReal::RestorePropertiesDialog::finished
					, this, &EditPropertiesDialog::done);
		} else {
			acceptPropertyModifications();
		}
	}
}

void EditPropertiesDialog::changeProperty(
		QListWidgetItem *propertyItem
		, const QString &propertyName
		, const QString &propertyDisplayedName
		, qReal::IdList *elementsOnDiagram
		)
{
	//mPropertyName = mInterpreterEditorManager.propertyNameByDisplayedName(mId, propertyDisplayedName);
	mPropertyName = propertyName;
	mPropertyItem = propertyItem;
	mElementsOnDiagram = *elementsOnDiagram;

	if (propertyName.isEmpty()) {
		setWindowTitle(tr("Add new property"));
		mMode = addNew;
	} else {
		setWindowTitle(tr("Properties editor: ") + propertyDisplayedName);
		mMode = editExisting;
	}

	initDefaultValues();
}
