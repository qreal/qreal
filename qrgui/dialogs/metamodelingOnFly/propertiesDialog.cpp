#include <QtCore/QStringListModel>
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QMessageBox>

#include "propertiesDialog.h"
#include "ui_propertiesDialog.h"

#include "editor/editorView.h"
#include "editor/editorViewScene.h"

using namespace qReal;
using namespace gui;

PropertiesDialog::PropertiesDialog(EditorManagerInterface &interpreterEditorManager
		, qrRepo::LogicalRepoApi &logicalRepoApi
		, Id const &id
		, QWidget *parent)
	: QDialog(parent)
	, mUi(new Ui::PropertiesDialog)
	, mInterpreterEditorManager(interpreterEditorManager)
	, mLogicalRepoApi(logicalRepoApi)
	, mId(id)
	, mElementsOnDiagram(new IdList())
	, mEditPropertiesDialog(interpreterEditorManager, logicalRepoApi, id)
{
	mUi->setupUi(this);

	setWindowTitle(tr("Properties: ") + mInterpreterEditorManager.friendlyName(mId));
	mUi->propertiesNamesList->setWrapping(true);
	updatePropertiesNamesList();

	connect(mUi->closeButton, SIGNAL(clicked()), this, SLOT(closeDialog()));
	connect(mUi->deleteButton, SIGNAL(clicked()), this, SLOT(deleteProperty()));
	connect(mUi->addButton, SIGNAL(clicked()), this, SLOT(addProperty()));
	connect(mUi->changeButton, SIGNAL(clicked()), this, SLOT(changeProperty()));
}

PropertiesDialog::~PropertiesDialog()
{
	delete mElementsOnDiagram;
	delete mUi;
}

QStringList PropertiesDialog::getPropertiesDisplayedNamesList(QStringList const &propertiesNames)
{
	QStringList propertiesDisplayedNames;
	for (QString const &propertyName : propertiesNames) {
		propertiesDisplayedNames << mInterpreterEditorManager.propertyDisplayedName(mId, propertyName);
	}

	return propertiesDisplayedNames;
}

void PropertiesDialog::updatePropertiesNamesList()
{
	mPropertiesNames = mInterpreterEditorManager.propertyNames(mId);
	QStringList const propertiesDisplayedNames = getPropertiesDisplayedNamesList(mPropertiesNames);
	mUi->propertiesNamesList->clear();
	mUi->propertiesNamesList->addItems(propertiesDisplayedNames);
	disableParentProperties(propertiesDisplayedNames);
}

void PropertiesDialog::disableParentProperties(QStringList const propertiesDisplayedNames)
{
	int const size = mPropertiesNames.length();

	for (int i = 0; i < size; i++) {
		if (mInterpreterEditorManager.isParentProperty(mId, mPropertiesNames[i])) {
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
	mInterpreterEditorManager.deleteProperty(propertyName);
	updatePropertiesNamesList();
}

void PropertiesDialog::change(QString const &text)
{
	if (!text.isEmpty()) {
		mEditPropertiesDialog.changeProperty(
				mUi->propertiesNamesList->item(mUi->propertiesNamesList->currentRow())
				, mPropertiesNames[mUi->propertiesNamesList->currentRow()]
				, text
				, new IdList());
	} else {
		mEditPropertiesDialog.changeProperty(
				mUi->propertiesNamesList->item(mUi->propertiesNamesList->currentRow())
				, ""
				, text
				, mElementsOnDiagram);
	}

	mElementsOnDiagram->clear();
	mEditPropertiesDialog.setModal(true);
	mEditPropertiesDialog.show();
	connect(&mEditPropertiesDialog, SIGNAL(finished(int)), SLOT(updatePropertiesNamesList()));
}


bool PropertiesDialog::checkElementOnDiagram(qrRepo::LogicalRepoApi const &api, Id &id)
{
	if (id.idSize() != 3) {
		id = Id(id.editor(), id.diagram(), id.element());
	}

	bool sign = !api.logicalElements(id).isEmpty();

	foreach (Id nodeChild, mInterpreterEditorManager.children(id)) {
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
	for (auto const &logicalElement: logicalElements) {
		if (!mElementsOnDiagram->contains(logicalElement)) {
			mElementsOnDiagram->append(logicalElement);
		}
	}

	for (auto &nodeChild: mInterpreterEditorManager.children(id)) {
		findElementsOnDiagram(api, nodeChild);
	}
}

void PropertiesDialog::addProperty()
{
	findElementsOnDiagram(mLogicalRepoApi, mId);
	mUi->propertiesNamesList->setCurrentItem(nullptr);
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
