/* Copyright 2007-2016 QReal Research Group, CyberTech Labs Ltd.
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

#include "testUtils/qrguiFacade.h"

using namespace qrTest;
using namespace ::testing;

QrguiFacade::QrguiFacade(QString const &modelName)
	: mModels(modelName, mEditorManager)
{
	mActiveTab = mModels.graphicalModelAssistApi().children(mModels.graphicalModelAssistApi().rootId())[0];

	ON_CALL(mMainWindowInterpretersInterfaceMock, errorReporter()).WillByDefault(Return(&mErrorReporterMock));

	ON_CALL(mMainWindowInterpretersInterfaceMock, activeDiagram()).WillByDefault(Invoke(
			[this] () { return mActiveTab; }
	));

	ON_CALL(mMainWindowInterpretersInterfaceMock, highlight(_, _, _)).WillByDefault(Return());
	ON_CALL(mMainWindowInterpretersInterfaceMock, dehighlight(_)).WillByDefault(Return());
	ON_CALL(mMainWindowInterpretersInterfaceMock, dehighlight()).WillByDefault(Return());

	ON_CALL(mProjectManagementInterfaceMock, saveFilePath()).WillByDefault(Return(QString("./test")));
	ON_CALL(mProjectManagementInterfaceMock, save()).WillByDefault(Return());

	ON_CALL(mTextManagerMock, showInTextEditor(_, _, _)).WillByDefault(Invoke(
			[this](const QFileInfo &file, const QString & genName, const qReal::text::LanguageInfo &language) {
				Q_UNUSED(genName)
				Q_UNUSED(language)
				emit mSystemEvents.newCodeAppeared(mActiveTab, QFileInfo(file));
			}
			));

	ON_CALL(mTextManagerMock, isDefaultPath(_)).WillByDefault(Return(true));
	ON_CALL(mTextManagerMock, isModifiedEver(_)).WillByDefault(Return(false));
	ON_CALL(mTextManagerMock, generatorName(_)).WillByDefault(Return(QString("trikQts")));

	ON_CALL(mErrorReporterMock, addError(_, _)).WillByDefault(Invoke(
			[this](const QString &message, const qReal::Id &position) {
				Q_UNUSED(message)
				Q_UNUSED(position)
				emit mErrorReporterMock.error();
				mWereErrors = true;
			}));

	ON_CALL(mErrorReporterMock, clear()).WillByDefault(Invoke(
			[this]() {
				mWereErrors = false;
			}));

	ON_CALL(mErrorReporterMock, clearErrors()).WillByDefault(Invoke(
			[this]() {
				mWereErrors = false;
			}));

	ON_CALL(mErrorReporterMock, wereErrors()).WillByDefault(Invoke(
			[this]() {
				return mWereErrors;
			}));

	EXPECT_CALL(mMainWindowInterpretersInterfaceMock, errorReporter()).Times(AtLeast(0));
	EXPECT_CALL(mMainWindowInterpretersInterfaceMock, activeDiagram()).Times(AtLeast(0));
	EXPECT_CALL(mMainWindowInterpretersInterfaceMock, highlight(_, _, _)).Times(AtLeast(0));
	EXPECT_CALL(mMainWindowInterpretersInterfaceMock, dehighlight(_)).Times(AtLeast(0));
	EXPECT_CALL(mMainWindowInterpretersInterfaceMock, dehighlight()).Times(AtLeast(0));
	EXPECT_CALL(mMainWindowInterpretersInterfaceMock, activateItemOrDiagram(_, _)).Times(AtLeast(0));

	EXPECT_CALL(mErrorReporterMock, addInformation(_, _)).Times(AtLeast(0));
	EXPECT_CALL(mErrorReporterMock, addWarning(_, _)).Times(AtLeast(0));
	EXPECT_CALL(mErrorReporterMock, addError(_, _)).Times(AtLeast(0));
	EXPECT_CALL(mErrorReporterMock, clear()).Times(AtLeast(0));
	EXPECT_CALL(mErrorReporterMock, clearErrors()).Times(AtLeast(0));
	EXPECT_CALL(mErrorReporterMock, wereErrors()).Times(AtLeast(0));

	EXPECT_CALL(mProjectManagementInterfaceMock, save()).Times(AtLeast(0));
	EXPECT_CALL(mProjectManagementInterfaceMock, saveFilePath()).Times(AtLeast(0));

	EXPECT_CALL(mTextManagerMock, showInTextEditor(_, _, _)).Times(AtLeast(0));
	EXPECT_CALL(mTextManagerMock, isDefaultPath(_)).Times(AtLeast(0));
	EXPECT_CALL(mTextManagerMock, isModifiedEver(_)).Times(AtLeast(0));
	EXPECT_CALL(mTextManagerMock, generatorName(_)).Times(AtLeast(0));
}

qReal::gui::MainWindowInterpretersInterface &QrguiFacade::mainWindowInterpretersInterface()
{
	return mMainWindowInterpretersInterfaceMock;
}

qReal::gui::MainWindowDockInterface &QrguiFacade::mainWindowDockInterface()
{
	return mMainWindowDockInterfaceMock;
}

qReal::GraphicalModelAssistInterface &QrguiFacade::graphicalModelAssistInterface()
{
	return mModels.graphicalModelAssistApi();
}

qReal::LogicalModelAssistInterface &QrguiFacade::logicalModelAssistInterface()
{
	return mModels.logicalModelAssistApi();
}

qReal::Controller &QrguiFacade::controller()
{
	return mController;
}

qReal::ProjectManagementInterface &QrguiFacade::projectManagementInterface()
{
	return mProjectManagementInterfaceMock;
}

qrRepo::RepoControlInterface &QrguiFacade::repoControlInterface()
{
	return mModels.repoControlApi();
}

qReal::SystemEvents &QrguiFacade::systemEvents()
{
	return mSystemEvents;
}

qReal::gui::editor::SceneCustomizationInterface &QrguiFacade::sceneCustomizer()
{
	return mSceneCustomizationInterfaceMock;
}

qReal::TextManagerInterface &QrguiFacade::textManager()
{
	return mTextManagerMock;
}

void QrguiFacade::setActiveTab(qReal::Id const &id)
{
	mActiveTab = id;
	emit systemEvents().activeTabChanged(qReal::TabInfo(id, nullptr));
}
