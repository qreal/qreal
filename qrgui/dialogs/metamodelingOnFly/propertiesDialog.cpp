#include <QtCore/QStringListModel>
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QMessageBox>

#include "propertiesDialog.h"
#include "ui_propertiesDialog.h"
#include "../../view/editorView.h"
#include "../../view/editorViewScene.h"

using namespace qReal;
using namespace gui;

PropertiesDialog::PropertiesDialog(MainWindow &mainWindow, EditorManagerInterface &interperterEditorManager
		, Id const &id)
		: QDialog(&mainWindow)
		, mUi(new Ui::PropertiesDialog)
		, mInterperterEditorManager(interperterEditorManager)
		, mId(id)
		, mMainWindow(mainWindow)
		, mEditPropertiesDialog(NULL)
{
	mUi->setupUi(this);

	mEditPropertiesDialog = new EditPropertiesDialog(interperterEditorManager, id, &mMainWindow);
	setWindowTitle(tr("Properties: ") + mInterperterEditorManager.friendlyName(mId));
	QStringList const propertiesNames = mInterperterEditorManager.propertyNames(mId);
	QStringList const propertiesDisplayedNames = getPropertiesDisplayedNamesList(propertiesNames);
	mUi->propertiesNamesList->addItems(propertiesDisplayedNames);
	mUi->propertiesNamesList->setWrapping(true);
	int const size = propertiesNames.length();

	for (int i = 0; i < size; i++) {
		if (mInterperterEditorManager.isParentProperty(mId, propertiesNames[i])) {
			mUi->propertiesNamesList->findItems(propertiesDisplayedNames[i]
					, Qt::MatchFixedString).first()->setFlags(Qt::NoItemFlags);
		}
	}

	connect(mUi->closeButton, SIGNAL(clicked()), this, SLOT(closeDialog()));
	connect(mUi->deleteButton, SIGNAL(clicked()), this, SLOT(deleteProperty()));
	connect(mUi->addButton, SIGNAL(clicked()), this, SLOT(addProperty()));
	connect(mUi->changeButton, SIGNAL(clicked()), this, SLOT(changeProperty()));
}

PropertiesDialog::~PropertiesDialog()
{
	delete mEditPropertiesDialog;
	delete mUi;
}

QStringList PropertiesDialog::getPropertiesDisplayedNamesList(QStringList const &propertiesNames)
{
	QStringList propertiesDisplayedNames;
	foreach (QString const &propertyName, propertiesNames) {
		propertiesDisplayedNames << mInterperterEditorManager.propertyDisplayedName(mId, propertyName);
	}
	return propertiesDisplayedNames;
}

void PropertiesDialog::updatePropertiesNamesList()
{
	QStringList const propertiesNames = mInterperterEditorManager.propertyNames(mId);
	QStringList const propertiesDisplayedNames = getPropertiesDisplayedNamesList(propertiesNames);
	if (mUi->propertiesNamesList->count() < propertiesDisplayedNames.length()) {
		mUi->propertiesNamesList->addItem(propertiesDisplayedNames.last());
	}
}

void PropertiesDialog::closeDialog()
{
	close();
}

void PropertiesDialog::deleteProperty()
{
	if (mUi->propertiesNamesList->selectedItems().isEmpty()) {
		return;
	}

	QListWidgetItem *selectedItem = mUi->propertiesNamesList->takeItem(mUi->propertiesNamesList->currentRow());
	QString const &propDisplayedName = selectedItem->text();
	mInterperterEditorManager.deleteProperty(propDisplayedName);
}

void PropertiesDialog::change(QString const &text)
{
	mEditPropertiesDialog->changeProperty(mUi->propertiesNamesList->item(mUi->propertiesNamesList->currentRow()), text);
	mEditPropertiesDialog->setModal(true);
	mEditPropertiesDialog->show();
	connect(mEditPropertiesDialog, SIGNAL(finished(int)), SLOT(updatePropertiesNamesList()));
}


bool PropertiesDialog::checkElementOnDiagram(qrRepo::LogicalRepoApi const &api, Id &id)
{
	if (id.idSize() != 3) {
		id = Id(id.editor(), id.diagram(), id.element());
	}

	bool sign = !api.logicalElements(id).isEmpty();

	foreach (Id nodeChild, mInterperterEditorManager.children(id)) {
		sign |= checkElementOnDiagram(api, nodeChild);
	}

	return sign;
}

void PropertiesDialog::addProperty()
{
	qrRepo::LogicalRepoApi const &logicalRepoApi = mMainWindow.models()->logicalRepoApi();
	if (checkElementOnDiagram(logicalRepoApi, mId)) {
		QMessageBox::warning(this, tr("Warning")
				, tr("For adding a new property from the scene and from the explorer of logical model"
						"should be removed all the elements of the object and its inheritors!"));
	} else {
		mUi->propertiesNamesList->setCurrentItem(NULL);
		change("");
	}
}

void PropertiesDialog::changeProperty()
{
	if (mUi->propertiesNamesList->selectedItems().isEmpty()) {
		return;
	}

	QString const &propDisplayedName = mUi->propertiesNamesList->item(mUi->propertiesNamesList->currentRow())->text();
	change(propDisplayedName);
}
