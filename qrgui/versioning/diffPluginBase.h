#pragma once

#include "diffPluginInterface.h"
#include "../../qrutils/versioningUtils/briefVersioningInterface.h"
#include "qrgui/versioning/versioningPluginsManagerDeclSpec.h"


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

	virtual void showDiff(QString const &targetProject, QWidget *parentWidget, bool const &compactMode = false)
	{
		if (mHandler) {
			mHandler->showDiff(targetProject, parentWidget, compactMode);
		}
	}

	virtual void showDiff(QString repoRevision, QString const &targetProject
						  , QWidget *parentWidget, bool const &compactMode = false)
	{
		if (mHandler) {
			mHandler->showDiff(repoRevision, targetProject, parentWidget, compactMode);
		}
	}

	virtual void showDiff(QString oldRepoRevision , QString newRepoRevision
			, QString const &targetProject , QWidget *parentWidget, bool const &compactMode = false)
	{
		if (mHandler) {
			mHandler->showDiff(oldRepoRevision, newRepoRevision, targetProject, parentWidget, compactMode);
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
