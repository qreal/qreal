#include "diffPluginWrapper.h"
#include "view/diffWindow.h"

using namespace versioning;

DiffPluginWrapper::DiffPluginWrapper(DiffPluginBase *plugin
		, qrRepo::WorkingCopyManagementInterface *workingCopyManager
		, BriefVersioningInterface *vcs
		, MainWindow *mainWindow
		, EditorManager *manager)
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

void DiffPluginWrapper::showDiff(const QString &targetProject)
{
	mLoader->startModelLoading(targetProject);
}

void DiffPluginWrapper::showDiff(int repoRevision, const QString &targetProject)
{
}

void DiffPluginWrapper::showDiff(int oldRepoRevision, int newRepoRevision
		, const QString &targetProject)
{
}

void DiffPluginWrapper::onModelLoaded(DiffModel *model)
{
	if (!model) {
		return;
	}
	DiffWindow *diffWindow = new DiffWindow(mMainWindow, model, mMainWindow);
	diffWindow->exec();
	delete diffWindow;
}
