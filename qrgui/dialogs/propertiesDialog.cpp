#include <QStringListModel>
#include <QInputDialog>
#include <QMessageBox>
#include "propertiesDialog.h"
#include "ui_propertiesDialog.h"
#include "editPropertiesDialog.h"

using namespace qReal;

PropertiesDialog::PropertiesDialog(QWidget *parent)
	: QDialog(parent)
	, mUi(new Ui::PropertiesDialog)
{
	mUi->setupUi(this);
}

PropertiesDialog::~PropertiesDialog()
{
	delete mUi;
}

void PropertiesDialog::init(EditorManagerInterface* interperterEditorManager, Id const &id)
{
	mInterperterEditorManager = interperterEditorManager;
	mId = id;
	setWindowTitle(tr("Properties") + ": " + mInterperterEditorManager->friendlyName(mId));
	QStringList propertiesNames = mInterperterEditorManager->getPropertyNames(mId);
	QStringList propertiesDisplayedNames;
	foreach (QString propertyName, propertiesNames) {
		propertiesDisplayedNames << mInterperterEditorManager->propertyDisplayedName(mId, propertyName);
	}
	mUi->PropertiesNamesList->addItems(propertiesDisplayedNames);
	mUi->PropertiesNamesList->setWrapping(true);
	int size = propertiesNames.length();
	for (int i = 0; i < size; i++) {
		if (mInterperterEditorManager->isParentProperty(mId, propertiesNames[i])) {
			mUi->PropertiesNamesList->findItems(propertiesDisplayedNames[i], Qt::MatchFixedString).first()->setFlags(Qt::NoItemFlags);
		}
	}

	connect(mUi->closeButton, SIGNAL(clicked()), this, SLOT(closeDialog()));
	connect(mUi->deleteButton, SIGNAL(clicked()), this, SLOT(deleteProperty()));
	connect(mUi->addButton, SIGNAL(clicked()), this, SLOT(addProperty()));
	connect(mUi->changeButton, SIGNAL(clicked()), this, SLOT(changeProperty()));
}

void PropertiesDialog::closeDialog()
{
	close();
}

void PropertiesDialog::deleteProperty()
{
	if (mUi->PropertiesNamesList->selectedItems().isEmpty()) {
		return;
	}
	QListWidgetItem *selectedItem = mUi->PropertiesNamesList->takeItem(mUi->PropertiesNamesList->currentRow());
	QString const &propDisplayedName = selectedItem->text();
	mInterperterEditorManager->deleteProperty(propDisplayedName);
}

void PropertiesDialog::change(QString const &text)
{
	EditPropertiesDialog *editPropertiesDialog = new EditPropertiesDialog();
	editPropertiesDialog->init(mUi->PropertiesNamesList->item(mUi->PropertiesNamesList->currentRow()), mInterperterEditorManager, mId, text);
	editPropertiesDialog->setModal(true);
	editPropertiesDialog->show();
}

void PropertiesDialog::addProperty()
{
	bool ok;
	mUi->PropertiesNamesList->setCurrentItem(NULL);
	QString const text = QInputDialog::getText(this, tr("New property"), tr("Enter the property name:"), QLineEdit::Normal, QString::null, &ok);
	if (ok) {
		if (!text.isEmpty()) {
			mUi->PropertiesNamesList->addItem(text);
			mInterperterEditorManager->addProperty(mId, text);
			change(text);
		} else {
			QMessageBox::critical(this, tr("Error"), tr("The property name can not be empty!"));
			addProperty();
		}
	}
}

void PropertiesDialog::changeProperty()
{
	if (mUi->PropertiesNamesList->selectedItems().isEmpty()) {
		return;
	}
	QString const &propDisplayedName = mUi->PropertiesNamesList->item(mUi->PropertiesNamesList->currentRow())->text();
	change(propDisplayedName);
}
