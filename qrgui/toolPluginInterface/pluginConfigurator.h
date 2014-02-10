#pragma once

#include <qrrepo/repoControlInterface.h>

#include "toolPluginInterface/usedInterfaces/graphicalModelAssistInterface.h"
#include "toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h"
#include "mainwindow/mainWindowInterpretersInterface.h"
#include "mainwindow/projectManager/projectManagementInterface.h"
#include "view/sceneCustomizationInterface.h"
#include "textEditor/textManagerInterface.h"
#include "systemEvents.h"

namespace qReal {

class PluginConfigurator
{
public:
	PluginConfigurator(
		qrRepo::RepoControlInterface &repoControlInterface
		, GraphicalModelAssistInterface &graphicalModelApi
		, LogicalModelAssistInterface &logicalModelApi
		, gui::MainWindowInterpretersInterface &mainWindowInterpretersInterface
		, ProjectManagementInterface &projectManager
		, SceneCustomizationInterface &sceneCustomizer
		, SystemEvents &systemEvents
		, TextManagerInterface &textManager
	)
		: mRepoControlInterface(repoControlInterface)
		, mGraphicalModelApi(graphicalModelApi)
		, mLogicalModelApi(logicalModelApi)
		, mMainWindowInterpretersInterface(mainWindowInterpretersInterface)
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

	ProjectManagementInterface &projectManager() const
	{
		return mProjectManager;
	}

	SceneCustomizationInterface &sceneCustomizer() const
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
	ProjectManagementInterface &mProjectManager;
	SceneCustomizationInterface &mSceneCustomizer;
	SystemEvents &mSystemEvents;
	TextManagerInterface &mTextManager;
};

}
