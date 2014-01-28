#pragma once

#include <QtCore/QScopedPointer>

#include <gtest/gtest.h>

#include <src/interpreter/interpreterInterface.h>
#include <interpreterBase/robotModel/robotModelInterfaceMock.h>

#include "support/qrguiFacade.h"
#include "support/dummyBlocksFactory.h"

namespace qrTest {
namespace robotsTests {
namespace interpreterCoreTests {

class InterpreterTest : public testing::Test
{
protected:
	void SetUp() override;

	QScopedPointer<interpreterCore::interpreter::InterpreterInterface> mInterpreter;
	QScopedPointer<QrguiFacade> mQrguiFacade;
	qrTest::RobotModelInterfaceMock mModel;
};

}
}
}
