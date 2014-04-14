#include "diffPluginWrapper.h"
#include "view/diffWindow.h"

using namespace versioning;

DiffPluginWrapper::DiffPluginWrapper(DiffPluginBase *plugin
		, qrRepo::WorkingCopyManagementInterface *workingCopyManager
		, BriefVersioningInterface *vcs
		, MainWindow *mainWindow
		, EditorManagerInterface *manager)
	: mWorkingCopyManager(workingCopyManager)
	, mVcs(vcs), mMainWindow(mainWindow), mEditorManager(manager)
	, mErrorReporter(plugin->errorReporter())
	, mProjectManager(plugin->projectManager())
	, mLoader(new details::ModelLoader(mVcs, mErrorReporter
			, mEditorManager, mWorkingCopyManager))
{
	connect(mLoader, SIGNAL(modelLoaded(DiffModel*))
			, this, SLOT(onModelLoaded(DiffModel*)));
}

void DiffPluginWrapper::showDiff(const QString &targetProject, QWidget *parentWidget)
{
	mParentWidget = parentWidget;
	mLoader->startModelLoading(targetProject);
}

void DiffPluginWrapper::showDiff(QString repoRevision, const QString &targetProject, QWidget *parentWidget)
{
	mParentWidget = parentWidget;
	mLoader->startModelLoading(repoRevision, targetProject);
}

void DiffPluginWrapper::showDiff(QString oldRepoRevision, QString newRepoRevision
		, const QString &targetProject, QWidget *parentWidget)
{
	mParentWidget = parentWidget;
	mLoader->startModelLoading(oldRepoRevision, newRepoRevision, targetProject);
}

void DiffPluginWrapper::onModelLoaded(DiffModel *model)
{
	if (!model) {
		return;
	}
	DiffWindow *diffWindow = new DiffWindow(mMainWindow, model, mMainWindow);
	mParentWidget->layout()->addWidget(diffWindow);
}

