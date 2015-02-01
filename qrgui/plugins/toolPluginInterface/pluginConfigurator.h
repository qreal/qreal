#pragma once

#include <qrrepo/repoControlInterface.h>

#include "qrgui/plugins/toolPluginInterface/usedInterfaces/graphicalModelAssistInterface.h"
#include "qrgui/plugins/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h"
#include "qrgui/mainWindow/mainWindowInterpretersInterface.h"
#include "qrgui/mainWindow/projectManager/projectManagementInterface.h"
#include "qrgui/editor/sceneCustomizationInterface.h"
#include "qrgui/textEditor/textManagerInterface.h"
#include "qrgui/plugins/toolPluginInterface/systemEvents.h"

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
