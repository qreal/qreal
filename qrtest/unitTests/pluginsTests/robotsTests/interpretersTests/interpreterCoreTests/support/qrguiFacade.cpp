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

void QrguiFacade::setActiveTab(qReal::Id const &id)
{
	mActiveTab = id;
	emit systemEvents().activeTabChanged(qReal::TabInfo(id, nullptr));
}
