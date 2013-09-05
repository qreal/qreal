#include "qrguiFacade.h"

using namespace qrTest::robotsInterpreterCoreTests;

QrguiFacade::QrguiFacade(QString const &modelName)
		: mModels(modelName, mEditorManager)
{

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

//qReal::SceneCustomizationInterface &QrguiFacade::sceneCustomizationInterface() const
//{

//}

qReal::ProjectManagementInterface &QrguiFacade::projectManagementInterface()
{
	return mProjectManagementInterfaceMock;
}
