#pragma once

#include "diffPluginInterface.h"
#include "../../qrutils/versioningUtils/briefVersioningInterface.h"
#include "../mainwindow/mainWindow.h"

namespace qReal
{

/// A temporaly created base class for all diff-tools implemented in system.
/// Must be removed when system would be refatored
class DiffPluginBase : public DiffPluginInterface
{

public:
	DiffPluginBase() : mHandler(NULL) {}
	virtual ~DiffPluginBase() {}

	virtual void init(PluginConfigurator const &configurator)
	{
		ToolPluginInterface::init(configurator);
		mErrorReporter = configurator.mainWindowInterpretersInterface().errorReporter();
		mProjectManager = &(configurator.projectManager());
	}

	virtual void showDiff(QString const &targetProject = QString())
	{
		if (mHandler) {
			mHandler->showDiff(targetProject);
		}
	}

	virtual void showDiff(int repoRevision, QString const &targetProject = QString())
	{
		if (mHandler) {
			mHandler->showDiff(repoRevision, targetProject);
		}
	}

	virtual void showDiff(int oldRepoRevision , int newRepoRevision
			, QString const &targetProject = QString())
	{
		if (mHandler) {
			mHandler->showDiff(oldRepoRevision, newRepoRevision, targetProject);
		}
	}

	qReal::ErrorReporterInterface *errorReporter() const;
	qReal::ProjectManagementInterface *projectManager() const;

	/// This method allows to redirect all the useful operations
	/// to specified handled. Temp solution to redirect
	/// control flow to qrgui
	void setHandler(DiffPluginInterface *handler);

protected:
	ErrorReporterInterface *mErrorReporter;
	ProjectManagementInterface *mProjectManager;
	DiffPluginInterface *mHandler;
};

}
