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

using namespace generationRules;

GenerationRulesPlugin::GenerationRulesPlugin()
	: mRepo(nullptr)
	, mAction(tr("Destroy everything"), nullptr)
{
	connect(&mAction, &QAction::triggered, this, &GenerationRulesPlugin::generateCode);
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

void GenerationRulesPlugin::generateCode()
{
	QString enumStream =
			"'enum State {' newline \n"
				"foreach (state in State) {\n"
				"tab state.Name ',' \n"
				"newline \n"
			"} \n"
			"tab StartState.Name ',' newline \n"
			"tab EndState.Name newline \n"
			"'}' \n"
			;

	auto parserResultForEnum = generatedTreeFromString(enumStream);
	auto programForEnum = parserResultForEnum.dynamicCast<simpleParser::ast::Program>();
	generationRules::generator::VariablesTable table;
	QString resultOfGenerationForEnum = generator::CommonGenerator::generatedResult(programForEnum
			, mMetamodelRepoApi, mRepo, mLogicalModelAssistInterface, table);
	qDebug() << resultOfGenerationForEnum;
	table.clear();

	QString stateStream =
			"foreach (state in State) {\n"
				"'case ' state.Name ':' newline \n"
				"foreach (transition in state.outcomingLinks(Transition)) { \n"
					"tab 'if (symbol == ' transition.symbol ')' newline \n"
						"tab tab 'currentState = ' transition.transitionEnd.Name ';' newline \n"
						"tab tab 'break;' newline \n"
				"} \n"
				"tab 'else' newline \n"
					"tab tab 'currentState = ' EndState.Name ';' newline \n"
					"tab tab 'break;' newline"
			"}"
			;

	auto parserResultForState = generatedTreeFromString(stateStream);
	auto programForState = parserResultForState.dynamicCast<simpleParser::ast::Foreach>();
	QString resultOfGenerationForState = generator::CommonGenerator::generatedResult(programForState
			, mMetamodelRepoApi, mRepo, mLogicalModelAssistInterface, table);
	qDebug() << resultOfGenerationForState;
}

QSharedPointer<simpleParser::ast::Node> GenerationRulesPlugin::generatedTreeFromString(QString stream)
{
	QScopedPointer<simpleParser::Lexer> lexer;
	QList<qrtext::core::Error> errors;

	errors.clear();
	lexer.reset(new simpleParser::Lexer(errors));

	auto lexerResult = lexer->tokenize(stream);

	for (qrtext::core::Token<simpleParser::TokenTypes> token : lexerResult) {
		qDebug() << token.token();
	}

	// testing parser
	errors.clear();
	QScopedPointer<simpleParser::Parser> parser;
	parser.reset(new simpleParser::Parser(errors));
	auto parserResult = parser->parse(lexerResult, lexer->userFriendlyTokenNames());

	for (qrtext::core::Error error : errors) {
		qDebug() << error.errorMessage();
	}

	return parserResult;
}

void GenerationRulesPlugin::openGenerationRulesWindow()
{
	const QAction * const action = static_cast<QAction *>(sender());
	const qReal::Id id = action->data().value<qReal::Id>();
	mSpecifyGenerationRulesDialog = new qReal::gui::SpecifyGenerationRulesDialog(mEditorManagerInterface, id, mMetamodelRepoApi);
}
