#pragma once

#include "../../qrrepo/repoControlInterface.h"
#include "usedInterfaces/graphicalModelAssistInterface.h"
#include "usedInterfaces/logicalModelAssistInterface.h"
#include "../mainwindow/mainWindowInterpretersInterface.h"

namespace qReal {

class PluginConfigurator
{
public:
	PluginConfigurator(
		qrRepo::RepoControlInterface &repoControlInterface
		, GraphicalModelAssistInterface &graphicalModelApi
		, LogicalModelAssistInterface &logicalModelApi
		, gui::MainWindowInterpretersInterface &mainWindowInterpretersInterface
	)
		: mRepoControlInterface(repoControlInterface)
		, mGraphicalModelApi(graphicalModelApi)
		, mLogicalModelApi(logicalModelApi)
		, mMainWindowInterpretersInterface(mainWindowInterpretersInterface)
	{}

	qrRepo::RepoControlInterface &repoControlInterface() const {
		return mRepoControlInterface;
	}

	GraphicalModelAssistInterface &graphicalModelApi() const {
		return mGraphicalModelApi;
	}

	LogicalModelAssistInterface &logicalModelApi() const {
		return mLogicalModelApi;
	}

	gui::MainWindowInterpretersInterface &mainWindowInterpretersInterface() const {
		return mMainWindowInterpretersInterface;
	}

private:
	qrRepo::RepoControlInterface &mRepoControlInterface;
	GraphicalModelAssistInterface &mGraphicalModelApi;
	LogicalModelAssistInterface &mLogicalModelApi;
	gui::MainWindowInterpretersInterface &mMainWindowInterpretersInterface;
};

}
