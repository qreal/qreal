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

#include "lexerTest.h"

#include "gtest/gtest.h"

using namespace textLanguageParser;
using namespace qrTest;

void LexerTest::SetUp()
{
	mLexemes.reset(new TokenPatterns());
	mLexer.reset(new textLanguageParser::details::Lexer(*mLexemes));
}

TEST_F(LexerTest, tokenizeSanityCheck)
{
	QString const stream = "  123 abc";

	auto result = mLexer->tokenize(stream);

	EXPECT_TRUE(result.comments.isEmpty());
	EXPECT_TRUE(result.errors.isEmpty());
	ASSERT_EQ(2, result.tokens.size());
	EXPECT_EQ(TokenType::integerLiteral, result.tokens[0].token());
	EXPECT_EQ(TokenType::identifier, result.tokens[1].token());
}

TEST_F(LexerTest, connections)
{
	QString const stream = "  123 abc\n  456";

	auto result = mLexer->tokenize(stream);

	ASSERT_EQ(3, result.tokens.size());

	EXPECT_EQ(ast::Connection(2, 0, 2), result.tokens[0].range().start());
	EXPECT_EQ(ast::Connection(4, 0, 4), result.tokens[0].range().end());

	EXPECT_EQ(ast::Connection(6, 0, 6), result.tokens[1].range().start());
	EXPECT_EQ(ast::Connection(8, 0, 8), result.tokens[1].range().end());

	EXPECT_EQ(ast::Connection(12, 1, 2), result.tokens[2].range().start());
	EXPECT_EQ(ast::Connection(14, 1, 4), result.tokens[2].range().end());
}

TEST_F(LexerTest, customization)
{
	mLexemes->redefine(TokenType::integerLiteral, QRegularExpression("[+-]?\\d+x"));
	mLexer.reset(new textLanguageParser::details::Lexer(*mLexemes));

	QString const stream = "+12x 45x";

	auto result = mLexer->tokenize(stream);

	ASSERT_EQ(2, result.tokens.size());

	EXPECT_EQ(TokenType::integerLiteral, result.tokens[0].token());
	EXPECT_EQ(0, result.tokens[0].range().start().absolutePosition());
	EXPECT_EQ(3, result.tokens[0].range().end().absolutePosition());

	EXPECT_EQ(TokenType::integerLiteral, result.tokens[1].token());
	EXPECT_EQ(5, result.tokens[1].range().start().absolutePosition());
	EXPECT_EQ(7, result.tokens[1].range().end().absolutePosition());
}

TEST_F(LexerTest, keywords)
{
	QString const stream = "and And _and +break";

	auto result = mLexer->tokenize(stream);

	ASSERT_EQ(5, result.tokens.size());

	EXPECT_EQ(TokenType::andKeyword, result.tokens[0].token());
	EXPECT_EQ(TokenType::identifier, result.tokens[1].token());
	EXPECT_EQ(TokenType::identifier, result.tokens[2].token());
	EXPECT_EQ(TokenType::plus, result.tokens[3].token());
	EXPECT_EQ(TokenType::breakKeyword, result.tokens[4].token());
}

TEST_F(LexerTest, lexemeTypes)
{
	QString stream = "ololo and break do else elseif end false for function goto if in local nil not or repeat"
			" return then true until while";

	auto result = mLexer->tokenize(stream);
	ASSERT_EQ(23, result.tokens.size());

	EXPECT_EQ(TokenType::identifier, result.tokens[0].token());
	EXPECT_EQ(TokenType::andKeyword, result.tokens[1].token());
	EXPECT_EQ(TokenType::breakKeyword, result.tokens[2].token());
	EXPECT_EQ(TokenType::doKeyword, result.tokens[3].token());
	EXPECT_EQ(TokenType::elseKeyword, result.tokens[4].token());
	EXPECT_EQ(TokenType::elseifKeyword, result.tokens[5].token());
	EXPECT_EQ(TokenType::endKeyword, result.tokens[6].token());
	EXPECT_EQ(TokenType::falseKeyword, result.tokens[7].token());
	EXPECT_EQ(TokenType::forKeyword, result.tokens[8].token());
	EXPECT_EQ(TokenType::functionKeyword, result.tokens[9].token());
	EXPECT_EQ(TokenType::gotoKeyword, result.tokens[10].token());
	EXPECT_EQ(TokenType::ifKeyword, result.tokens[11].token());
	EXPECT_EQ(TokenType::inKeyword, result.tokens[12].token());
	EXPECT_EQ(TokenType::localKeyword, result.tokens[13].token());
	EXPECT_EQ(TokenType::nilKeyword, result.tokens[14].token());
	EXPECT_EQ(TokenType::notKeyword, result.tokens[15].token());
	EXPECT_EQ(TokenType::orKeyword, result.tokens[16].token());
	EXPECT_EQ(TokenType::repeatKeyword, result.tokens[17].token());
	EXPECT_EQ(TokenType::returnKeyword, result.tokens[18].token());
	EXPECT_EQ(TokenType::thenKeyword, result.tokens[19].token());
	EXPECT_EQ(TokenType::trueKeyword, result.tokens[20].token());
	EXPECT_EQ(TokenType::untilKeyword, result.tokens[21].token());
	EXPECT_EQ(TokenType::whileKeyword, result.tokens[22].token());

	stream = "+ - * / % ^ #";
	result = mLexer->tokenize(stream);
	ASSERT_EQ(7, result.tokens.size());

	EXPECT_EQ(TokenType::plus, result.tokens[0].token());
	EXPECT_EQ(TokenType::minus, result.tokens[1].token());
	EXPECT_EQ(TokenType::asterick, result.tokens[2].token());
	EXPECT_EQ(TokenType::slash, result.tokens[3].token());
	EXPECT_EQ(TokenType::percent, result.tokens[4].token());
	EXPECT_EQ(TokenType::hat, result.tokens[5].token());
	EXPECT_EQ(TokenType::sharp, result.tokens[6].token());

	stream = "& ~ | << >> //";
	result = mLexer->tokenize(stream);
	ASSERT_EQ(6, result.tokens.size());

	EXPECT_EQ(TokenType::ampersand, result.tokens[0].token());
	EXPECT_EQ(TokenType::tilda, result.tokens[1].token());
	EXPECT_EQ(TokenType::verticalLine, result.tokens[2].token());
	EXPECT_EQ(TokenType::doubleLess, result.tokens[3].token());
	EXPECT_EQ(TokenType::doubleGreater, result.tokens[4].token());
	EXPECT_EQ(TokenType::doubleSlash, result.tokens[5].token());

	stream = "== ~= <= >= < > =";
	result = mLexer->tokenize(stream);
	ASSERT_EQ(7, result.tokens.size());

	EXPECT_EQ(TokenType::doubleEquals, result.tokens[0].token());
	EXPECT_EQ(TokenType::tildaEquals, result.tokens[1].token());
	EXPECT_EQ(TokenType::lessEquals, result.tokens[2].token());
	EXPECT_EQ(TokenType::greaterEquals, result.tokens[3].token());
	EXPECT_EQ(TokenType::less, result.tokens[4].token());
	EXPECT_EQ(TokenType::greater, result.tokens[5].token());
	EXPECT_EQ(TokenType::equals, result.tokens[6].token());

	stream = "( ) { } [ ] ::";
	result = mLexer->tokenize(stream);
	ASSERT_EQ(7, result.tokens.size());

	EXPECT_EQ(TokenType::openingBracket, result.tokens[0].token());
	EXPECT_EQ(TokenType::closingBracket, result.tokens[1].token());
	EXPECT_EQ(TokenType::openingCurlyBracket, result.tokens[2].token());
	EXPECT_EQ(TokenType::closingCurlyBracket, result.tokens[3].token());
	EXPECT_EQ(TokenType::openingSquareBracket, result.tokens[4].token());
	EXPECT_EQ(TokenType::closingSquareBracket, result.tokens[5].token());
	EXPECT_EQ(TokenType::doubleColon, result.tokens[6].token());

	stream = "; : , . .. ...";
	result = mLexer->tokenize(stream);
	ASSERT_EQ(6, result.tokens.size());

	EXPECT_EQ(TokenType::semicolon, result.tokens[0].token());
	EXPECT_EQ(TokenType::colon, result.tokens[1].token());
	EXPECT_EQ(TokenType::comma, result.tokens[2].token());
	EXPECT_EQ(TokenType::dot, result.tokens[3].token());
	EXPECT_EQ(TokenType::doubleDot, result.tokens[4].token());
	EXPECT_EQ(TokenType::tripleDot, result.tokens[5].token());
}

TEST_F(LexerTest, errorReporting)
{
	QString const stream = "ololo @~= !!! nil";

	auto result = mLexer->tokenize(stream);

	ASSERT_EQ(2, result.tokens.size());
	ASSERT_EQ(2, result.errors.size());

	EXPECT_EQ(TokenType::identifier, result.tokens[0].token());
	EXPECT_EQ(TokenType::nilKeyword, result.tokens[1].token());

	EXPECT_EQ(QString("Lexer error"), result.errors[0].errorMessage());
	EXPECT_EQ(ast::Connection(6, 0, 6), result.errors[0].connection());
	EXPECT_EQ(ErrorType::lexicalError, result.errors[0].errorType());
	EXPECT_EQ(Severity::error, result.errors[0].severity());

	EXPECT_EQ(QString("Lexer error"), result.errors[1].errorMessage());
	EXPECT_EQ(ast::Connection(10, 0, 10), result.errors[1].connection());
	EXPECT_EQ(ErrorType::lexicalError, result.errors[1].errorType());
	EXPECT_EQ(Severity::error, result.errors[1].severity());

	EXPECT_EQ(14, result.tokens[1].range().start().absolutePosition());
}

TEST_F(LexerTest, multilineErrorReporting)
{
	QString const stream = "ololo @~=\n !!! nil";

	auto result = mLexer->tokenize(stream);

	ASSERT_EQ(2, result.tokens.size());
	ASSERT_EQ(2, result.errors.size());

	EXPECT_EQ(TokenType::identifier, result.tokens[0].token());
	EXPECT_EQ(TokenType::nilKeyword, result.tokens[1].token());

	EXPECT_EQ(QString("Lexer error"), result.errors[0].errorMessage());
	EXPECT_EQ(ast::Connection(6, 0, 6), result.errors[0].connection());
	EXPECT_EQ(ErrorType::lexicalError, result.errors[0].errorType());
	EXPECT_EQ(Severity::error, result.errors[0].severity());

	EXPECT_EQ(QString("Lexer error"), result.errors[1].errorMessage());
	EXPECT_EQ(ast::Connection(11, 1, 1), result.errors[1].connection());
	EXPECT_EQ(ErrorType::lexicalError, result.errors[1].errorType());
	EXPECT_EQ(Severity::error, result.errors[1].severity());

	EXPECT_EQ(15, result.tokens[1].range().start().absolutePosition());
}

TEST_F(LexerTest, unicode)
{
	mLexemes->redefine(TokenType::identifier, QRegularExpression("[а-яА-Я_][а-яА-Я_1-9]+"));
	mLexer.reset(new textLanguageParser::details::Lexer(*mLexemes));

	QString const stream = "ололо русский\n язык";

	auto result = mLexer->tokenize(stream);

	ASSERT_EQ(3, result.tokens.size());

	EXPECT_EQ(ast::Connection(0, 0, 0), result.tokens[0].range().start());
	EXPECT_EQ(ast::Connection(4, 0, 4), result.tokens[0].range().end());

	EXPECT_EQ(ast::Connection(6, 0, 6), result.tokens[1].range().start());
	EXPECT_EQ(ast::Connection(12, 0, 12), result.tokens[1].range().end());

	EXPECT_EQ(ast::Connection(15, 1, 1), result.tokens[2].range().start());
	EXPECT_EQ(ast::Connection(18, 1, 4), result.tokens[2].range().end());
}

TEST_F(LexerTest, numericLiterals)
{
	QString stream = "3   345   0xff   0xBEBADA";

	auto result = mLexer->tokenize(stream);

	ASSERT_EQ(4, result.tokens.size());

	for (auto token : result.tokens) {
		EXPECT_EQ(TokenType::integerLiteral, token.token());
	}

	stream = "3.0     3.1416     314.16e-2     0.31416E1     34e1\n"
			"0x0.1E  0xA23p-4   0X1.921FB54442D18P+1";

	result = mLexer->tokenize(stream);

	ASSERT_EQ(8, result.tokens.size());

	for (auto token : result.tokens) {
		EXPECT_EQ(TokenType::floatLiteral, token.token());
	}
}

TEST_F(LexerTest, strings)
{
	QString stream = "a = 'alo\\n123\"'\n"
			"   a = \"alo\\n123\\\"\"\n"
			"   a = '\\97lo\\10\\04923\"'"
			;

	auto result = mLexer->tokenize(stream);

	ASSERT_EQ(9, result.tokens.size());

	EXPECT_EQ(TokenType::identifier, result.tokens[0].token());
	EXPECT_EQ(TokenType::equals, result.tokens[1].token());
	EXPECT_EQ(TokenType::string, result.tokens[2].token());

	EXPECT_EQ(TokenType::identifier, result.tokens[3].token());
	EXPECT_EQ(TokenType::equals, result.tokens[4].token());
	EXPECT_EQ(TokenType::string, result.tokens[5].token());

	EXPECT_EQ(TokenType::identifier, result.tokens[6].token());
	EXPECT_EQ(TokenType::equals, result.tokens[7].token());
	EXPECT_EQ(TokenType::string, result.tokens[8].token());

	stream = " x = \"asd\\\" sd\" y = 'asd'";
	result = mLexer->tokenize(stream);

	ASSERT_EQ(6, result.tokens.size());

	EXPECT_EQ(TokenType::identifier, result.tokens[0].token());
	EXPECT_EQ(TokenType::equals, result.tokens[1].token());
	EXPECT_EQ(TokenType::string, result.tokens[2].token());

	EXPECT_EQ(TokenType::identifier, result.tokens[3].token());
	EXPECT_EQ(TokenType::equals, result.tokens[4].token());
	EXPECT_EQ(TokenType::string, result.tokens[5].token());

	EXPECT_EQ(ast::Connection(5, 0, 5), result.tokens[2].range().start());
	EXPECT_EQ(ast::Connection(14, 0, 14), result.tokens[2].range().end());

	stream = " a = 'asd\nef' end ";
	result = mLexer->tokenize(stream);

	ASSERT_EQ(4, result.tokens.size());
	EXPECT_EQ(TokenType::identifier, result.tokens[0].token());
	EXPECT_EQ(TokenType::equals, result.tokens[1].token());
	EXPECT_EQ(TokenType::string, result.tokens[2].token());
	EXPECT_EQ(TokenType::endKeyword, result.tokens[3].token());

	EXPECT_EQ(ast::Connection(5, 0, 5), result.tokens[2].range().start());
	EXPECT_EQ(ast::Connection(12, 1, 2), result.tokens[2].range().end());

	EXPECT_EQ(ast::Connection(14, 1, 4), result.tokens[3].range().start());
	EXPECT_EQ(ast::Connection(16, 1, 6), result.tokens[3].range().end());

	stream = " a = 'asd\nef\ngh' end ";
	result = mLexer->tokenize(stream);

	ASSERT_EQ(4, result.tokens.size());
	EXPECT_EQ(TokenType::identifier, result.tokens[0].token());
	EXPECT_EQ(TokenType::equals, result.tokens[1].token());
	EXPECT_EQ(TokenType::string, result.tokens[2].token());
	EXPECT_EQ(TokenType::endKeyword, result.tokens[3].token());

	EXPECT_EQ(ast::Connection(5, 0, 5), result.tokens[2].range().start());
	EXPECT_EQ(ast::Connection(15, 2, 2), result.tokens[2].range().end());

	EXPECT_EQ(ast::Connection(17, 2, 4), result.tokens[3].range().start());
	EXPECT_EQ(ast::Connection(19, 2, 6), result.tokens[3].range().end());
}

TEST_F(LexerTest, comments)
{
	QString stream = "a = 1 --ololo\n"
			"---- x\n"
			"a = 'no -- comment in a string' end\n"
			"--"
			;

	auto result = mLexer->tokenize(stream);

	ASSERT_EQ(7, result.tokens.size());
	ASSERT_EQ(3, result.comments.size());

	EXPECT_EQ(TokenType::identifier, result.tokens[0].token());
	EXPECT_EQ(TokenType::equals, result.tokens[1].token());
	EXPECT_EQ(TokenType::integerLiteral, result.tokens[2].token());
	EXPECT_EQ(TokenType::identifier, result.tokens[3].token());
	EXPECT_EQ(TokenType::equals, result.tokens[4].token());
	EXPECT_EQ(TokenType::string, result.tokens[5].token());
	EXPECT_EQ(TokenType::endKeyword, result.tokens[6].token());

	EXPECT_EQ(ast::Connection(6, 0, 6), result.comments[0].range().start());
	EXPECT_EQ(ast::Connection(12, 0, 12), result.comments[0].range().end());

	EXPECT_EQ(ast::Connection(14, 1, 0), result.comments[1].range().start());
	EXPECT_EQ(ast::Connection(19, 1, 5), result.comments[1].range().end());

	EXPECT_EQ(ast::Connection(57, 3, 0), result.comments[2].range().start());
	EXPECT_EQ(ast::Connection(58, 3, 1), result.comments[2].range().end());
}
