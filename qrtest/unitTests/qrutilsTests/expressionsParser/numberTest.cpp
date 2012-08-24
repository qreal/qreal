#include "../../../qrutils/expressionsParser/number.h"

#include "gtest/gtest.h"

using namespace utils;

TEST(NumberTest, sumIntTest) {
	Number n1(2, Number::intType);
	Number n2(3, Number::intType);
	n1 += n2;
	EXPECT_EQ(n1.property("Number").toInt(), 5);
}

TEST(NumberTest, sumDoubleTest) {
	Number n1(2.3, Number::doubleType);
	Number n2(2.2, Number::doubleType);
	n1 += n2;
	EXPECT_EQ(n1.property("Number").toDouble(), 4.5);
}

TEST(NumberTest, sumIntDoubleTest) {
	Number n1(2, Number::intType);
	Number n2(2.5, Number::doubleType);
	n1 += n2;
	EXPECT_EQ(n1.property("Number").toDouble(), 4.5);
}

TEST(NumberTest, comparisonTest) {
	Number n1(2, Number::intType);
	Number n2(3, Number::intType);
	
	EXPECT_TRUE(n1 < n2);
}
