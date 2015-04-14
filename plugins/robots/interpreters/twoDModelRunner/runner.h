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

#include <qrgui/systemFacade/systemFacade.h>
#include <qrgui/systemFacade/components/consoleErrorReporter.h>
#include <qrgui/systemFacade/components/nullMainWindow.h>
#include <qrgui/systemFacade/components/projectManager.h>
#include <qrgui/systemFacade/components/nullTextManager.h>
#include <qrgui/editor/sceneCustomizer.h>
#include <qrgui/plugins/toolPluginInterface/pluginConfigurator.h>
#include <interpreterCore/robotsPluginFacade.h>

namespace twoDModel {

class Runner
{
public:
	Runner();

	void interpret(const QString &saveFile, bool background);

private:
	qReal::SystemFacade mQRealFacade;
	qReal::ConsoleErrorReporter mErrorReporter;
	qReal::ProjectManager mProjectManager;
	qReal::NullMainWindow mMainWindow;
	qReal::NullTextManager mTextManager;
	qReal::SceneCustomizer mSceneCustomizer;
	qReal::PluginConfigurator mConfigurator;
	interpreterCore::RobotsPluginFacade mPluginFacade;
};

}
