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

#include "qrguiFacade.h"

using namespace qrTest::robotsTests::interpreterCoreTests;
using namespace ::testing;

QrguiFacade::QrguiFacade(QString const &modelName)
	: mModels(modelName, mEditorManager)
{
	ON_CALL(mMainWindowInterpretersInterfaceMock, errorReporter()).WillByDefault(Return(&mErrorReporterMock));

	ON_CALL(mMainWindowInterpretersInterfaceMock, activeDiagram()).WillByDefault(Invoke(
			[this] () { return mActiveTab; }
	));

	ON_CALL(mMainWindowInterpretersInterfaceMock, highlight(_, _, _)).WillByDefault(Return());
	ON_CALL(mMainWindowInterpretersInterfaceMock, dehighlight(_)).WillByDefault(Return());
	ON_CALL(mMainWindowInterpretersInterfaceMock, dehighlight()).WillByDefault(Return());

	EXPECT_CALL(mMainWindowInterpretersInterfaceMock, errorReporter()).Times(AtLeast(0));
	EXPECT_CALL(mMainWindowInterpretersInterfaceMock, activeDiagram()).Times(AtLeast(0));
	EXPECT_CALL(mMainWindowInterpretersInterfaceMock, highlight(_, _, _)).Times(AtLeast(0));
	EXPECT_CALL(mMainWindowInterpretersInterfaceMock, dehighlight(_)).Times(AtLeast(0));
	EXPECT_CALL(mMainWindowInterpretersInterfaceMock, dehighlight()).Times(AtLeast(0));

	EXPECT_CALL(mErrorReporterMock, addInformation(_, _)).Times(AtLeast(0));
	EXPECT_CALL(mErrorReporterMock, addWarning(_, _)).Times(AtLeast(0));
	EXPECT_CALL(mErrorReporterMock, addError(_, _)).Times(AtLeast(0));

	EXPECT_CALL(mErrorReporterMock, clear()).Times(AtLeast(0));
}

qReal::gui::MainWindowInterpretersInterface &QrguiFacade::mainWindowInterpretersInterface()
{
	return mMainWindowInterpretersInterfaceMock;
}

qReal::GraphicalModelAssistInterface &QrguiFacade::graphicalModelAssistInterface()
{
	return mModels.graphicalModelAssistApi();
}

qReal::LogicalModelAssistInterface &QrguiFacade::logicalModelAssistInterface()
{
	return mModels.logicalModelAssistApi();
}

qReal::ProjectManagementInterface &QrguiFacade::projectManagementInterface()
{
	return mProjectManagementInterfaceMock;
}

qReal::SystemEvents &QrguiFacade::systemEvents()
{
	return mSystemEvents;
}

void QrguiFacade::setActiveTab(qReal::Id const &id)
{
	mActiveTab = id;
	emit systemEvents().activeTabChanged(qReal::TabInfo(id, nullptr));
}
