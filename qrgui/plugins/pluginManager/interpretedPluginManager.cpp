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

#include "interpretedPluginManager.h"

#include <QtWidgets/QApplication>

#include <qrkernel/platformInfo.h>

using namespace qReal;

InterpretedPluginManager::InterpretedPluginManager()
	: mCustomizer()
	, mPluginManager(PlatformInfo::invariantSettingsPath("pathToInterpretedPlugins"))
{
	mPlugins = mPluginManager.loadAllPlugins<InterpretedPluginInterface>();
}

InterpretedPluginManager::~InterpretedPluginManager()
{
}

void InterpretedPluginManager::init(
		const PluginConfigurator &configurator
		, qrRepo::LogicalRepoApi &metamodelRepoApi
		, EditorManagerInterface *editorManagerInterface)
{
	for (InterpretedPluginInterface * const interpretedPlugin : mPlugins) {
		interpretedPlugin->init(configurator, metamodelRepoApi, editorManagerInterface);
	}
}

QList<ActionInfo> InterpretedPluginManager::actions() const
{
	QList<ActionInfo> result;
	for (InterpretedPluginInterface * const toolPlugin : mPlugins) {
		result += toolPlugin->actions();
	}

	return result;
}

QList<QAction *> InterpretedPluginManager::menuActionsList() const
{
	QList<QAction *> result;
	for (InterpretedPluginInterface * const interpretedPlugin : mPlugins) {
		result += interpretedPlugin->menuActionList();
	}

	return result;
}

void InterpretedPluginManager::loadDefaultSettings()
{
}

void InterpretedPluginManager::setHotKeyActions() const
{
}
