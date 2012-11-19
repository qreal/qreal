#include <QStringListModel>
#include "propertiesDialog.h"
#include "ui_propertiesDialog.h"

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
	setWindowTitle(tr("Properties") + ": " + mInterperterEditorManager->friendlyName(id));
	QStringList propertiesNames = mInterperterEditorManager->getPropertyNames(id);
	QStringList propertiesDisplayedNames;
	foreach (QString propertyName, propertiesNames) {
		propertiesDisplayedNames << mInterperterEditorManager->propertyDisplayedName(id, propertyName);
	}
	mUi->PropertiesNamesList->addItems(propertiesDisplayedNames);
	mUi->PropertiesNamesList->setWrapping(true);
	int size = propertiesNames.length();
	for (int i = 0; i < size; i++) {
		if (mInterperterEditorManager->isParentProperty(id, propertiesNames[i])) {
			mUi->PropertiesNamesList->findItems(propertiesDisplayedNames[i], Qt::MatchFixedString).first()->setFlags(Qt::NoItemFlags);
		}
	}

	connect(mUi->closeButton, SIGNAL(clicked()), this, SLOT(closeDialog()));
	connect(mUi->deleteButton, SIGNAL(clicked()), this, SLOT(deleteProperty()));
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
