#include "luaToolboxTest.h"

#include "gtest/gtest.h"

#include "qrtext/lua/ast/integerNumber.h"
#include "qrtext/lua/types/integer.h"

using namespace qrtext::lua;
using namespace qrTest;

void LuaToolboxTest::SetUp()
{
	mToolbox.reset(new LuaToolbox());
}

TEST_F(LuaToolboxTest, sanityCheck)
{
	qReal::Id const testId = qReal::Id("1", "2", "3", "test");
	int result = mToolbox->interpret<int>(testId, "test", "123");
	EXPECT_EQ(123, result);
	EXPECT_TRUE(mToolbox->errors().isEmpty());
	EXPECT_TRUE(mToolbox->ast(testId, "test")->is<ast::IntegerNumber>());
	EXPECT_TRUE(mToolbox->type(mToolbox->ast(testId, "test"))->is<types::Integer>());
}

TEST_F(LuaToolboxTest, easyTableInitializationSyntax)
{
	qReal::Id const testId = qReal::Id("1", "2", "3", "test");
	QStringList result = mToolbox->interpret<QStringList>(testId, "test", "'M1', 'M2', 'M3'");
	EXPECT_EQ(3, result.size());
	EXPECT_TRUE(mToolbox->errors().isEmpty());

	EXPECT_EQ("M1", result[0]);
	EXPECT_EQ("M2", result[1]);
	EXPECT_EQ("M3", result[2]);
}

TEST_F(LuaToolboxTest, intrinsicFunction)
{
	mToolbox->addIntrinsicFunction("f", new types::Integer(), {new types::Integer()}
			, [] (QList<QVariant> params) { return params[0].toInt() + 2; }
			);

	qReal::Id const testId = qReal::Id("1", "2", "3", "test");

	auto result = mToolbox->interpret<int>(testId, "test", "f(1)");
	EXPECT_TRUE(mToolbox->errors().isEmpty());

	EXPECT_EQ(3, result);
}

TEST_F(LuaToolboxTest, errorProcessingSanityCheck)
{
	mToolbox->interpret<int>("true ||| false");
	EXPECT_FALSE(mToolbox->errors().isEmpty());
}

TEST_F(LuaToolboxTest, tables)
{
	mToolbox->interpret<int>("a = {10; 15}");

	int result = mToolbox->interpret<int>("a[0]");
	EXPECT_TRUE(mToolbox->errors().isEmpty());
	EXPECT_EQ(10, result);

	result = mToolbox->interpret<int>("a[1]");
	EXPECT_TRUE(mToolbox->errors().isEmpty());
	EXPECT_EQ(15, result);
}

TEST_F(LuaToolboxTest, concatenation)
{
	mToolbox->interpret<int>("s = \"a = \" .. \"1\"");
	auto result = mToolbox->interpret<QString>("s");
	ASSERT_TRUE(mToolbox->errors().isEmpty());
	EXPECT_EQ("a = 1", result);
}
