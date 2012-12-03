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

QStringList PropertiesDialog::getPropertiesDisplayedNamesList(QStringList propertiesNames)
{
	QStringList propertiesDisplayedNames;
	foreach (QString propertyName, propertiesNames) {
		propertiesDisplayedNames << mInterperterEditorManager->propertyDisplayedName(mId, propertyName);
	}
	return propertiesDisplayedNames;
}

void PropertiesDialog::updatePropertiesNamesList()
{
	QStringList propertiesNames = mInterperterEditorManager->getPropertyNames(mId);
	QStringList propertiesDisplayedNames = getPropertiesDisplayedNamesList(propertiesNames);
	if (mUi->PropertiesNamesList->count() < propertiesDisplayedNames.length()) {
		mUi->PropertiesNamesList->addItem(propertiesDisplayedNames.last());
	}
}

void PropertiesDialog::init(EditorManagerInterface* interperterEditorManager, Id const &id)
{
	mInterperterEditorManager = interperterEditorManager;
	mId = id;
	setWindowTitle(tr("Properties") + ": " + mInterperterEditorManager->friendlyName(mId));
	QStringList propertiesNames = mInterperterEditorManager->getPropertyNames(mId);
	QStringList propertiesDisplayedNames = getPropertiesDisplayedNamesList(propertiesNames);
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
	connect(editPropertiesDialog, SIGNAL(finished(int)), SLOT(updatePropertiesNamesList()));
}

void PropertiesDialog::addProperty()
{
	mUi->PropertiesNamesList->setCurrentItem(NULL);
	change("");
}

void PropertiesDialog::changeProperty()
{
	if (mUi->PropertiesNamesList->selectedItems().isEmpty()) {
		return;
	}
	QString const &propDisplayedName = mUi->PropertiesNamesList->item(mUi->PropertiesNamesList->currentRow())->text();
	change(propDisplayedName);
}
