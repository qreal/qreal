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

#include "generationRulesPlugin.h"

#include <QtWidgets/QAction>
#include <QtWidgets/QMessageBox>
#include <QtCore/QScopedPointer>
#include <QtCore/QSharedPointer>
#include <QtCore/QDebug>
#include <QtCore/QDir>

#include "lexer.h"
#include "parser.h"

#include "ast/complexIdentifier.h"
#include "ast/foreach.h"
#include "ast/identifier.h"
#include "ast/newline.h"
#include "ast/node.h"
#include "ast/program.h"
#include "ast/text.h"

#include "generator/commonInfo/variablesTable.h"
#include "generator/commonGenerator.h"
#include "generator/commonInfo/generatorConfigurer.h"
#include "generator/commonInfo/scopeInfo.h"

#include "treeGeneratorFromString.h"

using namespace generationRules;

GenerationRulesPlugin::GenerationRulesPlugin()
	: mRepo(nullptr)
	, mAction(tr("Generate code for current diagram"), nullptr)
{
	connect(&mAction, &QAction::triggered, this, &GenerationRulesPlugin::openWindowForPathsSpecifying);
}

GenerationRulesPlugin::~GenerationRulesPlugin()
{
}

QList<qReal::ActionInfo> GenerationRulesPlugin::actions()
{
	qReal::ActionInfo info(&mAction, "file", "tools");
	return {info};
}

QList<QAction *> GenerationRulesPlugin::menuActionList() const
{
	auto addGenerationRuleAction = new QAction(tr("Add generation rule"), nullptr);
	connect(addGenerationRuleAction, &QAction::triggered, this, &GenerationRulesPlugin::openGenerationRulesWindow);

	return {addGenerationRuleAction};
}

void GenerationRulesPlugin::init(const qReal::PluginConfigurator &configurator
		, qrRepo::LogicalRepoApi &metamodelRepoApi
		, qReal::EditorManagerInterface *editorManagerInterface)
{
	mRepo = &configurator.repoControlInterface();
	mMainWindowInterpretersInterface = &configurator.mainWindowInterpretersInterface();
	mLogicalModelAssistInterface = &configurator.logicalModelApi();

	mMetamodelRepoApi = &metamodelRepoApi;

	mEditorManagerInterface = editorManagerInterface;
}

void GenerationRulesPlugin::generateCodeForAllElements()
{
	mPathToGeneratedCode = mSpecifyPathsDialog->currentPathToFolder();
	mMainFileName = mSpecifyPathsDialog->currentFileName();

	// we need id of root element in metamodel
	// we consider that we have only one editor and one diagram
	// TODO: fix this
	const auto editorId = mEditorManagerInterface->editors().first();
	const auto diagramId = mEditorManagerInterface->diagrams(editorId).first();
	const auto fullRootNodeName = mEditorManagerInterface->diagramNodeNameString(editorId, diagramId);

	const auto rootNodeName = fullRootNodeName.split("/").last();
	mRootId = mEditorManagerInterface->elementsWithTheSameName(diagramId, rootNodeName, "MetaEntityNode").first();

	generateCode(editorId, diagramId);
}

void GenerationRulesPlugin::generateCode(
		const qReal::Id &editorId
		, const qReal::Id &diagramId)
{
	const auto rootStream = mEditorManagerInterface->generationRule(mRootId);

	const auto parserResultForRoot = TreeGeneratorFromString::generatedTreeFromString(rootStream);
	const auto programForRoot = parserResultForRoot.dynamicCast<simpleParser::ast::Program>();

	generationRules::generator::VariablesTable table;
	generationRules::generator::CurrentScope scope;

	generationRules::generator::GeneratorConfigurer generatorConfigurer(mLogicalModelAssistInterface
			, mEditorManagerInterface, editorId, diagramId, mPathToGeneratedCode);
	generationRules::generator::ScopeInfo scopeInfo(table, scope);

	const auto resultOfGenerationForRoot = generator::CommonGenerator::generatedResult(programForRoot
			, generatorConfigurer, scopeInfo);

	if (!resultOfGenerationForRoot.isEmpty()) {
		qDebug() << mPathToGeneratedCode << mMainFileName;
		QFile outputFile(mPathToGeneratedCode + "/" + mMainFileName);

		if (outputFile.open(QIODevice::WriteOnly)) {
			QTextStream stream(&outputFile);
			stream << resultOfGenerationForRoot;

			outputFile.close();
		}
	}

	QMessageBox::information(nullptr, tr("Files generated"), tr("All files have been generated to ")
			+ mPathToGeneratedCode + ("."));

	table.clear();
}

void GenerationRulesPlugin::openGenerationRulesWindow()
{
	const auto * const action = static_cast<QAction *>(sender());
	const auto id = action->data().value<qReal::Id>();
	const auto specifyGenerationRulesDialog = new qReal::gui::SpecifyGenerationRulesDialog(mEditorManagerInterface
			, id, mMetamodelRepoApi);

	Q_UNUSED(specifyGenerationRulesDialog);
}

void GenerationRulesPlugin::openWindowForPathsSpecifying()
{
	mSpecifyPathsDialog = new qReal::gui::SpecifyPathToGeneratedCodeDialog(mMetamodelRepoApi);

	connect(mSpecifyPathsDialog, &qReal::gui::SpecifyPathToGeneratedCodeDialog::pathsSpecified, this
			, &GenerationRulesPlugin::generateCodeForAllElements);
}
