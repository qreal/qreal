#pragma once

#include "testClass.h"
#include "gmock/gmock.h"

class MockedTestClass : public TestClass {
public:
	MOCK_METHOD0(testMethod, void());
};
