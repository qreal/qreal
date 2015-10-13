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

#include <qrgui/plugins/toolPluginInterface/usedInterfaces/projectManagementInterface.h>

#include <gmock/gmock.h>

namespace qrTest {

class ProjectManagementInterfaceMock : public qReal::ProjectManagementInterface {
public:
	MOCK_CONST_METHOD0(somethingOpened, bool());
	MOCK_CONST_METHOD0(saveFilePath, QString());
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
