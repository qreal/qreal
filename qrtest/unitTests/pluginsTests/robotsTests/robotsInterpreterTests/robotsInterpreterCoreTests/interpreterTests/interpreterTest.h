#pragma once

#include <gtest/gtest.h>

#include <src/interpreter/interpreterInterface.h>

namespace qrTest {
namespace robotsInterpreterCoreTests {

class InterpreterTest : public testing::Test
{
protected:
	virtual void SetUp();

	virtual void TearDown();

	robotsInterpreterCore::interpreter::InterpreterInterface *mInterpreter;  // Has ownership.
};

}
}
