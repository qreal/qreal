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

#include "specifyGenerationRulesDialog.h"
#include "ui_specifyGenerationRulesDialog.h"

using namespace qReal;
using namespace gui;

SpecifyGenerationRulesDialog::SpecifyGenerationRulesDialog(EditorManagerInterface *interpreterEditorManager
		, const Id &id
		, qrRepo::LogicalRepoApi *metamodelRepoApi
		, QWidget *parent)
	: QDialog(parent)
	, mUi(new Ui::SpecifyGenerationRulesDialog)
	, mInterpreterEditorManager(interpreterEditorManager)
	, mId(id)
	, mMetamodelRepoApi(metamodelRepoApi)
{
	mUi->setupUi(this);

	addPropertiesList();
	addOldRule();

	connect(mUi->propertiesView, &QListWidget::itemDoubleClicked, this
			, &SpecifyGenerationRulesDialog::insertPropertyIntoCode);

	connect(mUi->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this
			, &SpecifyGenerationRulesDialog::saveGenerationRule);

	this->setWindowTitle("Specify rules for " + mInterpreterEditorManager->friendlyName(mId));
	this->setVisible(true);
}

SpecifyGenerationRulesDialog::~SpecifyGenerationRulesDialog()
{
	delete mUi;
}

void SpecifyGenerationRulesDialog::insertPropertyIntoCode(QListWidgetItem* property)
{
	const QString propertyName = property->text();
	mUi->codeArea->insertPlainText(propertyName);
}

void SpecifyGenerationRulesDialog::saveGenerationRule()
{
	mInterpreterEditorManager->updateGenerationRule(mId, mUi->codeArea->toPlainText());
}

void SpecifyGenerationRulesDialog::addPropertiesList()
{
	mPropertiesNames = mInterpreterEditorManager->propertyNames(mId);
	const QStringList propertiesDisplayedNames = propertiesDisplayedNamesList(mPropertiesNames);
	mUi->propertiesView->clear();
	mUi->propertiesView->addItems(propertiesDisplayedNames);
}

void SpecifyGenerationRulesDialog::addOldRule()
{
	const QString previousRule = mInterpreterEditorManager->generationRule(mId);
	mUi->codeArea->insertPlainText(previousRule);
}

QStringList SpecifyGenerationRulesDialog::propertiesDisplayedNamesList(const QStringList &propertiesNames)
{
	QStringList propertiesDisplayedNames;
	for (const QString &propertyName : propertiesNames) {
		propertiesDisplayedNames << mInterpreterEditorManager->propertyDisplayedName(mId, propertyName);
	}

	return propertiesDisplayedNames;
}
