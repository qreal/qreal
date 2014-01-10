#pragma once

#include <gtest/gtest.h>

#include <src/interpreter/interpreterInterface.h>
#include <interpreterBase/robotModel/robotModel.h>
#include <interpreterBase/robotModel/robotModelInterfaceMock.h>

#include "support/qrguiFacade.h"
#include "support/dummyBlocksFactory.h"

namespace qrTest {
namespace robotsTests {
namespace interpreterCoreTests {

class InterpreterTest : public testing::Test
{
protected:
	virtual void SetUp();

	virtual void TearDown();

	interpreterCore::interpreter::InterpreterInterface *mInterpreter;  // Has ownership.
	QrguiFacade *mQrguiFacade;  // Has ownership.
	DummyBlockFactory *mBlocksFactory;  // Does not have ownership.
	qrTest::RobotModelInterfaceMock mModel;
};

}
}
}
