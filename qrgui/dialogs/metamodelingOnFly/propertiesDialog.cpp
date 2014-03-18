#include <QtCore/QStringListModel>
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QMessageBox>

#include "propertiesDialog.h"
#include "ui_propertiesDialog.h"

#include "view/editorView.h"
#include "view/editorViewScene.h"

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
		, mElementsOnDiagram(IdList())
{
	mUi->setupUi(this);

	mEditPropertiesDialog = new EditPropertiesDialog(interperterEditorManager, id, &mMainWindow, mMainWindow.models()->mutableLogicalRepoApi());
	setWindowTitle(tr("Properties: ") + mInterperterEditorManager.friendlyName(mId));
	mUi->propertiesNamesList->setWrapping(true);
	updatePropertiesNamesList();

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
	mPropertiesNames = mInterperterEditorManager.propertyNames(mId);
	QStringList const propertiesDisplayedNames = getPropertiesDisplayedNamesList(mPropertiesNames);
	mUi->propertiesNamesList->clear();
	mUi->propertiesNamesList->addItems(propertiesDisplayedNames);
	disableParentProperties(propertiesDisplayedNames);
}

void PropertiesDialog::disableParentProperties(QStringList const propertiesDisplayedNames)
{
	int const size = mPropertiesNames.length();

	for (int i = 0; i < size; i++) {
		if (mInterperterEditorManager.isParentProperty(mId, mPropertiesNames[i])) {
			mUi->propertiesNamesList->findItems(propertiesDisplayedNames[i]
					, Qt::MatchFixedString).first()->setFlags(Qt::NoItemFlags);
		}
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

	QString const &propertyName = mPropertiesNames[mUi->propertiesNamesList->currentRow()];
	mInterperterEditorManager.deleteProperty(propertyName);
	updatePropertiesNamesList();
}

void PropertiesDialog::change(QString const &text)
{
	if (!text.isEmpty()) {
		mEditPropertiesDialog->changeProperty(mUi->propertiesNamesList->item(mUi->propertiesNamesList->currentRow()), mPropertiesNames[mUi->propertiesNamesList->currentRow()], text, IdList());
	} else {
		mEditPropertiesDialog->changeProperty(mUi->propertiesNamesList->item(mUi->propertiesNamesList->currentRow()), "", text, mElementsOnDiagram);
	}
	mElementsOnDiagram.clear();
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

void PropertiesDialog::findElementsOnDiagram(qrRepo::LogicalRepoApi const &api, Id &id)
{
	if (id.idSize() != 3) {
		id = Id(id.editor(), id.diagram(), id.element());
	}

	IdList logicalElements = api.logicalElements(id);
	for (const auto &logicalElement: logicalElements) {
		if (!mElementsOnDiagram.contains(logicalElement)) {
			mElementsOnDiagram << logicalElement;
		}
	}

	for (auto &nodeChild: mInterperterEditorManager.children(id)) {
		findElementsOnDiagram(api, nodeChild);
	}
}

void PropertiesDialog::addProperty()
{
	qrRepo::LogicalRepoApi const &logicalRepoApi = mMainWindow.models()->logicalRepoApi();
	findElementsOnDiagram(logicalRepoApi, mId);
	mUi->propertiesNamesList->setCurrentItem(NULL);
	change("");
}

void PropertiesDialog::changeProperty()
{
	if (mUi->propertiesNamesList->selectedItems().isEmpty()) {
		return;
	}

	QString const &propDisplayedName = mUi->propertiesNamesList->item(mUi->propertiesNamesList->currentRow())->text();
	change(propDisplayedName);
}
