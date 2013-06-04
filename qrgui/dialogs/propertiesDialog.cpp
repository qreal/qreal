#include <QtCore/QStringListModel>
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QMessageBox>

#include "propertiesDialog.h"
#include "ui_propertiesDialog.h"
#include "../view/editorView.h"
#include "../view/editorViewScene.h"

using namespace qReal;

PropertiesDialog::PropertiesDialog(MainWindow *mainWindow, QWidget *parent)
		: QDialog(parent)
		, mUi(new Ui::PropertiesDialog)
		, mMainWindow(mainWindow)
		, mEditPropertiesDialog(NULL)
{
	mUi->setupUi(this);
}

PropertiesDialog::~PropertiesDialog()
{
	delete mUi;
}

QStringList PropertiesDialog::getPropertiesDisplayedNamesList(QStringList const &propertiesNames)
{
	QStringList propertiesDisplayedNames;
	foreach (QString const &propertyName, propertiesNames) {
		propertiesDisplayedNames << mInterperterEditorManager->propertyDisplayedName(mId, propertyName);
	}
	return propertiesDisplayedNames;
}

void PropertiesDialog::updatePropertiesNamesList()
{
	QStringList const propertiesNames = mInterperterEditorManager->propertyNames(mId);
	QStringList const propertiesDisplayedNames = getPropertiesDisplayedNamesList(propertiesNames);
	if (mUi->PropertiesNamesList->count() < propertiesDisplayedNames.length()) {
		mUi->PropertiesNamesList->addItem(propertiesDisplayedNames.last());
	}
}

void PropertiesDialog::init(EditorManagerInterface* interperterEditorManager, Id const &id)
{
	mInterperterEditorManager = interperterEditorManager;
	mId = id;
	mEditPropertiesDialog = new EditPropertiesDialog(*interperterEditorManager, id, mMainWindow);
	setWindowTitle(tr("Properties: ") + mInterperterEditorManager->friendlyName(mId));
	QStringList const propertiesNames = mInterperterEditorManager->propertyNames(mId);
	QStringList const propertiesDisplayedNames = getPropertiesDisplayedNamesList(propertiesNames);
	mUi->PropertiesNamesList->addItems(propertiesDisplayedNames);
	mUi->PropertiesNamesList->setWrapping(true);
	int const size = propertiesNames.length();
	for (int i = 0; i < size; i++) {
		if (mInterperterEditorManager->isParentProperty(mId, propertiesNames[i])) {
			mUi->PropertiesNamesList->findItems(propertiesDisplayedNames[i]
					, Qt::MatchFixedString).first()->setFlags(Qt::NoItemFlags);
		}
	}

	connect(mUi->closeButton, SIGNAL(clicked()), this, SLOT(closeDialog()));
	connect(mUi->deleteButton, SIGNAL(clicked()), this, SLOT(deleteProperty()));
	connect(mUi->addButton, SIGNAL(clicked()), this, SLOT(addProperty()));
	connect(mUi->changeButton, SIGNAL(clicked()), this, SLOT(changeProperty()));
}

void PropertiesDialog::closeDialog()
{
	delete mEditPropertiesDialog;
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
	mEditPropertiesDialog->changeProperty(mUi->PropertiesNamesList->item(mUi->PropertiesNamesList->currentRow()), text);
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

	foreach (Id nodeChild, mInterperterEditorManager->children(id)) {
		sign |= checkElementOnDiagram(api, nodeChild);
	}

	return sign;
}

void PropertiesDialog::addProperty()
{
	qrRepo::LogicalRepoApi const &logicalRepoApi = mMainWindow->models()->logicalRepoApi();
	if (checkElementOnDiagram(logicalRepoApi, mId)) {
		QMessageBox::warning(this, tr("Warning")
				, tr("For adding a new property from the scene and from the explorer of logical model should be removed all the elements of the object and its inheritors!"));
	} else {
		mUi->PropertiesNamesList->setCurrentItem(NULL);
		change("");
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
