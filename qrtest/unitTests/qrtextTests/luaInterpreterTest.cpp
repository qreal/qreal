#include "luaInterpreterTest.h"

#include "gtest/gtest.h"

#include "qrtext/lua/types/float.h"
#include "qrtext/lua/types/integer.h"

using namespace qrTest;
using namespace qrtext;
using namespace qrtext::lua;
using namespace qrtext::lua::details;

void LuaInterpreterTest::SetUp()
{
	mAnalyzer.reset(new LuaSemanticAnalyzer(mErrors));
	mParser.reset(new LuaParser(mErrors));
	mLexer.reset(new LuaLexer(mErrors));
	mInterpreter.reset(new LuaInterpreter(mErrors));
}

QSharedPointer<qrtext::core::ast::Node> LuaInterpreterTest::parseAndAnalyze(QString const &code)
{
	return mAnalyzer->analyze(mParser->parse(mLexer->tokenize(code)));
}

TEST_F(LuaInterpreterTest, sanityCheck)
{
	int result = interpret<int>("1");
	ASSERT_EQ(1, result);
}

TEST_F(LuaInterpreterTest, assignment)
{
	interpret<int>("a = 1");
	int result = interpret<int>("a");
	ASSERT_EQ(1, result);
}

TEST_F(LuaInterpreterTest, intrinsicFunction)
{
	mAnalyzer->addIntrinsicFunction("f", QSharedPointer<types::Function>(new types::Function(
			QSharedPointer<core::types::TypeExpression>(new types::Float()),
			{QSharedPointer<core::types::TypeExpression>(new types::Integer())}
			)));

	mInterpreter->addIntrinsicFunction("f", [](QList<QVariant> params) {
			return params[0].toInt() * params[0].toInt();
			});

	int result = interpret<int>("f(5)");

	ASSERT_TRUE(mErrors.isEmpty());
	ASSERT_EQ(25, result);
}
