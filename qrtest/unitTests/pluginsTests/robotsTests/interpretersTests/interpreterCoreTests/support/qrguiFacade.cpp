#include "qrguiFacade.h"

using namespace qrTest::robotsTests::interpreterCoreTests;
using namespace ::testing;

QrguiFacade::QrguiFacade(QString const &modelName)
	: mModels(modelName, mEditorManager)
{
	ON_CALL(mMainWindowInterpretersInterfaceMock, errorReporter()).WillByDefault(Return(&mErrorReporterMock));

	ON_CALL(mMainWindowInterpretersInterfaceMock, activeDiagram())
			.WillByDefault(
				Return(
					qReal::Id::loadFromString(
						"qrm:/RobotsMetamodel/RobotsDiagram/RobotsDiagramNode/{f08fa823-e187-4755-87ba-e4269ae4e798}"
					)
				)
			);

	EXPECT_CALL(mMainWindowInterpretersInterfaceMock, errorReporter()).Times(AtLeast(0));
	EXPECT_CALL(mMainWindowInterpretersInterfaceMock, activeDiagram()).Times(AtLeast(0));

	EXPECT_CALL(mErrorReporterMock, addInformation(_, _)).Times(0);
	EXPECT_CALL(mErrorReporterMock, addWarning(_, _)).Times(0);
	EXPECT_CALL(mErrorReporterMock, addError(_, _)).Times(0);

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

//qReal::SceneCustomizationInterface &QrguiFacade::sceneCustomizationInterface() const
//{

//}

qReal::ProjectManagementInterface &QrguiFacade::projectManagementInterface()
{
	return mProjectManagementInterfaceMock;
}

qReal::SystemEvents &QrguiFacade::systemEvents()
{
	return mSystemEvents;
}
