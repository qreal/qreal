#include "../qrutils/expressionsParser/number.h"
#include "../plugins/blockDiagram/visualDebugSupport/interpreter/blockParser.h"

#include "gtest/gtest.h"

using namespace utils;

TEST(NumberTest, sum) {
	Number n1(2, Number::intType);
	Number n2(3, Number::intType);
	n1 += n2;
	EXPECT_EQ(5, n1.property("Number").toInt());
}

TEST(ParserTest, expressionCalculation) {
	qReal::BlockParser *parser = new qReal::BlockParser(NULL);
	QString const stream = "1+1";
	int pos = 0;
	EXPECT_EQ(2, parser->parseExpression(stream, pos).property("Number").toInt());
}
