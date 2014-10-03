#pragma once

#include <QtCore/QScopedPointer>
#include <QtWidgets/QAction>

#include <gtest/gtest.h>

#include <src/interpreter/interpreterInterface.h>
#include <interpreterBase/robotModel/robotModelInterfaceMock.h>
#include <interpreterBase/robotModel/robotModelManagerInterfaceMock.h>
#include <interpreterBase/robotModel/configurationInterfaceMock.h>
#include <utils/realTimeline.h>

/// @todo /src/managers
#include <interpreterCore/managers/blocksFactoryManagerInterfaceMock.h>

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
	utils::RealTimeline mTimeline;
	RobotModelInterfaceMock mModel;
	RobotModelManagerInterfaceMock mModelManager;
	QScopedPointer<QAction> mFakeConnectToRobotAction;
	BlocksFactoryManagerInterfaceMock mBlocksFactoryManager;
	ConfigurationInterfaceMock mConfigurationInterfaceMock;
};

}
}
}
