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

#include "luaParserIncorrectInputTest.h"

#include "gtest/gtest.h"

using namespace qrtext;
using namespace qrtext::lua;
using namespace qrtext::lua::details;
using namespace qrTest;

void LuaParserIncorrectInputTest::SetUp()
{
	mErrors.clear();
	mLexer.reset(new LuaLexer(mErrors));
	mParser.reset(new LuaParser(mErrors));
}

TEST_F(LuaParserIncorrectInputTest, incorrectLogicalOperators)
{
	const QString &stream = "true ||| false";
	mParser->parse(mLexer->tokenize(stream), mLexer->userFriendlyTokenNames());
	EXPECT_FALSE(mErrors.isEmpty());
}

TEST_F(LuaParserIncorrectInputTest, incorrectArraySlice)
{
	const QString &stream = "a[i, j]";
	mParser->parse(mLexer->tokenize(stream), mLexer->userFriendlyTokenNames());
	EXPECT_FALSE(mErrors.isEmpty());
}
