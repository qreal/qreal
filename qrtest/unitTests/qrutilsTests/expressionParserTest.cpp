#include "expressionParserTest.h"

#include "gtest/gtest.h"

using namespace utils;
using namespace qrTest;

using ::testing::Exactly;
using ::testing::_; 

void ExpressionParserTest::SetUp() {
	mParser = new ExpressionsParser(&mErrorReporter);
}

void ExpressionParserTest::TearDown() {
	delete mParser;
}

TEST_F(ExpressionParserTest, priorityTest) {
	QString const stream = "2 + 2 * 2";
	int pos = 0;

	EXPECT_EQ(mParser->parseExpression(stream, pos).property("Number").toInt(), 6);
}

TEST_F(ExpressionParserTest, multiplicativityTest) {
	QString const stream = "(2 + 2) * 3 + 4";
	int pos = 0;

	EXPECT_EQ(mParser->parseExpression(stream, pos).property("Number").toInt(), 16);
}

TEST_F(ExpressionParserTest, associativityTest) {
	QString const stream1 = "(2 + 2) + 2";
	int pos1 = 0;

	QString const stream2 = "2 + (2 + 2)";
	int pos2 = 0;

	EXPECT_EQ(mParser->parseExpression(stream1, pos1).property("Number").toInt(), 6);
	EXPECT_EQ(mParser->parseExpression(stream2, pos2).property("Number").toInt(), 6);
}

TEST_F(ExpressionParserTest, doubleTest) {
	QString const stream = "(2.2 + 2.2) * 5";
	int pos = 0;

	EXPECT_EQ(mParser->parseExpression(stream, pos).property("Number").toDouble(), 22.0);
}

TEST_F(ExpressionParserTest, unaryMinusTest) {
	QString const stream = "-2 * 2 + 3";
	int pos = 0;

	EXPECT_EQ(mParser->parseExpression(stream, pos).property("Number").toInt(), -1);
}

TEST_F(ExpressionParserTest, basicComparisonTest) {
	QString const stream = "2 + 3 < 3 * 2";
	int pos = 0;

	EXPECT_TRUE(mParser->parseConditionHelper(stream, pos));
}

TEST_F(ExpressionParserTest, conjunctionTest) {
	QString const stream = "2 < 3 && 3 > 5";
	int pos = 0;

	EXPECT_FALSE(mParser->parseConditionHelper(stream, pos));
}

TEST_F(ExpressionParserTest, disjunctionTest) {
	QString const stream = "2 < 3 || 3 > 5";
	int pos = 0;

	EXPECT_TRUE(mParser->parseConditionHelper(stream, pos));
}

TEST_F(ExpressionParserTest, complexComparisonTest) {
	QString const stream1 = "(2+2)*3 < 5 || 2*(6-3) < 7 && 7 < 8";
	QString const stream2 = "(2 < 5 || 8 < 7) && 7 < 8";
	int pos1 = 0;
	int pos2 = 0;

	EXPECT_TRUE(mParser->parseConditionHelper(stream1, pos1));
	EXPECT_TRUE(mParser->parseConditionHelper(stream2, pos2));
}

TEST_F(ExpressionParserTest, negationTest) {
	QString const stream = "!(2+2 < 5)";
	int pos = 0;

	EXPECT_FALSE(mParser->parseConditionHelper(stream, pos));
}

TEST_F(ExpressionParserTest, parseErrorTest1) {
	EXPECT_CALL(mErrorReporter, addCritical(_, _)).Times(Exactly(1));

	QString const stream = "((2+2)*5";
	int pos = 0;

	mParser->parseExpression(stream, pos);
}

TEST_F(ExpressionParserTest, parseErrorTest2) {
	EXPECT_CALL(mErrorReporter, addCritical(_, _)).Times(Exactly(1));

	QString const stream = "2**2";
	int pos = 0;

	mParser->parseExpression(stream, pos);
}
