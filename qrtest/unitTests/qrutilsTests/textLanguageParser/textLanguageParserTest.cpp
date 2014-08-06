#include "textLanguageParserTest.h"

#include "textLanguageParser/ast/number.h"

#include "gtest/gtest.h"

using namespace textLanguageParser;
using namespace qrTest;

void TextLanguageParserTest::SetUp()
{
	mParser.reset(new textLanguageParser::TextLanguageParser());
}

TEST_F(TextLanguageParserTest, sanityCheck)
{
	QString const stream = "123";

	auto result = mParser->parse(stream);

	EXPECT_TRUE(result.errors.isEmpty());
	EXPECT_NE(nullptr, result.astRoot);

	auto number = dynamic_cast<ast::Number *>(result.astRoot.data());

	ASSERT_NE(nullptr, number);

	EXPECT_EQ("123", number->stringRepresentation());
}
