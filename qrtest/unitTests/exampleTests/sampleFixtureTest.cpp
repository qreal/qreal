#include "sampleFixtureTest.h"

void BlockParserExampleTest::SetUp() {
	blockParser = new qReal::BlockParser(NULL);
}

void BlockParserExampleTest::TearDown() {
	delete blockParser;
}

TEST_F(BlockParserExampleTest, expression) {
	QString const stream = "(2+3)*5";
	int pos = 0;
	EXPECT_EQ(blockParser->parseExpression(stream, pos).property("Number").toInt(), 25);
}

TEST_F(BlockParserExampleTest, condition) {
	QString const stream = "2 + 3 < 5 * 5";
	int pos = 0;
	ASSERT_TRUE(blockParser->parseConditionHelper(stream, pos));
}
