#include "../../../../qrutils/expressionsParser/number.h"

#include "gtest/gtest.h"

using namespace utils;

TEST(NumberTest, ariphmeticsIntTest) {
	Number n1(2, Number::intType);
	Number n2(3, Number::intType);

	n1 += n2;
	EXPECT_EQ(n1.property("Number").toInt(), 5);

	n1 -= n2;
	EXPECT_EQ(n1.property("Number").toInt(), 2);

	n1 *= n2;
	EXPECT_EQ(n1.property("Number").toInt(), 6);

	n1 /= n2;
	EXPECT_EQ(n1.property("Number").toInt(), 2);

	n1 /= n2;
	EXPECT_EQ(n1.property("Number").toInt(), 0);

	n1 = -n2;
	EXPECT_EQ(n1.property("Number").toInt(), -3);
}

TEST(NumberTest, ariphmeticsDoubleTest) {
	Number n1(2.3, Number::doubleType);
	Number n2(2.2, Number::doubleType);

	n1 += n2;
	EXPECT_EQ(n1.property("Number").toDouble(), 4.5);

	n1 -= n2;
	EXPECT_EQ(n1.property("Number").toDouble(), 2.3);

	n1 *= n2;
	EXPECT_EQ(n1.property("Number").toDouble(), 5.06);

	n1 /= n2;
	EXPECT_EQ(n1.property("Number").toDouble(), 2.3);

	n2 = -n1;
	EXPECT_EQ(n2.property("Number").toDouble(), -2.3);
}

TEST(NumberTest, ariphmeticsBothTypesTest) {
	Number n1(2, Number::intType);
	Number n2(2.5, Number::doubleType);

	n1 += n2;
	EXPECT_EQ(n1.property("Type"), Number::doubleType);
	EXPECT_EQ(n1.property("Number").toDouble(), 4.5);

	n1.setProperty("Type", Number::intType);
	n1 -= n2;
	EXPECT_EQ(n1.property("Type"), Number::doubleType);
	EXPECT_EQ(n1.property("Number").toDouble(), 2.0);

	n1.setProperty("Type", Number::intType);
	n1 *= n2;
	EXPECT_EQ(n1.property("Type"), Number::doubleType);
	EXPECT_EQ(n1.property("Number").toDouble(), 5.0);

	n1.setProperty("Type", Number::intType);
	n1 /= n2;
	EXPECT_EQ(n1.property("Type"), Number::doubleType);
	EXPECT_EQ(n1.property("Number").toDouble(), 2.0);
}

TEST(NumberTest, comparisonTest) {
	Number n1(2, Number::intType);
	Number n2(3, Number::intType);
	Number n3(2.5, Number::doubleType);
	Number n4(5.5, Number::doubleType);

	EXPECT_TRUE(n1 < n2);
	EXPECT_TRUE(n1 <= n2);
	EXPECT_TRUE(n1 != n2);
	EXPECT_FALSE(n1 > n2);
	EXPECT_FALSE(n1 >= n2);
	EXPECT_FALSE(n1 == n2);

	EXPECT_FALSE(n2 < n1);
	EXPECT_FALSE(n2 <= n1);
	EXPECT_FALSE(n2 == n1);
	EXPECT_TRUE(n2 != n1);
	EXPECT_TRUE(n2 > n1);
	EXPECT_TRUE(n2 >= n1);

	EXPECT_TRUE(n3 < n4);
	EXPECT_TRUE(n3 <= n4);
	EXPECT_TRUE(n3 != n4);
	EXPECT_FALSE(n3 > n4);
	EXPECT_FALSE(n3 >= n4);
	EXPECT_FALSE(n3 == n4);

	EXPECT_TRUE(n4 > n3);
	EXPECT_TRUE(n4 >= n3);
	EXPECT_TRUE(n4 != n3);
	EXPECT_FALSE(n4 < n3);
	EXPECT_FALSE(n4 <= n3);
	EXPECT_FALSE(n4 == n3);

	EXPECT_TRUE(n1 < n3);
	EXPECT_TRUE(n1 <= n3);
	EXPECT_TRUE(n1 != n3);
	EXPECT_FALSE(n1 > n3);
	EXPECT_FALSE(n1 >= n3);
	EXPECT_FALSE(n1 == n3);
}
