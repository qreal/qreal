#pragma once

#include <QtCore/QString>
#include <QtWidgets/QApplication>

#include <qrgui/models/models.h>
#include <qrgui/pluginManager/editorManager.h>
#include <mocks/qrgui/mainwindow/mainWindowInterpretersInterfaceMock.h>
#include <mocks/qrgui/mainwindow/projectManager/projectManagementInterfaceMock.h>
#include <mocks/qrgui/toolPluginInterface/usedInterface/errorReporterMock.h>
#include <mocks/qrgui/view/sceneCustomizationInterfaceMock.h>

namespace qrTest {
namespace robotsTests {
namespace interpreterCoreTests {

class QrguiFacade
{
public:
	QrguiFacade(QString const &modelName);

	qReal::gui::MainWindowInterpretersInterface &mainWindowInterpretersInterface();
	qReal::GraphicalModelAssistInterface &graphicalModelAssistInterface();
	qReal::LogicalModelAssistInterface &logicalModelAssistInterface();
//	qReal::SceneCustomizationInterface &sceneCustomizationInterface();
	qReal::ProjectManagementInterface &projectManagementInterface();

private:
	class AppContainer {
	public:
		AppContainer()
		{
			int argc = 0;
			char *argv[] = {new char[1]{0}, new char[1]{0}};
			mApp = new QApplication(argc, argv);
		}

		~AppContainer()
		{
			delete mApp;
		}

	private:
		QApplication *mApp;  // Has ownership.
	};

	AppContainer mAppContainer;  // Used for RAII only.
	qReal::EditorManager mEditorManager;
	qReal::models::Models mModels;
	qrTest::MainWindowInterpretersInterfaceMock mMainWindowInterpretersInterfaceMock;
	qrTest::ProjectManagementInterfaceMock mProjectManagementInterfaceMock;
	qrTest::ErrorReporterMock mErrorReporterMock;
};

}
}
}
