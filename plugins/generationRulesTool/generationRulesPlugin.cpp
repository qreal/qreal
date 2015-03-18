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

void GenerationRulesPlugin::init(qReal::PluginConfigurator const &configurator, qrRepo::LogicalRepoApi &metamodelRepoApi)
{
	mRepo = &configurator.repoControlInterface();
	mMainWindowInterpretersInterface = &configurator.mainWindowInterpretersInterface();
	mLogicalModelAssistInterface = &configurator.logicalModelApi();

	mMetamodelRepoApi = &metamodelRepoApi;
}

void GenerationRulesPlugin::generateCode()
{
	QString enumStream =
			"'enum State {'\n"
				"foreach (state in State) {\n"
				"state.Name ',' \n"
				"newline \n"
			"} \n"
			"StartState.Name ',' newline \n"
			"EndState.Name newline \n"
			"'}' \n"
			;

	generateCodeFromString(enumStream);

	QString stateStream =
			"foreach (state in State) {\n"
				"'case ' State.Name ':' newline \n"
				"foreach (transition in state.outcomingLinks) { \n"
					"'if (symbol == ' transition.symbol ')' newline \n"
						"'currentState = ' transition.transitionEnd.Name ';' newline \n"
						"'break;' newline \n"
				"} \n"
				"'else' newline \n"
					"'currentState = ' EndState.Name ';' newline \n"
					"'break;' newline"
			"}"
			;

	generateCodeFromString(stateStream);
}

void GenerationRulesPlugin::generateCodeFromString(QString stream)
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

	// testing parser result
	auto program = parserResult.dynamicCast<simpleParser::ast::Foreach>();
	qDebug() << program.isNull();
	auto programRepresentation = qrtext::as<simpleParser::ast::Foreach>(program);
	qDebug() << "Is null as program: " << programRepresentation.isNull();

	auto programChildren = programRepresentation->children();
	qDebug() << programChildren.size();

	QString resultOfGeneration = generator::CommonGenerator::generatedResult(program, mMetamodelRepoApi, mRepo, mLogicalModelAssistInterface);
	qDebug() << resultOfGeneration;
}
