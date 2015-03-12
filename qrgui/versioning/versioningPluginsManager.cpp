#include "versioningPluginsManager.h"

#include <QtWidgets/QApplication>

#include <qrutils/fileSystemUtils.h>

#include "qrgui/mainWindow/mainWindow.h"


using namespace qReal;

QString const tempFolderName = "tempVCS";

VersioningPluginsManager::VersioningPluginsManager(
		qrRepo::RepoControlInterface *repoApi
		, ErrorReporterInterface *errorReporter
		, ProjectManager *projectManager
		)
	: mRepoApi(repoApi), mErrorReporter(errorReporter)
	, mDiffInterface(NULL)
	, mProjectManager(projectManager)
	, mTempDir(qApp->applicationDirPath() + "/" + tempFolderName)
{
	SettingsManager::setValue("versioningManagerTempDir", mTempDir);
	mRepoApi->setWorkingCopyInspector(this);
}

QString VersioningPluginsManager::tempFolder() const
{
	return SettingsManager::value("versioningManagerTempDir", mTempDir).toString();
}

void VersioningPluginsManager::prepareWorkingCopy()
{
	mRepoApi->prepareWorkingCopy(tempFolder());
}

void VersioningPluginsManager::initFromToolPlugins(QListIterator<ToolPluginInterface *> iterator
		, EditorManagerInterface *editorManager, QWidget *parent)
{
	while (iterator.hasNext()) {
		ToolPluginInterface *toolPlugin = iterator.next();
		VersioningPluginInterface *versioningPlugin =
			dynamic_cast<VersioningPluginInterface *>(toolPlugin);
		mDiffInterface = dynamic_cast<DiffPluginInterface *>(toolPlugin);

		if (versioningPlugin) {
			mPlugins.append(versioningPlugin);
        if (!versioningPlugin->clientExist()) {
				foreach(ActionInfo const &actionInfo, versioningPlugin->actions()) {
					if (actionInfo.isAction()){
						actionInfo.action()->setVisible(false);
					} else {
						actionInfo.menu()->menuAction()->setVisible(false);
					}
				}
			}

			versioningPlugin->setWorkingCopyManager(mRepoApi);
			connect(
				versioningPlugin
				, SIGNAL(workingCopyDownloaded(bool, QString const &))
				, this
				, SLOT(onWorkingCopyDownloaded(bool, QString const &))
			);

			connect(versioningPlugin, SIGNAL(workingCopyUpdated(bool)), this, SLOT(onWorkingCopyUpdated(bool)));

			connect(versioningPlugin, SIGNAL(changesSubmitted(bool)), this, SLOT(onChangesSubmitted(bool)));
		}
	}

	if (mDiffInterface){
		mDiffInterface->configure(mProjectManager, mErrorReporter, mRepoApi, this, parent, editorManager);
		foreach (VersioningPluginInterface *versioningPlugin, mPlugins) {
			versioningPlugin->setDiffViewerInterface(mDiffInterface);
		}
	}
}

VersioningPluginInterface *VersioningPluginsManager::activePlugin(bool needPreparation, const QString &workingDir)
{
	if (needPreparation) {
		prepareWorkingCopy();
	}

	VersioningPluginInterface *result = nullptr;
	foreach (VersioningPluginInterface *plugin, mPlugins) {
		if (plugin->isMyWorkingCopy(workingDir,true)) {
			result = plugin;
		}
	}

	if (needPreparation) {
		qReal::FileSystemUtils::removeDir(tempFolder());
	}

	return result;
}

qrRepo::WorkingCopyInspectionInterface *VersioningPluginsManager::activeWorkingCopyInspector(QString const &workingDir)
{
	WorkingCopyInspectionInterface *activeInspector =
		dynamic_cast<WorkingCopyInspectionInterface *>(activeClient(workingDir));
	return activeInspector;
}

BriefVersioningInterface *VersioningPluginsManager::activeClient(QString const &workingDir)
{
	return activePlugin(false, workingDir);
}

bool VersioningPluginsManager::onFileAdded(const QString &filePath, const QString &workingDir)
{
	WorkingCopyInspectionInterface *activeInspector = activeWorkingCopyInspector(workingDir);
	if (!activeInspector) {
		return true;
	}

	return activeInspector->onFileAdded(filePath, workingDir);
}

bool VersioningPluginsManager::onFileRemoved(const QString &filePath, const QString &workingDir)
{
	WorkingCopyInspectionInterface *activeInspector = activeWorkingCopyInspector(workingDir);
	if (!activeInspector) {
		return true;
	}

	return activeInspector->onFileRemoved(filePath, workingDir);
}

bool VersioningPluginsManager::onFileChanged(const QString &filePath, const QString &workingDir)
{
	WorkingCopyInspectionInterface *activeInspector = activeWorkingCopyInspector(workingDir);
	if (!activeInspector) {
		return true;
	}

	return activeInspector->onFileChanged(filePath, workingDir);
}

void VersioningPluginsManager::beginWorkingCopyDownloading(
	  QString const &repoAddress
	, QString const &targetProject
	, QString commitId
	, bool quiet
	)
{
	BriefVersioningInterface *activeVcs = activePlugin(true, tempFolder());
	if (!activeVcs) {
		return;
	}

	return activeVcs->beginWorkingCopyDownloading(repoAddress, targetProject, commitId, quiet);
}

void VersioningPluginsManager::beginWorkingCopyUpdating(QString const &targetProject)
{
	BriefVersioningInterface *activeVcs = activePlugin(true, tempFolder());
	if (!activeVcs) {
		return;
	}

	return activeVcs->beginWorkingCopyUpdating(targetProject);
}

void VersioningPluginsManager::beginChangesSubmitting(
	QString const &description
	, QString const &targetProject
	, bool quiet
	)
{
	BriefVersioningInterface *activeVcs = activePlugin(true, tempFolder());
	if (!activeVcs) {
		return;
	}

	return activeVcs->beginChangesSubmitting(description, targetProject, quiet);
}

bool VersioningPluginsManager::reinitWorkingCopy(QString const &targetProject)
{
	BriefVersioningInterface *activeVcs = activePlugin(true, tempFolder());
	if (!activeVcs) {
		return true;
	}

	return activeVcs->reinitWorkingCopy(targetProject);
}

QString VersioningPluginsManager::information(QString const &targetProject)
{
	BriefVersioningInterface *activeVcs = activePlugin(true, tempFolder());
	if (!activeVcs) {
		return QString();
	}

	return activeVcs->information(targetProject);
}

QString VersioningPluginsManager::commitId(QString const &targetProject)
{
	BriefVersioningInterface *activeVcs = activePlugin(true, tempFolder());
	if (!activeVcs) {
		return "-1";
	}

	return activeVcs->commitId(targetProject);
}

QString VersioningPluginsManager::remoteRepositoryUrl(QString const &targetProject)
{
	BriefVersioningInterface *activeVcs = activePlugin(true, tempFolder());
	if (!activeVcs) {
		return QString();
	}

	return activeVcs->remoteRepositoryUrl(targetProject);
}

bool VersioningPluginsManager::isMyWorkingCopy(QString const &directory, bool quiet, bool prepareAndProcess)
{
	Q_UNUSED(quiet)
	return activePlugin(prepareAndProcess, directory) != nullptr;
}

QString VersioningPluginsManager::friendlyName()
{
	BriefVersioningInterface *activeVcs = activePlugin(true, tempFolder());
	if (!activeVcs) {
		return QString();
	}

	return activeVcs->friendlyName();
}

void VersioningPluginsManager::reportError(const QString &message)
{
	mErrorReporter->addError(message);
}

void VersioningPluginsManager::reportErrors(const QStringList &messages)
{
	foreach (QString const &message, messages) {
		reportError(message);
	}
}

void VersioningPluginsManager::reportWarning(const QString &message)
{
	mErrorReporter->addWarning(message);
}

void VersioningPluginsManager::reportWarnings(const QStringList &messages)
{
	foreach (QString const &message, messages) {
		reportWarning(message);
	}
}

void VersioningPluginsManager::onWorkingCopyDownloaded(bool success, QString const &targetProject)
{
	emit workingCopyDownloaded(success, targetProject);
}

void VersioningPluginsManager::onWorkingCopyUpdated(bool success)
{
	QString currentAdress = mProjectManager->saveFilePath();
	mProjectManager->close();
	mProjectManager->open(currentAdress);
	emit workingCopyUpdated(success);
}

void VersioningPluginsManager::onChangesSubmitted(bool success)
{
	emit changesSubmitted(success);
}

bool VersioningPluginsManager::clientExist()
{
	return activePlugin()->clientExist();
}
