#pragma once

#include <QtCore/QString>

#include <qrgui/mainwindow/projectManager/projectManagementInterface.h>

#include <gmock/gmock.h>

namespace qrTest {

class ProjectManagementInterfaceMock : public qReal::ProjectManagementInterface {
public:
	MOCK_METHOD0(saveFilePath, QString());
	MOCK_METHOD1(openExisting, bool(const QString &fileName));
	MOCK_METHOD0(suggestToOpenExisting, bool());
	MOCK_METHOD0(openNewWithDiagram, bool());
	MOCK_METHOD1(suggestToCreateDiagram, void(bool isClosable));
	MOCK_METHOD0(suggestToImport, bool());
	MOCK_METHOD0(close, void());
	MOCK_METHOD0(save, void());
	MOCK_METHOD1(saveAs, bool (QString const &fileName));
	MOCK_METHOD0(suggestToSaveAs, bool());
	MOCK_METHOD0(saveOrSuggestToSaveAs, bool());
	MOCK_METHOD0(openEmptyWithSuggestToSaveChanges, bool());
	MOCK_METHOD1(open, bool(const QString &fileName));
	MOCK_METHOD0(suggestToSaveChangesOrCancel, bool());
	MOCK_METHOD1(setUnsavedIndicator, void(bool isUnsaved));
};

}
