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

#pragma once

#include <QtCore/QString>

#include <qrgui/editor/sceneCustomizationInterface.h>

#include <gmock/gmock.h>

namespace qrTest {

/// Mock class for QrGui scene customization interface.
class SceneCustomizationInterfaceMock : public qReal::gui::editor::SceneCustomizationInterface
{
public:
	MOCK_CONST_METHOD0(addExplosionMenuName, QString());
	MOCK_CONST_METHOD0(changeExplosionMenuName, QString());
	MOCK_CONST_METHOD0(deleteExplosionMenuName, QString());
	MOCK_CONST_METHOD0(goToConnectedMenuName, QString());
	MOCK_CONST_METHOD0(showExpandConnectionAction, bool());
	MOCK_CONST_METHOD0(expandExplosionActionText, QString());
	MOCK_CONST_METHOD0(collapseExplosionActionText, QString());
	MOCK_CONST_METHOD0(allowSubprogramPropertiesChanging, bool());
	MOCK_METHOD1(setAddExplosionMenuName, void(const QString &));
	MOCK_METHOD1(setChangeExplosionMenuName, void(const QString &));
	MOCK_METHOD1(setDeleteExplosionMenuName, void(const QString &));
	MOCK_METHOD1(setGoToConnectedMenuName, void(const QString &));
	MOCK_METHOD1(setShowExpandConnectionAction, void(bool));
	MOCK_METHOD1(setExpandExplosionActionText, void(const QString &));
	MOCK_METHOD1(setCollapseExplosionActionText, void(const QString &));
	MOCK_METHOD1(setAllowSubprogramPropertiesChanging, void(bool));
};

}
