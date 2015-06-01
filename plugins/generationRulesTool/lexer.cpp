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

#include "lexer.h"

using namespace qrtext::core;
using namespace simpleParser;

simpleParser::Lexer::Lexer(QList<Error> &errors)
	: qrtext::core::Lexer<TokenTypes>(initPatterns(), errors)
{
}

TokenPatterns<TokenTypes> simpleParser::Lexer::initPatterns()
{
	TokenPatterns<TokenTypes> tokenDefinitions;

	tokenDefinitions.defineToken(TokenTypes::whitespace, QRegularExpression("[ \t]+"), QObject::tr("whitespace"));
	tokenDefinitions.defineToken(TokenTypes::newline, QRegularExpression("[\n]"), QObject::tr("newline"));
	tokenDefinitions.defineToken(TokenTypes::identifier
			, QRegularExpression(R"([\p{L}_][\p{L}0-9_]*)")
			, QObject::tr("identifier"));

	tokenDefinitions.defineToken(TokenTypes::openingBracket, QRegularExpression("\\("), "(");
	tokenDefinitions.defineToken(TokenTypes::closingBracket, QRegularExpression("\\)"), ")");
	tokenDefinitions.defineToken(TokenTypes::openingCurlyBracket, QRegularExpression("{"), "{");
	tokenDefinitions.defineToken(TokenTypes::closingCurlyBracket, QRegularExpression("}"), "}");

	tokenDefinitions.defineToken(TokenTypes::dot, QRegularExpression("\\."), ".");

	tokenDefinitions.defineKeyword(TokenTypes::newlineKeyword, "newline");
	tokenDefinitions.defineKeyword(TokenTypes::foreachKeyword, "foreach");

	tokenDefinitions.defineToken(TokenTypes::text,
			QRegularExpression(R"(('[^'\\]*(\\(.|\n)[^'\\]*)*'))")
			, QObject::tr("text"));

	return tokenDefinitions;
}

