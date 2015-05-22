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

#include <plugins/generationRulesTool/tokenTypes.h>

using namespace qrTest;

void LexerTest::SetUp()
{
	mErrors.clear();
	mLexer.reset(new simpleParser::Lexer(mErrors));
}

void LexerTest::TearDown()
{
	mErrors.clear();
}

TEST_F(LexerTest, lexerTestForForeachExample)
{
	QString stream = "'enum State {'\n"
			"foreach (state in State) {\n"
			"state->name ',' \n"
			"newline \n"
			"} \n"
			"'}' \n";

	const auto lexerResult = mLexer->tokenize(stream);

	ASSERT_EQ(lexerResult.length(), 15);

	EXPECT_EQ(simpleParser::TokenTypes::text, lexerResult[0].token());
	EXPECT_EQ(simpleParser::TokenTypes::foreachKeyword, lexerResult[1].token());
	EXPECT_EQ(simpleParser::TokenTypes::openingBracket, lexerResult[2].token());
	EXPECT_EQ(simpleParser::TokenTypes::identifier, lexerResult[3].token());
	EXPECT_EQ(simpleParser::TokenTypes::inKeyword, lexerResult[4].token());
	EXPECT_EQ(simpleParser::TokenTypes::identifier, lexerResult[5].token());
	EXPECT_EQ(simpleParser::TokenTypes::closingBracket, lexerResult[6].token());

	EXPECT_EQ(simpleParser::TokenTypes::openingCurlyBracket, lexerResult[7].token());
	EXPECT_EQ(simpleParser::TokenTypes::identifier, lexerResult[8].token());
	EXPECT_EQ(simpleParser::TokenTypes::arrow, lexerResult[9].token());
	EXPECT_EQ(simpleParser::TokenTypes::identifier, lexerResult[10].token());
	EXPECT_EQ(simpleParser::TokenTypes::text, lexerResult[11].token());
	EXPECT_EQ(simpleParser::TokenTypes::newlineKeyword, lexerResult[12].token());
	EXPECT_EQ(simpleParser::TokenTypes::closingCurlyBracket, lexerResult[13].token());

	EXPECT_EQ(simpleParser::TokenTypes::text, lexerResult[14].token());
}

TEST_F(LexerTest, lexerTestForIfStatement)
{
	QString stream = "if (this->Name != 'StartState') { \n"
			"newline } \n"
			"else { \n"
			"this->Name newline } \n"
			;

	const auto lexerResult = mLexer->tokenize(stream);

	ASSERT_EQ(lexerResult.length(), 18);

	EXPECT_EQ(simpleParser::TokenTypes::ifKeyword, lexerResult[0].token());
	EXPECT_EQ(simpleParser::TokenTypes::openingBracket, lexerResult[1].token());
	EXPECT_EQ(simpleParser::TokenTypes::thisKeyword, lexerResult[2].token());
	EXPECT_EQ(simpleParser::TokenTypes::arrow, lexerResult[3].token());
	EXPECT_EQ(simpleParser::TokenTypes::identifier, lexerResult[4].token());
	EXPECT_EQ(simpleParser::TokenTypes::notEqual, lexerResult[5].token());
	EXPECT_EQ(simpleParser::TokenTypes::text, lexerResult[6].token());
	EXPECT_EQ(simpleParser::TokenTypes::closingBracket, lexerResult[7].token());
	EXPECT_EQ(simpleParser::TokenTypes::openingCurlyBracket, lexerResult[8].token());

	EXPECT_EQ(simpleParser::TokenTypes::newlineKeyword, lexerResult[9].token());
	EXPECT_EQ(simpleParser::TokenTypes::closingCurlyBracket, lexerResult[10].token());

	EXPECT_EQ(simpleParser::TokenTypes::elseKeyword, lexerResult[11].token());
	EXPECT_EQ(simpleParser::TokenTypes::openingCurlyBracket, lexerResult[12].token());

	EXPECT_EQ(simpleParser::TokenTypes::thisKeyword, lexerResult[13].token());
	EXPECT_EQ(simpleParser::TokenTypes::arrow, lexerResult[14].token());
	EXPECT_EQ(simpleParser::TokenTypes::identifier, lexerResult[15].token());
	EXPECT_EQ(simpleParser::TokenTypes::newlineKeyword, lexerResult[16].token());
	EXPECT_EQ(simpleParser::TokenTypes::closingCurlyBracket, lexerResult[17].token());
}
