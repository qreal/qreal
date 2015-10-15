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

#include <QtCore/QString>

#include <qrgui/models/models.h>
#include <qrgui/plugins/pluginManager/editorManager.h>
#include <qrgui/plugins/toolPluginInterface/systemEvents.h>
#include <mocks/qrgui/mainWindow/mainWindowInterpretersInterfaceMock.h>
#include <mocks/qrgui/mainWindow/projectManager/projectManagementInterfaceMock.h>
#include <mocks/qrgui/plugins/toolPluginInterface/usedInterface/errorReporterMock.h>
#include <mocks/qrgui/view/sceneCustomizationInterfaceMock.h>

namespace qrTest {
namespace robotsTests {
namespace interpreterCoreTests {

class QrguiFacade
{
public:
	explicit QrguiFacade(QString const &modelName);

	qReal::gui::MainWindowInterpretersInterface &mainWindowInterpretersInterface();
	qReal::GraphicalModelAssistInterface &graphicalModelAssistInterface();
	qReal::LogicalModelAssistInterface &logicalModelAssistInterface();
	qReal::ProjectManagementInterface &projectManagementInterface();
	qReal::SystemEvents &systemEvents();

	void setActiveTab(qReal::Id const &id);

private:
	qReal::EditorManager mEditorManager;
	qReal::models::Models mModels;
	qReal::SystemEvents mSystemEvents;
	qrTest::MainWindowInterpretersInterfaceMock mMainWindowInterpretersInterfaceMock;
	qrTest::ProjectManagementInterfaceMock mProjectManagementInterfaceMock;
	qrTest::ErrorReporterMock mErrorReporterMock;

	qReal::Id mActiveTab;
};

}
}
}
