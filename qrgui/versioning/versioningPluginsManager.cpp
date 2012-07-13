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
//	if (!activeInspector) {
//		reportWarning(tr("Current working copy versioned by unknown version control system"));
//	}
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
		return false;
	}
	return activeInspector->onFileAdded(filePath, workingDir);
}

bool VersioningPluginsManager::onFileRemoved(const QString &filePath, const QString &workingDir)
{
	WorkingCopyInspectionInterface *activeInspector = activeWorkingCopyInspector(workingDir);
	if (!activeInspector) {
		return false;
	}
	return activeInspector->onFileRemoved(filePath, workingDir);
}

bool VersioningPluginsManager::onFileChanged(const QString &filePath, const QString &workingDir)
{
	WorkingCopyInspectionInterface *activeInspector = activeWorkingCopyInspector(workingDir);
	if (!activeInspector) {
		return false;
	}
	return activeInspector->onFileChanged(filePath, workingDir);
}

void VersioningPluginsManager::reportError(const QString &message)
{
	mErrorReporter->addError(message);
}

void VersioningPluginsManager::reportErrors(const QStringList &messages)
{
	foreach (QString const &message, messages)
	{
		reportError(message);
	}
}

void VersioningPluginsManager::reportWarning(const QString &message)
{
	mErrorReporter->addWarning(message);
}

void VersioningPluginsManager::reportWarnings(const QStringList &messages)
{
	foreach (QString const &message, messages)
	{
		reportWarning(message);
	}
}
