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
