#include "treeGeneratorFromString.h"

#include "parser.h"
#include "lexer.h"

using namespace generationRules;

QSharedPointer<simpleParser::ast::Node> TreeGeneratorFromString::generatedTreeFromString(const QString &stream)
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
