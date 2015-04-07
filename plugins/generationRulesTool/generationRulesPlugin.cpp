#include "generationRulesPlugin.h"

#include <QtWidgets/QAction>
#include <QtCore/QScopedPointer>
#include <QtCore/QSharedPointer>
#include <QtCore/QDebug>

#include "lexer.h"
#include "parser.h"

#include "ast/complexIdentifier.h"
#include "ast/foreach.h"
#include "ast/identifier.h"
#include "ast/newline.h"
#include "ast/node.h"
#include "ast/program.h"
#include "ast/text.h"

#include "generator/variablesTable.h"
#include "generator/commonGenerator.h"

#include "treeGeneratorFromString.h"

using namespace generationRules;

GenerationRulesPlugin::GenerationRulesPlugin()
	: mRepo(nullptr)
	, mAction(tr("Destroy everything"), nullptr)
{
	connect(&mAction, &QAction::triggered, this, &GenerationRulesPlugin::generateCodeForAllElements);
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
	auto addGenerationRuleAction = new QAction("Add generation rule!!!", nullptr);
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
	// we need id of root element in metamodel
	// we consider that we have only one editor and one diagram
	// TODO: fix this
	qReal::Id editorId = mEditorManagerInterface->editors().first();
	qReal::Id diagramId = mEditorManagerInterface->diagrams(editorId).first();
	QString fullRootNodeName = mEditorManagerInterface->diagramNodeNameString(editorId, diagramId);

	QString rootNodeName = fullRootNodeName.split("/").last();
	qReal::Id rootNodeId = mEditorManagerInterface->elementsWithTheSameName(diagramId, rootNodeName, "MetaEntityNode").first();

	generateCode(rootNodeId);
}

void GenerationRulesPlugin::generateCode(const qReal::Id &rootId)
{
	QString rootStream = mEditorManagerInterface->generationRule(rootId);

	auto parserResultForRoot = TreeGeneratorFromString::generatedTreeFromString(rootStream);
	auto programForRoot = parserResultForRoot.dynamicCast<simpleParser::ast::Program>();
	generationRules::generator::VariablesTable table;
	QString resultOfGenerationForRoot = generator::CommonGenerator::generatedResult(programForRoot
			, mLogicalModelAssistInterface, table, mEditorManagerInterface);
	qDebug() << resultOfGenerationForRoot;
	table.clear();
}

void GenerationRulesPlugin::openGenerationRulesWindow()
{
	const QAction * const action = static_cast<QAction *>(sender());
	const qReal::Id id = action->data().value<qReal::Id>();
	mSpecifyGenerationRulesDialog = new qReal::gui::SpecifyGenerationRulesDialog(mEditorManagerInterface, id, mMetamodelRepoApi);
}
