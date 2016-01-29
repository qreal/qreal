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

#include <QtWidgets/QPushButton>

#include "appearance/ruleHighlighter.h"
#include "appearance/keywords.h"

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

	generationRules::RuleHighlighter *ruleHighlighter = new generationRules::RuleHighlighter(
			mUi->codeArea->document());

	Q_UNUSED(ruleHighlighter);

	addPropertiesList();

	mUi->templatesTree->header()->close();
	addTemplatesList();

	addOldRule();

	connect(mUi->propertiesView, &QListWidget::itemDoubleClicked, this
			, &SpecifyGenerationRulesDialog::insertPropertyIntoCode);

	connect(mUi->templatesTree, &QTreeWidget::itemDoubleClicked, this
			, &SpecifyGenerationRulesDialog::insertTemplateIntoCode);

	connect(mUi->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this
			, &SpecifyGenerationRulesDialog::saveGenerationRule);

	this->setWindowTitle(tr("Specify rules for ") + mInterpreterEditorManager->friendlyName(mId));
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

void SpecifyGenerationRulesDialog::insertTemplateIntoCode(QTreeWidgetItem *templateItem)
{
	const QString templateName = templateItem->text(0);
	mUi->codeArea->insertPlainText(templateName);
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

void SpecifyGenerationRulesDialog::addTemplatesList()
{
	mUi->templatesTree->setColumnCount(1);

	addOneTypeTemplates(tr("General templates"), generationRules::keywords::generalTemplates);
	addOneTypeTemplates(tr("Text templates"), generationRules::keywords::textTemplates);
	addOneTypeTemplates(tr("Links templates"), generationRules::keywords::linksTemplates);

	mUi->templatesTree->expandAll();
}

void SpecifyGenerationRulesDialog::addOneTypeTemplates(const QString &type, const QStringList &listOfTemplates)
{
	auto * const oneTypeTempatesList = new QTreeWidgetItem(mUi->templatesTree);
	oneTypeTempatesList->setText(0, type);

	QList<QTreeWidgetItem*> templatesItems;
	for (const auto &templateName : listOfTemplates) {
		auto * const treeItem = new QTreeWidgetItem();
		treeItem->setText(0, templateName);
		treeItem->setToolTip(0, generationRules::keywords::templatesAndTooltips[templateName]);
		templatesItems.append(treeItem);
	}

	oneTypeTempatesList->insertChildren(0, templatesItems);

	mUi->templatesTree->insertTopLevelItem(0, oneTypeTempatesList);
}

void SpecifyGenerationRulesDialog::addOldRule()
{
	const auto previousRule = mInterpreterEditorManager->generationRule(mId);
	mUi->codeArea->insertPlainText(previousRule);
}

QStringList SpecifyGenerationRulesDialog::propertiesDisplayedNamesList(const QStringList &propertiesNames)
{
	QStringList propertiesDisplayedNames;
	for (const auto &propertyName : propertiesNames) {
		propertiesDisplayedNames << mInterpreterEditorManager->propertyDisplayedName(mId, propertyName);
	}

	return propertiesDisplayedNames;
}
