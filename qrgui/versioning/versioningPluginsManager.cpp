#include "versioningPluginsManager.h"

using namespace qReal::versioning;

VersioningPluginsManager::VersioningPluginsManager(
		qReal::ToolPluginManager const &pluginManager,
		qrRepo::RepoControlInterface *repoApi,
		qReal::ErrorReporterInterface *errorReporter)
	: mRepoApi(repoApi), mErrorReporter(errorReporter)
{
	initFromToolPlugins(pluginManager.pluginsIterator());
	mRepoApi->setWorkingCopyInspector(this);
}

void VersioningPluginsManager::initFromToolPlugins(QListIterator<qReal::ToolPluginInterface *> iterator)
{
	while (iterator.hasNext()) {
		qReal::ToolPluginInterface *toolPlugin = iterator.next();
		VersioningPluginInterface *versioningPlugin =
				dynamic_cast<VersioningPluginInterface *>(toolPlugin);
		if (versioningPlugin) {
			mPlugins.append(versioningPlugin);
			versioningPlugin->setWorkingCopyManager(mRepoApi);
			connect(versioningPlugin, SIGNAL(workingCopyDownloaded(bool const))
					, this, SLOT(onWorkingCopyDownloaded(bool const)));
			connect(versioningPlugin, SIGNAL(workingCopyUpdated(bool const))
					, this, SLOT(onWorkingCopyUpdated(bool const)));
			connect(versioningPlugin, SIGNAL(changesSubmitted(bool const))
					, this, SLOT(onChangesSubmitted(bool const)));
		}
	}
}

VersioningPluginInterface *VersioningPluginsManager::activePlugin(QString const &workingDir) const
{
	foreach (VersioningPluginInterface *plugin, mPlugins) {
		if (plugin->isMyWorkingCopy(workingDir)) {
			return plugin;
		}
	}
	return NULL;
}

qrRepo::versioning::WorkingCopyInspectionInterface *VersioningPluginsManager::activeWorkingCopyInspector(QString const &workingDir)
{
	WorkingCopyInspectionInterface *activeInspector =
			dynamic_cast<WorkingCopyInspectionInterface *>(activeClient(workingDir));
	return activeInspector;
}

BriefVersioningInterface *VersioningPluginsManager::activeClient(QString const &workingDir) const
{
	return activePlugin(workingDir);
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
		, int revisionNumber)
{
	BriefVersioningInterface *activeVcs = activePlugin();
	if (!activeVcs) {
		return;
	}
	return activeVcs->beginWorkingCopyDownloading(repoAddress, targetProject, revisionNumber);
}

void VersioningPluginsManager::beginWorkingCopyUpdating()
{
	BriefVersioningInterface *activeVcs = activePlugin();
	if (!activeVcs) {
		return;
	}
	return activeVcs->beginWorkingCopyUpdating();
}

void VersioningPluginsManager::beginChangesSubmitting(QString const &description)
{
	BriefVersioningInterface *activeVcs = activePlugin();
	if (!activeVcs) {
		return;
	}
	return activeVcs->beginChangesSubmitting(description);
}

bool VersioningPluginsManager::reinitWorkingCopy()
{
	BriefVersioningInterface *activeVcs = activePlugin();
	if (!activeVcs) {
		return true;
	}
	return activeVcs->reinitWorkingCopy();
}

QString VersioningPluginsManager::information()
{
	BriefVersioningInterface *activeVcs = activePlugin();
	if (!activeVcs) {
		return QString();
	}
	return activeVcs->information();
}

int VersioningPluginsManager::revisionNumber()
{
	BriefVersioningInterface *activeVcs = activePlugin();
	if (!activeVcs) {
		return -1;
	}
	return activeVcs->revisionNumber();
}

bool VersioningPluginsManager::isMyWorkingCopy(QString const &directory)
{
	return activePlugin(directory) != NULL;
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

void VersioningPluginsManager::onWorkingCopyDownloaded(const bool success)
{
	emit workingCopyDownloaded(success);
}

void VersioningPluginsManager::onWorkingCopyUpdated(const bool success)
{
	emit workingCopyUpdated(success);
}

void VersioningPluginsManager::onChangesSubmitted(const bool success)
{
	emit changesSubmitted(success);
}
