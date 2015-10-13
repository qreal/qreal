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

#pragma once

#include <QtCore/QScopedPointer>
#include <QtCore/QEventLoop>
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
	QEventLoop mEventLoop;
	bool mInterpreterStopped = false;
};

}
}
}
