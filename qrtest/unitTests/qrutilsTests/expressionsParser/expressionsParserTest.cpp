#include "expressionsParserTest.h"

#include "gtest/gtest.h"

using namespace utils;
using namespace qrTest;

using ::testing::Exactly;
using ::testing::_;

void ExpressionsParserTest::SetUp() {
	mParser = new ExpressionsParser(&mErrorReporter);
}

void ExpressionsParserTest::TearDown() {
	delete mParser;
}

TEST_F(ExpressionsParserTest, priorityTest) {
	QString const stream = "2 + 2 * 2";
	int pos = 0;

	EXPECT_EQ(mParser->parseExpression(stream, pos)->value().toInt(), 6);
}

TEST_F(ExpressionsParserTest, multiplicativityTest) {
	QString const stream = "(2 + 2) * 3 + 4";
	int pos = 0;

	EXPECT_EQ(mParser->parseExpression(stream, pos)->value().toInt(), 16);
}

TEST_F(ExpressionsParserTest, associativityTest) {
	QString const stream1 = "(2 + 2) + 2";
	int pos1 = 0;

	QString const stream2 = "2 + (2 + 2)";
	int pos2 = 0;

	EXPECT_EQ(mParser->parseExpression(stream1, pos1)->value().toInt(), 6);
	EXPECT_EQ(mParser->parseExpression(stream2, pos2)->value().toInt(), 6);
}

TEST_F(ExpressionsParserTest, doubleTest) {
	QString const stream = "(2.2 + 2.2) * 5";
	int pos = 0;

	EXPECT_EQ(mParser->parseExpression(stream, pos)->value().toDouble(), 22.0);
}

TEST_F(ExpressionsParserTest, unaryMinusTest) {
	QString const stream = "-2 * 2 + 3";
	int pos = 0;

	EXPECT_EQ(mParser->parseExpression(stream, pos)->value().toInt(), -1);
}

TEST_F(ExpressionsParserTest, basicComparisonTest) {
	QString const stream = "2 + 3 < 3 * 2";
	int pos = 0;

	EXPECT_TRUE(mParser->parseConditionHelper(stream, pos));
}

TEST_F(ExpressionsParserTest, conjunctionTest) {
	QString const stream = "2 < 3 && 3 > 5";
	int pos = 0;

	EXPECT_FALSE(mParser->parseConditionHelper(stream, pos));
}

TEST_F(ExpressionsParserTest, disjunctionTest) {
	QString const stream = "2 < 3 || 3 > 5";
	int pos = 0;

	EXPECT_TRUE(mParser->parseConditionHelper(stream, pos));
}

TEST_F(ExpressionsParserTest, complexComparisonTest) {
	QString const stream1 = "(2+2)*3 < 5 || 2*(6-3) < 7 && 7 < 8";
	QString const stream2 = "(2 < 5 || 8 < 7) && 7 < 8";
	int pos1 = 0;
	int pos2 = 0;

	EXPECT_TRUE(mParser->parseConditionHelper(stream1, pos1));
	EXPECT_TRUE(mParser->parseConditionHelper(stream2, pos2));
}

TEST_F(ExpressionsParserTest, negationTest) {
	QString const stream = "!(2+2 < 5)";
	int pos = 0;

	EXPECT_FALSE(mParser->parseConditionHelper(stream, pos));
}

TEST_F(ExpressionsParserTest, parseErrorTest1) {
	EXPECT_CALL(mErrorReporter, addCritical(_, _)).Times(Exactly(1));

	QString const stream = "((2+2)*5";
	int pos = 0;

	mParser->parseExpression(stream, pos);
}

TEST_F(ExpressionsParserTest, parseErrorTest2) {
	EXPECT_CALL(mErrorReporter, addCritical(_, _)).Times(Exactly(1));

	QString const stream = "2**2";
	int pos = 0;

	mParser->parseExpression(stream, pos);
}

TEST_F(ExpressionsParserTest, parseErrorTest3) {
	EXPECT_CALL(mErrorReporter, addCritical(_, _)).Times(Exactly(1));

	QString const stream = "abc + 2";
	int pos = 0;

	mParser->parseExpression(stream, pos);
}
