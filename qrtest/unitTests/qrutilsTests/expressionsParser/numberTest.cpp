/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "../../../../qrutils/expressionsParser/number.h"

#include "gtest/gtest.h"

using namespace utils;

TEST(NumberTest, ariphmeticsIntTest) {
	Number n1(2, Number::intType);
	Number n2(3, Number::intType);

	n1 += n2;
	EXPECT_EQ(n1.value().toInt(), 5);

	n1 -= n2;
	EXPECT_EQ(n1.value().toInt(), 2);

	n1 *= n2;
	EXPECT_EQ(n1.value().toInt(), 6);

	n1 /= n2;
	EXPECT_EQ(n1.value().toInt(), 2);

	n1 /= n2;
	EXPECT_EQ(n1.value().toInt(), 0);

	n1 = -n2;
	EXPECT_EQ(n1.value().toInt(), -3);
}

TEST(NumberTest, ariphmeticsDoubleTest) {
	Number n1(2.3, Number::doubleType);
	Number n2(2.2, Number::doubleType);

	n1 += n2;
	EXPECT_EQ(n1.value().toDouble(), 4.5);

	n1 -= n2;
	EXPECT_EQ(n1.value().toDouble(), 2.3);

	n1 *= n2;
	EXPECT_EQ(n1.value().toDouble(), 5.06);

	n1 /= n2;
	EXPECT_EQ(n1.value().toDouble(), 2.3);

	n2 = -n1;
	EXPECT_EQ(n2.value().toDouble(), -2.3);
}

TEST(NumberTest, ariphmeticsBothTypesTest) {
	Number n1(2, Number::intType);
	Number n2(2.5, Number::doubleType);

	n1 += n2;
	EXPECT_EQ(n1.type(), Number::doubleType);
	EXPECT_EQ(n1.value().toDouble(), 4.5);

	n1.setType(Number::intType);
	n1 -= n2;
	EXPECT_EQ(n1.type(), Number::doubleType);
	EXPECT_EQ(n1.value().toDouble(), 2.0);

	n1.setType(Number::intType);
	n1 *= n2;
	EXPECT_EQ(n1.type(), Number::doubleType);
	EXPECT_EQ(n1.value().toDouble(), 5.0);

	n1.setType(Number::intType);
	n1 /= n2;
	EXPECT_EQ(n1.type(), Number::doubleType);
	EXPECT_EQ(n1.value().toDouble(), 2.0);
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
