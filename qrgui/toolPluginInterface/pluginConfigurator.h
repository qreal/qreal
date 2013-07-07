#pragma once

#include "../../qrrepo/repoControlInterface.h"
#include "usedInterfaces/graphicalModelAssistInterface.h"
#include "usedInterfaces/logicalModelAssistInterface.h"
#include "../mainwindow/mainWindowInterpretersInterface.h"
#include "../mainwindow/projectManager/projectManagementInterface.h"
#include "../view/sceneCustomizationInterface.h"
#include "systemEventsInterface.h"
#include "../texteditor/codeManagerInterface.h"

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
		, SystemEventsInterface &systemEvents
		, CodeManagerInterface &codeManager
	)
		: mRepoControlInterface(repoControlInterface)
		, mGraphicalModelApi(graphicalModelApi)
		, mLogicalModelApi(logicalModelApi)
		, mMainWindowInterpretersInterface(mainWindowInterpretersInterface)
		, mProjectManager(projectManager)
		, mSceneCustomizer(sceneCustomizer)
		, mSystemEvents(systemEvents)
		, mCodeManager(codeManager)
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

	SystemEventsInterface &systemEvents() const
	{
		return mSystemEvents;
	}

	CodeManagerInterface &codeManager() const
	{
		return mCodeManager;
	}

private:
	qrRepo::RepoControlInterface &mRepoControlInterface;
	GraphicalModelAssistInterface &mGraphicalModelApi;
	LogicalModelAssistInterface &mLogicalModelApi;
	gui::MainWindowInterpretersInterface &mMainWindowInterpretersInterface;
	ProjectManagementInterface &mProjectManager;
	SceneCustomizationInterface &mSceneCustomizer;
	SystemEventsInterface &mSystemEvents;
	CodeManagerInterface &mCodeManager;
};

}
