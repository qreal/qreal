#include "diffPluginWrapper.h"
#include "view/diffWindow.h"
using namespace versioning;

DiffPluginWrapper::DiffPluginWrapper(qReal::ProjectManagementInterface *projectManager
		, qReal::ErrorReporterInterface *errorReporter
		, qrRepo::WorkingCopyManagementInterface *workingCopyManager
		, BriefVersioningInterface *vcs
		, QWidget *parent
		, EditorManagerInterface *manager)
	: mWorkingCopyManager(workingCopyManager)
	, mVcs(vcs), mMainWindow(parent), mEditorManager(manager)
	, mErrorReporter(errorReporter)
	, mProjectManager(projectManager)
	, mLoader(new details::ModelLoader(mVcs, mErrorReporter
			, mEditorManager, mWorkingCopyManager))
{
	connect(mLoader, SIGNAL(modelLoaded(DiffModel*))
			, this, SLOT(onModelLoaded(DiffModel*)));
}

void DiffPluginWrapper::showDiff(const QString &targetProject, QWidget *parentWidget, const bool &compactMode)
{
	mCompactMode = compactMode;
	mParentWidget = parentWidget;
	mLoader->startModelLoading(targetProject);
}

void DiffPluginWrapper::showDiff(QString repoRevision, const QString &targetProject
								 , QWidget *parentWidget, bool const &compactMode)
{
	mCompactMode = compactMode;
	mParentWidget = parentWidget;
	mLoader->startModelLoading(repoRevision, targetProject);
}

void DiffPluginWrapper::showDiff(QString oldRepoRevision, QString newRepoRevision
		, const QString &targetProject, QWidget *parentWidget, const bool &compactMode)
{
	mCompactMode = compactMode;
	mParentWidget = parentWidget;
	mLoader->startModelLoading(oldRepoRevision, newRepoRevision, targetProject);
}

void DiffPluginWrapper::onModelLoaded(DiffModel *model)
{
	if (!model) {
		return;
	}
	DiffWindow *diffWindow = new DiffWindow(model, mCompactMode, mMainWindow);
	mParentWidget->layout()->addWidget(diffWindow);
}

