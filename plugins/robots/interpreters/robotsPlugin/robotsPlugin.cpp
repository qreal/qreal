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

#include "robotsPlugin.h"

#include <QtWidgets/QApplication>

#include <qrkernel/settingsManager.h>
#include <interpreterCore/managers/saveConvertionManager.h>

using namespace qReal;
using namespace robotsPlugin;

static const int gridWidth = 25; // Half of element size

RobotsPlugin::RobotsPlugin()
	: mMainWindowInterpretersInterface(nullptr)
{
	mRobotsPluginFacade.reset(new interpreterCore::RobotsPluginFacade);
}

void RobotsPlugin::init(const PluginConfigurator &configurator)
{
	mRobotsPluginFacade->init(configurator);
	SettingsManager::setValue("IndexGrid", gridWidth);
}

qReal::Customizer *RobotsPlugin::customizationInterface()
{
	return &mRobotsPluginFacade->customizer();
}

QPair<QString, gui::PreferencesPage *> RobotsPlugin::preferencesPage()
{
	return qMakePair(QObject::tr("Robots"), mRobotsPluginFacade->robotsSettingsPage());
}

QList<ActionInfo> RobotsPlugin::actions()
{
	return mRobotsPluginFacade->actionsManager().actions();
}

QList<HotKeyActionInfo> RobotsPlugin::hotKeyActions()
{
	return mRobotsPluginFacade->actionsManager().hotKeyActionInfos();
}

QList<ProjectConverter> RobotsPlugin::projectConverters()
{
	return interpreterCore::SaveConvertionManager::converters();
}

QStringList RobotsPlugin::defaultSettingsFiles()
{
	return mRobotsPluginFacade->defaultSettingsFiles();
}

QObject *RobotsPlugin::guiScriptFacade()
{
	return mRobotsPluginFacade.data()->guiScriptFacade();
}
