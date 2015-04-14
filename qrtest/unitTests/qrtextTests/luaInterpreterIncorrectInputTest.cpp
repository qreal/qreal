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

#include "luaInterpreterIncorrectInputTest.h"

#include "gtest/gtest.h"

#include "qrtext/lua/types/float.h"
#include "qrtext/lua/types/integer.h"

using namespace qrTest;
using namespace qrtext;
using namespace qrtext::lua;
using namespace qrtext::lua::details;

void LuaInterpreterIncorrectInputTest::SetUp()
{
	mAnalyzer.reset(new LuaSemanticAnalyzer(mErrors));
	mParser.reset(new LuaParser(mErrors));
	mLexer.reset(new LuaLexer(mErrors));
	mInterpreter.reset(new LuaInterpreter(mErrors));
}

QSharedPointer<qrtext::core::ast::Node> LuaInterpreterIncorrectInputTest::parseAndAnalyze(const QString &code)
{
	mErrors.clear();
	auto const ast = mParser->parse(mLexer->tokenize(code), mLexer->userFriendlyTokenNames());
	if (mErrors.isEmpty()) {
		return mAnalyzer->analyze(ast);
	} else {
		return {};
	}
}

TEST_F(LuaInterpreterIncorrectInputTest, incorrectNotTrue)
{
	interpret("b = !true");

	ASSERT_FALSE(mErrors.isEmpty());
}

TEST_F(LuaInterpreterIncorrectInputTest, unknownFunctionCall)
{
	interpret("a = b();");

	ASSERT_FALSE(mErrors.isEmpty());
}

TEST_F(LuaInterpreterIncorrectInputTest, incorrectExpression)
{
	interpret("1 * + 2");
	ASSERT_FALSE(mErrors.isEmpty());

	interpret("1 *");
	ASSERT_FALSE(mErrors.isEmpty());

	interpret("* 1");
	ASSERT_FALSE(mErrors.isEmpty());
}
