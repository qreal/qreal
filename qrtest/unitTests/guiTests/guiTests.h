/* Copyright 2015 QReal Research Group
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

#include <gtest/gtest.h>
#include <QProcess>
#include "qrgui/mainWindow/qrealApplication.h"
#include "qrgui/plugins/toolPluginInterface/usedInterfaces/mainWindowScriptAPIInterface.h"
#include "./qrgui/mainWindow/mainWindow.h"

namespace guiTesting {

/// Test suite for GUI
class guiTests : public testing::Test
{

protected:
	void SetUp() override;
	void TearDown() override;

	void run(const QString &script);
	void runFromFile(const QString &fileName);


private:
//	QProcess* mQrealInstance;
	MainWidnowScriptAPIInterface* mMainWindowScriptAPIInterface;
	qReal::QRealApplication* mApp;
	qReal::MainWindow* mWindow;
	bool scriptInterpretationDefaultValue = SettingsManager::value("scriptInterpretation").toBool();
	// это надо изменить глобально на savedata() и loaddata, мб? например, если мы изменяем путь до чего-то и хотим проверить, сохранились
	// ли в регистре эти изменения правильно, а потом всё вернуть.
};

}
