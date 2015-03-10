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
	mMetamodelRepoApi = &metamodelRepoApi;
}

void GenerationRulesPlugin::generateCode()
{
//	for (int i = 0; i < mMetamodelRepoApi->elementsCount(); ++i) {
//		qDebug() << "element";
//	}

	QString stream = "'enum State {'\n"
			"foreach (State) {\n"
			"State.name ',' \n"
			"newline \n"
			"} \n"
			"StartState.name ',' newline \n"
			"EndState.name newline \n"
			"'}' \n";

	// testing lexer
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
	QSharedPointer<simpleParser::ast::Program> program = parserResult.dynamicCast<simpleParser::ast::Program>();
	qDebug() << program.isNull();
	auto programRepresentation = qrtext::as<simpleParser::ast::Program>(program);
	qDebug() << "Is null as program: " << programRepresentation.isNull();

	auto programChildren = programRepresentation->children();
	qDebug() << programChildren.size();

	QString resultOfGeneration = generator::CommonGenerator::generatedResult(program, mMetamodelRepoApi);
	//qDebug() << resultOfGeneration;
}
