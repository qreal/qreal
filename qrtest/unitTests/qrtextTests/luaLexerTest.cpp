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

#include "luaLexerTest.h"

#include "gtest/gtest.h"

using namespace qrtext::lua::details;
using namespace qrtext::core;
using namespace qrTest;

void LuaLexerTest::SetUp()
{
	mErrors.clear();
	mLexer.reset(new LuaLexer(mErrors));
}

TEST_F(LuaLexerTest, tokenizeSanityCheck)
{
	const QString stream = "  123 abc";

	auto result = mLexer->tokenize(stream);

	EXPECT_TRUE(mErrors.isEmpty());
	ASSERT_EQ(2, result.size());
	EXPECT_EQ(LuaTokenTypes::integerLiteral, result[0].token());
	EXPECT_EQ(LuaTokenTypes::identifier, result[1].token());
}

TEST_F(LuaLexerTest, connections)
{
	const QString stream = "  123 abc\n  456";

	auto result = mLexer->tokenize(stream);

	ASSERT_EQ(3, result.size());

	EXPECT_EQ(Connection(2, 0, 2), result[0].range().start());
	EXPECT_EQ(Connection(4, 0, 4), result[0].range().end());

	EXPECT_EQ(Connection(6, 0, 6), result[1].range().start());
	EXPECT_EQ(Connection(8, 0, 8), result[1].range().end());

	EXPECT_EQ(Connection(12, 1, 2), result[2].range().start());
	EXPECT_EQ(Connection(14, 1, 4), result[2].range().end());
}

TEST_F(LuaLexerTest, keywords)
{
	const QString stream = "and And _and +break";

	auto result = mLexer->tokenize(stream);

	ASSERT_EQ(5, result.size());

	EXPECT_EQ(LuaTokenTypes::andKeyword, result[0].token());
	EXPECT_EQ(LuaTokenTypes::identifier, result[1].token());
	EXPECT_EQ(LuaTokenTypes::identifier, result[2].token());
	EXPECT_EQ(LuaTokenTypes::plus, result[3].token());
	EXPECT_EQ(LuaTokenTypes::breakKeyword, result[4].token());
}

TEST_F(LuaLexerTest, lexemeTypes)
{
	QString stream = "ololo and break do else elseif end false for function goto if in local nil not or repeat"
			" return then true until while";

	auto result = mLexer->tokenize(stream);
	ASSERT_EQ(23, result.size());

	EXPECT_EQ(LuaTokenTypes::identifier, result[0].token());
	EXPECT_EQ(LuaTokenTypes::andKeyword, result[1].token());
	EXPECT_EQ(LuaTokenTypes::breakKeyword, result[2].token());
	EXPECT_EQ(LuaTokenTypes::doKeyword, result[3].token());
	EXPECT_EQ(LuaTokenTypes::elseKeyword, result[4].token());
	EXPECT_EQ(LuaTokenTypes::elseifKeyword, result[5].token());
	EXPECT_EQ(LuaTokenTypes::endKeyword, result[6].token());
	EXPECT_EQ(LuaTokenTypes::falseKeyword, result[7].token());
	EXPECT_EQ(LuaTokenTypes::forKeyword, result[8].token());
	EXPECT_EQ(LuaTokenTypes::functionKeyword, result[9].token());
	EXPECT_EQ(LuaTokenTypes::gotoKeyword, result[10].token());
	EXPECT_EQ(LuaTokenTypes::ifKeyword, result[11].token());
	EXPECT_EQ(LuaTokenTypes::inKeyword, result[12].token());
	EXPECT_EQ(LuaTokenTypes::localKeyword, result[13].token());
	EXPECT_EQ(LuaTokenTypes::nilKeyword, result[14].token());
	EXPECT_EQ(LuaTokenTypes::notKeyword, result[15].token());
	EXPECT_EQ(LuaTokenTypes::orKeyword, result[16].token());
	EXPECT_EQ(LuaTokenTypes::repeatKeyword, result[17].token());
	EXPECT_EQ(LuaTokenTypes::returnKeyword, result[18].token());
	EXPECT_EQ(LuaTokenTypes::thenKeyword, result[19].token());
	EXPECT_EQ(LuaTokenTypes::trueKeyword, result[20].token());
	EXPECT_EQ(LuaTokenTypes::untilKeyword, result[21].token());
	EXPECT_EQ(LuaTokenTypes::whileKeyword, result[22].token());

	stream = "+ - * / % ^ #";
	result = mLexer->tokenize(stream);
	ASSERT_EQ(7, result.size());

	EXPECT_EQ(LuaTokenTypes::plus, result[0].token());
	EXPECT_EQ(LuaTokenTypes::minus, result[1].token());
	EXPECT_EQ(LuaTokenTypes::asterick, result[2].token());
	EXPECT_EQ(LuaTokenTypes::slash, result[3].token());
	EXPECT_EQ(LuaTokenTypes::percent, result[4].token());
	EXPECT_EQ(LuaTokenTypes::hat, result[5].token());
	EXPECT_EQ(LuaTokenTypes::sharp, result[6].token());

	stream = "& ~ | << >> //";
	result = mLexer->tokenize(stream);
	ASSERT_EQ(6, result.size());

	EXPECT_EQ(LuaTokenTypes::ampersand, result[0].token());
	EXPECT_EQ(LuaTokenTypes::tilda, result[1].token());
	EXPECT_EQ(LuaTokenTypes::verticalLine, result[2].token());
	EXPECT_EQ(LuaTokenTypes::doubleLess, result[3].token());
	EXPECT_EQ(LuaTokenTypes::doubleGreater, result[4].token());
	EXPECT_EQ(LuaTokenTypes::doubleSlash, result[5].token());

	stream = "== ~= != <= >= < > =";
	result = mLexer->tokenize(stream);
	ASSERT_EQ(8, result.size());

	EXPECT_EQ(LuaTokenTypes::doubleEquals, result[0].token());
	EXPECT_EQ(LuaTokenTypes::tildaEquals, result[1].token());
	EXPECT_EQ(LuaTokenTypes::exclamationMarkEquals, result[2].token());
	EXPECT_EQ(LuaTokenTypes::lessEquals, result[3].token());
	EXPECT_EQ(LuaTokenTypes::greaterEquals, result[4].token());
	EXPECT_EQ(LuaTokenTypes::less, result[5].token());
	EXPECT_EQ(LuaTokenTypes::greater, result[6].token());
	EXPECT_EQ(LuaTokenTypes::equals, result[7].token());

	stream = "( ) { } [ ] ::";
	result = mLexer->tokenize(stream);
	ASSERT_EQ(7, result.size());

	EXPECT_EQ(LuaTokenTypes::openingBracket, result[0].token());
	EXPECT_EQ(LuaTokenTypes::closingBracket, result[1].token());
	EXPECT_EQ(LuaTokenTypes::openingCurlyBracket, result[2].token());
	EXPECT_EQ(LuaTokenTypes::closingCurlyBracket, result[3].token());
	EXPECT_EQ(LuaTokenTypes::openingSquareBracket, result[4].token());
	EXPECT_EQ(LuaTokenTypes::closingSquareBracket, result[5].token());
	EXPECT_EQ(LuaTokenTypes::doubleColon, result[6].token());

	stream = "; : , . .. ...";
	result = mLexer->tokenize(stream);
	ASSERT_EQ(6, result.size());

	EXPECT_EQ(LuaTokenTypes::semicolon, result[0].token());
	EXPECT_EQ(LuaTokenTypes::colon, result[1].token());
	EXPECT_EQ(LuaTokenTypes::comma, result[2].token());
	EXPECT_EQ(LuaTokenTypes::dot, result[3].token());
	EXPECT_EQ(LuaTokenTypes::doubleDot, result[4].token());
	EXPECT_EQ(LuaTokenTypes::tripleDot, result[5].token());

	stream = "|| &&";
	result = mLexer->tokenize(stream);
	ASSERT_EQ(2, result.size());

	EXPECT_EQ(LuaTokenTypes::doubleVerticalLine, result[0].token());
	EXPECT_EQ(LuaTokenTypes::doubleAmpersand, result[1].token());
}

TEST_F(LuaLexerTest, errorReporting)
{
	const QString stream = "ololo @~= !!! nil";

	auto result = mLexer->tokenize(stream);

	ASSERT_EQ(2, result.size());

	EXPECT_EQ(LuaTokenTypes::identifier, result[0].token());
	EXPECT_EQ(LuaTokenTypes::nilKeyword, result[1].token());

	EXPECT_FALSE(mErrors[0].errorMessage().isEmpty());
	EXPECT_EQ(Connection(6, 0, 6), mErrors[0].connection());
	EXPECT_EQ(ErrorType::lexicalError, mErrors[0].errorType());
	EXPECT_EQ(Severity::error, mErrors[0].severity());

	EXPECT_FALSE(mErrors[1].errorMessage().isEmpty());
	EXPECT_EQ(Connection(10, 0, 10), mErrors[1].connection());
	EXPECT_EQ(ErrorType::lexicalError, mErrors[1].errorType());
	EXPECT_EQ(Severity::error, mErrors[1].severity());

	EXPECT_EQ(14, result[1].range().start().absolutePosition());
}

TEST_F(LuaLexerTest, multilineErrorReporting)
{
	const QString stream = "ololo @~=\n !!! nil";

	auto result = mLexer->tokenize(stream);

	ASSERT_EQ(2, result.size());
	ASSERT_EQ(2, mErrors.size());

	EXPECT_EQ(LuaTokenTypes::identifier, result[0].token());
	EXPECT_EQ(LuaTokenTypes::nilKeyword, result[1].token());

	EXPECT_FALSE(mErrors[0].errorMessage().isEmpty());
	EXPECT_EQ(Connection(6, 0, 6), mErrors[0].connection());
	EXPECT_EQ(ErrorType::lexicalError, mErrors[0].errorType());
	EXPECT_EQ(Severity::error, mErrors[0].severity());

	EXPECT_FALSE( mErrors[1].errorMessage().isEmpty());
	EXPECT_EQ(Connection(11, 1, 1), mErrors[1].connection());
	EXPECT_EQ(ErrorType::lexicalError, mErrors[1].errorType());
	EXPECT_EQ(Severity::error, mErrors[1].severity());

	EXPECT_EQ(15, result[1].range().start().absolutePosition());
}

TEST_F(LuaLexerTest, unicode)
{
	const QString stream = "ололо русский\n язык";

	auto result = mLexer->tokenize(stream);

	ASSERT_EQ(3, result.size());

	EXPECT_EQ(Connection(0, 0, 0), result[0].range().start());
	EXPECT_EQ(Connection(4, 0, 4), result[0].range().end());

	EXPECT_EQ(Connection(6, 0, 6), result[1].range().start());
	EXPECT_EQ(Connection(12, 0, 12), result[1].range().end());

	EXPECT_EQ(Connection(15, 1, 1), result[2].range().start());
	EXPECT_EQ(Connection(18, 1, 4), result[2].range().end());
}

TEST_F(LuaLexerTest, numericLiterals)
{
	QString stream = "3   345   0xff   0xBEBADA";

	auto result = mLexer->tokenize(stream);

	ASSERT_EQ(4, result.size());

	for (auto const &token : result) {
		EXPECT_EQ(LuaTokenTypes::integerLiteral, token.token());
	}

	stream = "3.0     3.1416     314.16e-2     0.31416E1     34e1\n"
			"0x0.1E  0xA23p-4   0X1.921FB54442D18P+1";

	result = mLexer->tokenize(stream);

	ASSERT_EQ(8, result.size());

	for (auto const &token : result) {
		EXPECT_EQ(LuaTokenTypes::floatLiteral, token.token());
	}
}

TEST_F(LuaLexerTest, strings)
{
	QString stream = "a = 'alo\\n123\"'\n"
			"   a = \"alo\\n123\\\"\"\n"
			"   a = '\\97lo\\10\\04923\"'"
			;

	auto result = mLexer->tokenize(stream);

	ASSERT_EQ(9, result.size());

	EXPECT_EQ(LuaTokenTypes::identifier, result[0].token());
	EXPECT_EQ(LuaTokenTypes::equals, result[1].token());
	EXPECT_EQ(LuaTokenTypes::string, result[2].token());

	EXPECT_EQ(LuaTokenTypes::identifier, result[3].token());
	EXPECT_EQ(LuaTokenTypes::equals, result[4].token());
	EXPECT_EQ(LuaTokenTypes::string, result[5].token());

	EXPECT_EQ(LuaTokenTypes::identifier, result[6].token());
	EXPECT_EQ(LuaTokenTypes::equals, result[7].token());
	EXPECT_EQ(LuaTokenTypes::string, result[8].token());

	stream = " x = \"asd\\\" sd\" y = 'asd'";
	result = mLexer->tokenize(stream);

	ASSERT_EQ(6, result.size());

	EXPECT_EQ(LuaTokenTypes::identifier, result[0].token());
	EXPECT_EQ(LuaTokenTypes::equals, result[1].token());
	EXPECT_EQ(LuaTokenTypes::string, result[2].token());

	EXPECT_EQ(LuaTokenTypes::identifier, result[3].token());
	EXPECT_EQ(LuaTokenTypes::equals, result[4].token());
	EXPECT_EQ(LuaTokenTypes::string, result[5].token());

	EXPECT_EQ(Connection(5, 0, 5), result[2].range().start());
	EXPECT_EQ(Connection(14, 0, 14), result[2].range().end());

	stream = " a = 'asd\nef' end ";
	result = mLexer->tokenize(stream);

	ASSERT_EQ(4, result.size());
	EXPECT_EQ(LuaTokenTypes::identifier, result[0].token());
	EXPECT_EQ(LuaTokenTypes::equals, result[1].token());
	EXPECT_EQ(LuaTokenTypes::string, result[2].token());
	EXPECT_EQ(LuaTokenTypes::endKeyword, result[3].token());

	EXPECT_EQ(Connection(5, 0, 5), result[2].range().start());
	EXPECT_EQ(Connection(12, 1, 2), result[2].range().end());

	EXPECT_EQ(Connection(14, 1, 4), result[3].range().start());
	EXPECT_EQ(Connection(16, 1, 6), result[3].range().end());

	stream = " a = 'asd\nef\ngh' end ";
	result = mLexer->tokenize(stream);

	ASSERT_EQ(4, result.size());
	EXPECT_EQ(LuaTokenTypes::identifier, result[0].token());
	EXPECT_EQ(LuaTokenTypes::equals, result[1].token());
	EXPECT_EQ(LuaTokenTypes::string, result[2].token());
	EXPECT_EQ(LuaTokenTypes::endKeyword, result[3].token());

	EXPECT_EQ(Connection(5, 0, 5), result[2].range().start());
	EXPECT_EQ(Connection(15, 2, 2), result[2].range().end());

	EXPECT_EQ(Connection(17, 2, 4), result[3].range().start());
	EXPECT_EQ(Connection(19, 2, 6), result[3].range().end());
}

TEST_F(LuaLexerTest, comments)
{
	QString stream = "a = 1 --ololo\n"
			"---- x\n"
			"a = 'no -- comment in a string' end\n"
			"--"
			;

	auto result = mLexer->tokenize(stream);
	auto comments = mLexer->comments();

	ASSERT_EQ(7, result.size());
	ASSERT_EQ(3, comments.size());


	EXPECT_EQ(LuaTokenTypes::identifier, result[0].token());
	EXPECT_EQ(LuaTokenTypes::equals, result[1].token());
	EXPECT_EQ(LuaTokenTypes::integerLiteral, result[2].token());
	EXPECT_EQ(LuaTokenTypes::identifier, result[3].token());
	EXPECT_EQ(LuaTokenTypes::equals, result[4].token());
	EXPECT_EQ(LuaTokenTypes::string, result[5].token());
	EXPECT_EQ(LuaTokenTypes::endKeyword, result[6].token());

	EXPECT_EQ(Connection(6, 0, 6), comments[0].range().start());
	EXPECT_EQ(Connection(12, 0, 12), comments[0].range().end());

	EXPECT_EQ(Connection(14, 1, 0), comments[1].range().start());
	EXPECT_EQ(Connection(19, 1, 5), comments[1].range().end());

	EXPECT_EQ(Connection(57, 3, 0), comments[2].range().start());
	EXPECT_EQ(Connection(58, 3, 1), comments[2].range().end());
}
