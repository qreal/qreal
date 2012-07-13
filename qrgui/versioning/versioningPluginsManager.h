#pragma once

#include "../pluginManager/toolPluginManager.h"
#include "versioningPluginInterface.h"
#include "briefVersioningInterface.h"
#include "../models/models.h"

namespace qReal
{
namespace versioning
{

class VersioningPluginsManager : public QObject, public qrRepo::versioning::WorkingCopyInspectionInterface
{
public:
	/// Inits plugin list using loaded by plugin manager ones
	VersioningPluginsManager(ToolPluginManager const &pluginManager,
		qrRepo::RepoControlInterface *repoApi, ErrorReporterInterface *errorReporter);

	BriefVersioningInterface *activeClient(const QString &workingDir) const;

	bool onFileAdded(QString const &filePath, QString const &workingDir);
	bool onFileRemoved(QString const &filePath, QString const &workingDir);
	bool onFileChanged(QString const &filePath, QString const &workingDir);

private:
	void initFromToolPlugins(QListIterator<ToolPluginInterface *> iterator);
	VersioningPluginInterface *activePlugin(QString const &workingDir) const;
	WorkingCopyInspectionInterface *activeWorkingCopyInspector(QString const &workingDir);

	void reportError(QString const &message);
	void reportErrors(QStringList const &messages);
	void reportWarning(QString const &message);
	void reportWarnings(QStringList const &messages);

	qrRepo::RepoControlInterface *mRepoApi;
	QList<VersioningPluginInterface *> mPlugins;
	ErrorReporterInterface *mErrorReporter;
};

}
}
