#include "textLanguageFacadeTest.h"

#include "gtest/gtest.h"

#include "textLanguageParser/ast/nodes/integerNumber.h"
#include "textLanguageParser/types/integer.h"

using namespace textLanguageParser;
using namespace qrTest;

void TextLanguageFacadeTest::SetUp()
{
	mFacade.reset(new TextLanguageFacade());
}

TEST_F(TextLanguageFacadeTest, sanityCheck)
{
	qReal::Id const testId = qReal::Id("1", "2", "3", "test");
	int result = mFacade->interpret<int>(testId, "test", "123");
	EXPECT_EQ(123, result);
	EXPECT_TRUE(mFacade->errors().isEmpty());
	EXPECT_TRUE(mFacade->ast(testId, "test")->is<ast::IntegerNumber>());
	EXPECT_TRUE(mFacade->type(mFacade->ast(testId, "test"))->is<types::Integer>());
}
