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

QSharedPointer<qrtext::core::ast::Node> LuaInterpreterIncorrectInputTest::parseAndAnalyze(QString const &code)
{
	mErrors.clear();
	auto const ast = mParser->parse(mLexer->tokenize(code));
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
