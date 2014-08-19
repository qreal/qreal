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
