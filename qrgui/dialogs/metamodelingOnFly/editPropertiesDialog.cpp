#include <QtWidgets/QMessageBox>

#include "editPropertiesDialog.h"
#include "ui_editPropertiesDialog.h"
#include "propertiesDialog.h"

using namespace qReal;

EditPropertiesDialog::EditPropertiesDialog(EditorManagerInterface &interpreterEditorManager, Id const &id, QWidget *parent)
		: QDialog(parent)
		, mUi(new Ui::EditPropertiesDialog)
		, mInterperterEditorManager(interpreterEditorManager)
		, mId(id)
		, mMode(addNew)
{
	mUi->setupUi(this);
}

EditPropertiesDialog::~EditPropertiesDialog()
{
	delete mUi;
}

void EditPropertiesDialog::initDefaultValues()
{
	mUi->attributeTypeEdit->setText(mInterperterEditorManager.typeName(mId, mPropertyName));
	mUi->defaultValueEdit->setText(mInterperterEditorManager.defaultPropertyValue(mId, mPropertyName));
	mUi->displayedNameEdit->setText(mInterperterEditorManager.propertyDisplayedName(mId, mPropertyName));
}

void EditPropertiesDialog::messageBoxCancel()
{
	mUi->attributeTypeEdit->setText(mInterperterEditorManager.typeName(mId, mPropertyName));
	mUi->defaultValueEdit->setText(mInterperterEditorManager.defaultPropertyValue(mId, mPropertyName));
}

void EditPropertiesDialog::updateProperties()
{
	mInterperterEditorManager.updateProperties(
			mId
			, mPropertyName
			, mUi->attributeTypeEdit->text()
			, mUi->defaultValueEdit->text()
			, mUi->displayedNameEdit->text()
			);

	if (mPropertyItem != NULL) {
		mPropertyItem->setText(mInterperterEditorManager.propertyDisplayedName(mId, mPropertyName));
	}

	done(QDialog::Accepted);
}

void EditPropertiesDialog::okButtonClicked()
{
	if (mUi->attributeTypeEdit->text().isEmpty() || mUi->displayedNameEdit->text().isEmpty()) {
		QMessageBox::critical(this, tr("Error"), tr("All required properties should be filled!"));
	} else {
		if (mPropertyName.isEmpty()) {
			mPropertyName = mUi->displayedNameEdit->text();
			mInterperterEditorManager.addProperty(mId, mPropertyName);
		}

		if (mMode == editExisting
				&& mInterperterEditorManager.typeName(mId, mPropertyName) != mUi->attributeTypeEdit->text()
				)
		{
			// TODO: Remove connects.
			QMessageBox messageBox(tr("Warning:")
					, tr("You changed the type of property. In case of incorrect conversion it may result in resetting of the existing property value.")
					, QMessageBox::Warning, QMessageBox::Ok, QMessageBox::Cancel, QMessageBox::NoButton);
			messageBox.button(QMessageBox::Ok)->setText(tr("Proceed anyway"));
			messageBox.button(QMessageBox::Cancel)->setText(tr("Cancel the type conversion"));
			connect(messageBox.button(QMessageBox::Cancel), SIGNAL(clicked()), this, SLOT(messageBoxCancel()));
			connect(messageBox.button(QMessageBox::Ok), SIGNAL(clicked()), this, SLOT(updateProperties()));
			messageBox.exec();
		} else {
			updateProperties();
		}
	}
}

void EditPropertiesDialog::changeProperty(QListWidgetItem *propertyItem, QString const &propertyName)
{
	mPropertyName = mInterperterEditorManager.propertyNameByDisplayedName(mId, propertyName);
	mPropertyItem = propertyItem;

	if (propertyName.isEmpty()) {
		setWindowTitle(tr("Add new property"));
		mMode = addNew;
	} else {
		setWindowTitle(tr("Properties editor: ") + propertyName);
		mMode = editExisting;
	}

	initDefaultValues();

	connect(mUi->okPushButton, SIGNAL(clicked()), this, SLOT(okButtonClicked()));
}
