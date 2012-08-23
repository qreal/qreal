#include "mockedTestClass.h"

#include "gtest/gtest.h"

using ::testing::AtLeast;

TEST(MockTest, testMethodTest) {
	MockedTestClass testObject;
	EXPECT_CALL(testObject, testMethod()).Times(AtLeast(1));
	
	testObject.testMethod();
}
