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

#include "qrtext/src/lua/luaLexer.h"

using namespace qrtext::lua;
using namespace qrtext::lua::details;
using namespace qrtext::core;

LuaLexer::LuaLexer(QList<core::Error> &errors)
	: Lexer<LuaTokenTypes>(initPatterns(), errors)
{
}

TokenPatterns<LuaTokenTypes> LuaLexer::initPatterns()
{
	TokenPatterns<LuaTokenTypes> tokenDefinitions;

	tokenDefinitions.defineToken(LuaTokenTypes::whitespace, QRegularExpression("[ \t]+"), QObject::tr("whitespace"));
	tokenDefinitions.defineToken(LuaTokenTypes::newline, QRegularExpression("[\n]"), QObject::tr("newline"));
	tokenDefinitions.defineToken(LuaTokenTypes::identifier
			, QRegularExpression(R"([\p{L}_][\p{L}0-9_]*)")
			, QObject::tr("identifier"));

	tokenDefinitions.defineKeyword(LuaTokenTypes::andKeyword, "and");
	tokenDefinitions.defineKeyword(LuaTokenTypes::breakKeyword, "break");
	tokenDefinitions.defineKeyword(LuaTokenTypes::doKeyword, "do");
	tokenDefinitions.defineKeyword(LuaTokenTypes::elseKeyword, "else");
	tokenDefinitions.defineKeyword(LuaTokenTypes::elseifKeyword, "elseif");
	tokenDefinitions.defineKeyword(LuaTokenTypes::endKeyword, "end");
	tokenDefinitions.defineKeyword(LuaTokenTypes::falseKeyword, "false");
	tokenDefinitions.defineKeyword(LuaTokenTypes::forKeyword, "for");
	tokenDefinitions.defineKeyword(LuaTokenTypes::functionKeyword, "function");
	tokenDefinitions.defineKeyword(LuaTokenTypes::gotoKeyword, "goto");
	tokenDefinitions.defineKeyword(LuaTokenTypes::ifKeyword, "if");
	tokenDefinitions.defineKeyword(LuaTokenTypes::inKeyword, "in");
	tokenDefinitions.defineKeyword(LuaTokenTypes::localKeyword, "local");
	tokenDefinitions.defineKeyword(LuaTokenTypes::nilKeyword, "nil");
	tokenDefinitions.defineKeyword(LuaTokenTypes::notKeyword, "not");
	tokenDefinitions.defineKeyword(LuaTokenTypes::orKeyword, "or");
	tokenDefinitions.defineKeyword(LuaTokenTypes::repeatKeyword, "repeat");
	tokenDefinitions.defineKeyword(LuaTokenTypes::returnKeyword, "return");
	tokenDefinitions.defineKeyword(LuaTokenTypes::thenKeyword, "then");
	tokenDefinitions.defineKeyword(LuaTokenTypes::trueKeyword, "true");
	tokenDefinitions.defineKeyword(LuaTokenTypes::untilKeyword, "until");
	tokenDefinitions.defineKeyword(LuaTokenTypes::whileKeyword, "while");

	tokenDefinitions.defineToken(LuaTokenTypes::plus, QRegularExpression("\\+"), "+");
	tokenDefinitions.defineToken(LuaTokenTypes::minus, QRegularExpression("-"), "-");
	tokenDefinitions.defineToken(LuaTokenTypes::asterick, QRegularExpression("\\*"), "*");
	tokenDefinitions.defineToken(LuaTokenTypes::slash, QRegularExpression("/"), "/");
	tokenDefinitions.defineToken(LuaTokenTypes::percent, QRegularExpression("%"), "%");
	tokenDefinitions.defineToken(LuaTokenTypes::hat, QRegularExpression("\\^"), "^");
	tokenDefinitions.defineToken(LuaTokenTypes::sharp, QRegularExpression("#"), "#");

	tokenDefinitions.defineToken(LuaTokenTypes::ampersand, QRegularExpression("&"), "&");
	tokenDefinitions.defineToken(LuaTokenTypes::tilda, QRegularExpression("~"), "~");
	tokenDefinitions.defineToken(LuaTokenTypes::verticalLine, QRegularExpression("\\|"), "|");
	tokenDefinitions.defineToken(LuaTokenTypes::doubleLess, QRegularExpression("<<"), "<<");
	tokenDefinitions.defineToken(LuaTokenTypes::doubleGreater, QRegularExpression(">>"), ">>");
	tokenDefinitions.defineToken(LuaTokenTypes::doubleSlash, QRegularExpression("//"), "//");

	tokenDefinitions.defineToken(LuaTokenTypes::doubleEquals, QRegularExpression("=="), "==");
	tokenDefinitions.defineToken(LuaTokenTypes::tildaEquals, QRegularExpression("~="), "~=");
	tokenDefinitions.defineToken(LuaTokenTypes::exclamationMarkEquals, QRegularExpression("!="), "!=");
	tokenDefinitions.defineToken(LuaTokenTypes::lessEquals, QRegularExpression("<="), "<=");
	tokenDefinitions.defineToken(LuaTokenTypes::greaterEquals, QRegularExpression(">="), ">=");
	tokenDefinitions.defineToken(LuaTokenTypes::less, QRegularExpression("<"), "<");
	tokenDefinitions.defineToken(LuaTokenTypes::greater, QRegularExpression(">"), ">");
	tokenDefinitions.defineToken(LuaTokenTypes::equals, QRegularExpression("="), "=");

	tokenDefinitions.defineToken(LuaTokenTypes::openingBracket, QRegularExpression("\\("), "(");
	tokenDefinitions.defineToken(LuaTokenTypes::closingBracket, QRegularExpression("\\)"), ")");
	tokenDefinitions.defineToken(LuaTokenTypes::openingCurlyBracket, QRegularExpression("{"), "{");
	tokenDefinitions.defineToken(LuaTokenTypes::closingCurlyBracket, QRegularExpression("}"), "}");
	tokenDefinitions.defineToken(LuaTokenTypes::openingSquareBracket, QRegularExpression("\\["), "[");
	tokenDefinitions.defineToken(LuaTokenTypes::closingSquareBracket, QRegularExpression("\\]"), "]");
	tokenDefinitions.defineToken(LuaTokenTypes::doubleColon, QRegularExpression("::"), "::");

	tokenDefinitions.defineToken(LuaTokenTypes::semicolon, QRegularExpression(";"), ";");
	tokenDefinitions.defineToken(LuaTokenTypes::colon, QRegularExpression(":"), ":");
	tokenDefinitions.defineToken(LuaTokenTypes::comma, QRegularExpression(","), ",");
	tokenDefinitions.defineToken(LuaTokenTypes::dot, QRegularExpression("\\."), ".");
	tokenDefinitions.defineToken(LuaTokenTypes::doubleDot, QRegularExpression("\\.\\."), "..");
	tokenDefinitions.defineToken(LuaTokenTypes::tripleDot, QRegularExpression("\\.\\.\\."), "...");

	tokenDefinitions.defineToken(LuaTokenTypes::string
			, QRegularExpression(R"(("[^"\\]*(\\(.|\n)[^"\\]*)*"))"
					R"(|('[^'\\]*(\\(.|\n)[^'\\]*)*'))")
			, QObject::tr("string")
			);

	tokenDefinitions.defineToken(LuaTokenTypes::integerLiteral
			, QRegularExpression("(0[xX][0-9a-fA-F]+)|([0-9]+)")
			, QObject::tr("integer literal")
			);

	tokenDefinitions.defineToken(LuaTokenTypes::floatLiteral
			, QRegularExpression(
					"(0[xX][0-9a-fA-F]+("
							"(\\.[0-9a-fA-F]+[pP](([+-][0-9a-fA-F]+)|([0-9a-fA-F]*)))"
							"|(\\.[0-9a-fA-F]+)"
							"|([pP](([+-][0-9a-fA-F]+)|([0-9a-fA-F]*)))))"
					"|([0-9]+("
							"(\\.[0-9]+)[eE](([+-][0-9]+)|([0-9]*))"
							"|(\\.[0-9]+)"
							"|([eE](([+-][0-9]+)|([0-9]*)))))"
					)
			, QObject::tr("float literal")
			);

	tokenDefinitions.defineToken(LuaTokenTypes::comment, QRegularExpression("--.*"), QObject::tr("comment"));

	tokenDefinitions.defineToken(LuaTokenTypes::doubleAmpersand, QRegularExpression("&&"), "&&");
	tokenDefinitions.defineToken(LuaTokenTypes::doubleVerticalLine, QRegularExpression("\\|\\|"), "||");

	return tokenDefinitions;
}
