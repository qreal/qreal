#include "sampleFixtureTest.h"

void BlockParserTest::SetUp() {
	blockParser = new qReal::BlockParser(NULL);
}

void BlockParserTest::TearDown() {
	delete blockParser;
}

TEST_F(BlockParserTest, expression) {
	QString const stream = "(2+3)*5";
	int pos = 0;
	EXPECT_EQ(25, blockParser->parseExpression(stream, pos).property("Number").toInt());
}

TEST_F(BlockParserTest, condition) {
	QString const stream = "2 + 3 < 5 * 5";
	int pos = 0;
	ASSERT_TRUE(blockParser->parseConditionHelper(stream, pos));
}
