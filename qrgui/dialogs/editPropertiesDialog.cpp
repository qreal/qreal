#include <QtWidgets/QMessageBox>

#include "editPropertiesDialog.h"
#include "ui_editPropertiesDialog.h"
#include "propertiesDialog.h"

using namespace qReal;

EditPropertiesDialog::EditPropertiesDialog(EditorManagerInterface &interperterEditorManager, Id const &id, QWidget *parent)
		: QDialog(parent)
		, mUi(new Ui::EditPropertiesDialog)
		, mInterperterEditorManager(interperterEditorManager)
		, mId(id)
{
	mUi->setupUi(this);
}

EditPropertiesDialog::~EditPropertiesDialog()
{
	delete mUi;
}

void EditPropertiesDialog::setupDefaultValues()
{
	mUi->attributeTypeEdit->setText(mInterperterEditorManager.typeName(mId, mPropertyName));
	mUi->defaultValueEdit->setText(mInterperterEditorManager.defaultPropertyValue(mId, mPropertyName));
	mUi->displayedNameEdit->setText(mInterperterEditorManager.propertyDisplayedName(mId, mPropertyName));
}

void EditPropertiesDialog::mbCancel()
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

void EditPropertiesDialog::ok()
{
	if (mUi->attributeTypeEdit->text().isEmpty() || mUi->displayedNameEdit->text().isEmpty()) {
		QMessageBox::critical(this, tr("Error"), tr("All required properties should be filled!"));
	} else {
		if (mPropertyName.isEmpty()) {
			mPropertyName = mUi->displayedNameEdit->text();
			mInterperterEditorManager.addProperty(mId, mPropertyName);
		}

		if (windowTitle() != tr("Adding of new property:")
				&& mInterperterEditorManager.typeName(mId, mPropertyName) != mUi->attributeTypeEdit->text()
				)
		{
			QMessageBox *mb = new QMessageBox(tr("Warning:")
					, tr("You changed the type of property. In case of incorrect conversion it may result in resetting of the existing property value.")
					, QMessageBox::Warning, QMessageBox::Ok, QMessageBox::Cancel, QMessageBox::NoButton);
			mb->button(QMessageBox::Ok)->setText(tr("Proceed anyway"));
			mb->button(QMessageBox::Cancel)->setText(tr("Cancel the type conversion"));
			mb->show();
			connect(mb->button(QMessageBox::Cancel), SIGNAL(clicked()), this, SLOT(mbCancel()));
			connect(mb->button(QMessageBox::Ok), SIGNAL(clicked()), this, SLOT(updateProperties()));
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
		setWindowTitle(tr("Adding of new property:"));
	} else {
		setWindowTitle(tr("Properties editor: ") + propertyName);
	}

	setupDefaultValues();

	connect(mUi->okPushButton, SIGNAL(clicked()), this, SLOT(ok()));
}
