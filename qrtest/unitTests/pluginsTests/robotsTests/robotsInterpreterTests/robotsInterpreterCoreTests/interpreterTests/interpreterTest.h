#pragma once

#include <gtest/gtest.h>

#include <src/interpreter/interpreterInterface.h>
#include <robotsInterpreterCore/robotModel/robotModel.h>
#include <robotsInterpreterCore/robotModel/robotModelInterfaceMock.h>

#include "support/qrguiFacade.h"
#include "support/dummyBlocksFactory.h"

namespace qrTest {
namespace robotsInterpreterCoreTests {

class InterpreterTest : public testing::Test
{
protected:
	virtual void SetUp();

	virtual void TearDown();

	robotsInterpreterCore::interpreter::InterpreterInterface *mInterpreter;  // Has ownership.
	QrguiFacade *mQrguiFacade;  // Has ownership.
	DummyBlockFactory *mBlocksFactory;  // Does not have ownership.
	qrTest::RobotModelInterfaceMock mModel;
};

}
}
