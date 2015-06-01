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

#include <QtCore/QStringListModel>
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QMessageBox>

#include "propertiesDialog.h"
#include "ui_propertiesDialog.h"

using namespace qReal;
using namespace gui;

PropertiesDialog::PropertiesDialog(const EditorManagerInterface &interpreterEditorManager
		, qrRepo::LogicalRepoApi &logicalRepoApi
		, const Id &id
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

QStringList PropertiesDialog::getPropertiesDisplayedNamesList(const QStringList &propertiesNames)
{
	QStringList propertiesDisplayedNames;
	for (const QString &propertyName : propertiesNames) {
		propertiesDisplayedNames << mInterpreterEditorManager.propertyDisplayedName(mId, propertyName);
	}

	return propertiesDisplayedNames;
}

void PropertiesDialog::updatePropertiesNamesList()
{
	mPropertiesNames = mInterpreterEditorManager.propertyNames(mId);
	const QStringList propertiesDisplayedNames = getPropertiesDisplayedNamesList(mPropertiesNames);
	mUi->propertiesNamesList->clear();
	mUi->propertiesNamesList->addItems(propertiesDisplayedNames);
	disableParentProperties(propertiesDisplayedNames);
}

void PropertiesDialog::disableParentProperties(const QStringList propertiesDisplayedNames)
{
	const int size = mPropertiesNames.length();

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

	const QString &propertyName = mPropertiesNames[mUi->propertiesNamesList->currentRow()];
	mInterpreterEditorManager.deleteProperty(propertyName);
	updatePropertiesNamesList();
}

void PropertiesDialog::change(const QString &text)
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


bool PropertiesDialog::checkElementOnDiagram(const qrRepo::LogicalRepoApi &api, Id &id)
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

void PropertiesDialog::findElementsOnDiagram(const qrRepo::LogicalRepoApi &api, Id &id)
{
	if (id.idSize() != 3) {
		id = Id(id.editor(), id.diagram(), id.element());
	}

	IdList logicalElements = api.logicalElements(id);
	for (const auto &logicalElement: logicalElements) {
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

	const QString &propDisplayedName = mUi->propertiesNamesList->item(mUi->propertiesNamesList->currentRow())->text();
	change(propDisplayedName);
}
