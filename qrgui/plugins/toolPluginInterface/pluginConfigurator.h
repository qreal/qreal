/* Copyright 2007-2015 QReal Research Group, Dmitry Mordvinov
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

#include <qrrepo/repoControlInterface.h>

#include <qrgui/plugins/toolPluginInterface/usedInterfaces/graphicalModelAssistInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/projectManagementInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/mainWindowInterpretersInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/mainWindowDockInterface.h>
#include <qrgui/editor/sceneCustomizationInterface.h>
#include <qrgui/textEditor/textManagerInterface.h>
#include <qrgui/plugins/toolPluginInterface/systemEvents.h>

namespace qReal {

/// A container class for convenient passing different system parts into plugins.
class PluginConfigurator
{
public:
	PluginConfigurator(
		qrRepo::RepoControlInterface &repoControlInterface
		, GraphicalModelAssistInterface &graphicalModelApi
		, LogicalModelAssistInterface &logicalModelApi
		, gui::MainWindowInterpretersInterface &mainWindowInterpretersInterface
		, gui::MainWindowDockInterface &mainWindowDockInterface
		, ProjectManagementInterface &projectManager
		, gui::editor::SceneCustomizationInterface &sceneCustomizer
		, SystemEvents &systemEvents
		, TextManagerInterface &textManager
	)
		: mRepoControlInterface(repoControlInterface)
		, mGraphicalModelApi(graphicalModelApi)
		, mLogicalModelApi(logicalModelApi)
		, mMainWindowInterpretersInterface(mainWindowInterpretersInterface)
		, mMainWindowDockInterface(mainWindowDockInterface)
		, mProjectManager(projectManager)
		, mSceneCustomizer(sceneCustomizer)
		, mSystemEvents(systemEvents)
		, mTextManager(textManager)
	{}

	qrRepo::RepoControlInterface &repoControlInterface() const
	{
		return mRepoControlInterface;
	}

	GraphicalModelAssistInterface &graphicalModelApi() const
	{
		return mGraphicalModelApi;
	}

	LogicalModelAssistInterface &logicalModelApi() const
	{
		return mLogicalModelApi;
	}

	gui::MainWindowInterpretersInterface &mainWindowInterpretersInterface() const
	{
		return mMainWindowInterpretersInterface;
	}

	gui::MainWindowDockInterface &mainWindowDockInterface() const
	{
		return mMainWindowDockInterface;
	}

	ProjectManagementInterface &projectManager() const
	{
		return mProjectManager;
	}

	gui::editor::SceneCustomizationInterface &sceneCustomizer() const
	{
		return mSceneCustomizer;
	}

	SystemEvents &systemEvents() const
	{
		return mSystemEvents;
	}

	TextManagerInterface &textManager() const
	{
		return mTextManager;
	}

private:
	qrRepo::RepoControlInterface &mRepoControlInterface;
	GraphicalModelAssistInterface &mGraphicalModelApi;
	LogicalModelAssistInterface &mLogicalModelApi;
	gui::MainWindowInterpretersInterface &mMainWindowInterpretersInterface;
	gui::MainWindowDockInterface &mMainWindowDockInterface;
	ProjectManagementInterface &mProjectManager;
	gui::editor::SceneCustomizationInterface &mSceneCustomizer;
	SystemEvents &mSystemEvents;
	TextManagerInterface &mTextManager;
};

}
