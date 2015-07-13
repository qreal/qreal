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

#include "parserTest.h"

using namespace qrTest;

void ParserTest::SetUp()
{
	mParserErrors.clear();
	mLexerErrors.clear();

	mLexer.reset(new simpleParser::Lexer(mLexerErrors));
	mParser.reset(new simpleParser::Parser(mParserErrors));
}

void ParserTest::TearDown()
{
	mParserErrors.clear();
	mLexerErrors.clear();
}

TEST_F(ParserTest, parserTestForForeachExample)
{
	const QString stream =
		"'enum State {' newline \n"
			"foreach (state in State) {\n"
			"tab state->Name ',' \n"
			"newline \n"
		"} \n"
			"foreach (state in StartState) {\n"
			"tab state->Name ',' \n"
			"newline \n"
		"} \n"
			"foreach (state in EndState) {\n"
			"tab state->Name ',' \n"
			"newline \n"
		"} \n"
		"'}' \n";

	const auto lexerResult = mLexer->tokenize(stream);
	const auto parserResult = mParser->parse(lexerResult, mLexer->userFriendlyTokenNames());

	ASSERT_EQ(parserResult->children().size(), 6);
}

TEST_F(ParserTest, parserTestForIfStatement)
{
	QString stream = "if (this->Name != 'StartState') { \n"
			"newline } \n"
			"else { \n"
			"this->Name newline } \n"
			;

	const auto lexerResult = mLexer->tokenize(stream);
	const auto parserResult = mParser->parse(lexerResult, mLexer->userFriendlyTokenNames());

	ASSERT_EQ(parserResult->children().size(), 1);
}
