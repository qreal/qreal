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
//	qReal::SceneCustomizationInterface &sceneCustomizationInterface();
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
