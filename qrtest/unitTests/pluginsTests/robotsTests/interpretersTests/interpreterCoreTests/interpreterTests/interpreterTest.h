#pragma once

#include <QtCore/QScopedPointer>
#include <QtWidgets/QAction>

#include <gtest/gtest.h>

#include <interpreterCore/interpreter/interpreterInterface.h>
#include <interpreterCore/textLanguage/robotsBlockParser.h>
#include <kitBase/robotModel/robotModelInterfaceMock.h>
#include <kitBase/robotModel/robotModelManagerInterfaceMock.h>
#include <kitBase/robotModel/configurationInterfaceMock.h>
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
	QScopedPointer<interpreterCore::textLanguage::RobotsBlockParser> mParser;
};

}
}
}
