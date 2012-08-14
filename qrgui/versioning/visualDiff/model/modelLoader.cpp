#include <QtGui/QApplication>

#include "modelLoader.h"

using namespace versioning;
using namespace versioning::details;

QString const tempProjectName = "tempDiff.qrs";

ModelLoader::ModelLoader(qReal::BriefVersioningInterface *vcs
		, qReal::ErrorReporterInterface *errorReporter
		, qReal::EditorManager *editorManager
		, qrRepo::WorkingCopyManagementInterface *workingCopyManager)
	: mVcs(vcs)
	, mErrorReporter(errorReporter)
	, mEditorManager(editorManager)
	, mWorkingCopyManager(workingCopyManager)
	, mTempProject(qApp->applicationDirPath() + "/" + tempProjectName)
{
	SettingsManager::instance()->setValue("diffTempProject", mTempProject);

	connect(mVcs, SIGNAL(workingCopyDownloaded(bool, QString))
			, this, SLOT(onDownloadingComplete(bool, QString)));
}

QString ModelLoader::tempProject() const
{
	return SettingsManager::value("diffTempProject", mTempProject).toString();
}

void ModelLoader::startModelLoading(QString const &targetProject)
{
	mOldModel = loadFromDisk(targetProject);
	if (!mOldModel) {
		emit modelLoaded(NULL);
		return;
	}
	QString const repoUrl = mVcs->remoteRepositoryUrl(targetProject);
	if (repoUrl.isEmpty()) {
		emit modelLoaded(NULL);
		return;
	}
	connect(this, SIGNAL(internalModelLoaded(qReal::models::Models*))
			, this, SLOT(onNewModelLoaded(qReal::models::Models*)));
	mVcs->beginWorkingCopyDownloading(repoUrl, tempProject(), -1, true);
}

void ModelLoader::startModelLoading(int repoRevision, QString const &targetProject)
{
	mOldModel = loadFromDisk(targetProject);
	if (!mOldModel) {
		emit modelLoaded(NULL);
		return;
	}
	QString const &repoUrl = mVcs->remoteRepositoryUrl(targetProject);
	if (repoUrl.isEmpty()) {
		emit modelLoaded(NULL);
		return;
	}
	connect(this, SIGNAL(internalModelLoaded(qReal::models::Models*))
			, this, SLOT(onNewModelLoaded(qReal::models::Models*)));
	mVcs->beginWorkingCopyDownloading(repoUrl, tempProject(), repoRevision, true);
}

void ModelLoader::startModelLoading(int oldRepoRevision, int newRepoRevision, QString const &targetProject)
{
	mRepoUrl = mVcs->remoteRepositoryUrl(targetProject);
	mNewRevision = newRepoRevision;
	if (mRepoUrl.isEmpty()) {
		emit modelLoaded(NULL);
		return;
	}
	connect(this, SIGNAL(internalModelLoaded(qReal::models::Models*))
			, this, SLOT(onOldModelLoaded(qReal::models::Models*)));
	mVcs->beginWorkingCopyDownloading(mRepoUrl, tempProject(), oldRepoRevision, true);
}

qReal::models::Models *ModelLoader::loadFromDisk(QString const &targetProject)
{
	qReal::models::Models *result = new qReal::models::Models(targetProject, *mEditorManager);
	result->repoControlApi().open(targetProject);
	result->reinit();
	return result;
}

void ModelLoader::onDownloadingComplete(bool success, const QString &targetProject)
{
	if (!success) {
		emit modelLoaded(NULL);
	}
	emit internalModelLoaded(loadFromDisk(targetProject));
}

void ModelLoader::onOldModelLoaded(qReal::models::Models *model)
{
	mOldModel = model;
	disconnect(this, SLOT(onOldModelLoaded(qReal::models::Models*)));
	if (!mOldModel) {
		emit modelLoaded(NULL);
	}
	connect(this, SIGNAL(internalModelLoaded(qReal::models::Models*))
			, this, SLOT(onNewModelLoaded(qReal::models::Models*)));
	mVcs->beginWorkingCopyDownloading(mRepoUrl, tempProject(), mNewRevision, true);
}

void ModelLoader::onNewModelLoaded(qReal::models::Models *model)
{
	mNewModel = model;
	disconnect(this, SLOT(onNewModelLoaded(qReal::models::Models*)));
	finishModelLoading();
}

void ModelLoader::finishModelLoading()
{
	DiffModel *result = NULL;
	if (mOldModel && mNewModel) {
		result = new DiffModel(mOldModel, mNewModel);
	}
	emit modelLoaded(result);
}

void ModelLoader::reportError(const QString &message)
{
	mErrorReporter->addError(message);
}

void ModelLoader::reportWarning(const QString &message)
{
	mErrorReporter->addWarning(message);
}
