#pragma once

#include "../pluginManager/toolPluginManager.h"
#include "versioningPluginInterface.h"
#include "briefVersioningInterface.h"
#include "../models/models.h"

namespace qReal
{
namespace versioning
{

class VersioningPluginsManager
		: public BriefVersioningInterface
		, public qrRepo::versioning::WorkingCopyInspectionInterface
{
	Q_OBJECT

public:
	/// Inits plugin list using loaded by plugin manager ones
	VersioningPluginsManager(ToolPluginManager const &pluginManager,
		qrRepo::RepoControlInterface *repoApi, ErrorReporterInterface *errorReporter);

	BriefVersioningInterface *activeClient(const QString &workingDir) const;

	bool onFileAdded(QString const &filePath, QString const &workingDir);
	bool onFileRemoved(QString const &filePath, QString const &workingDir);
	bool onFileChanged(QString const &filePath, QString const &workingDir);

public slots:
	void beginWorkingCopyDownloading(
			  QString const &repoAddress
			, QString const &targetProject
			, int revisionNumber = -1);
	void beginWorkingCopyUpdating();
	void beginChangesSubmitting(QString const &description);
	bool reinitWorkingCopy();
	QString information();
	int revisionNumber();
	bool isMyWorkingCopy(QString const &directory = "");

private slots:
	void onWorkingCopyDownloaded(bool const success);
	void onWorkingCopyUpdated(bool const success);
	void onChangesSubmitted(bool const success);

private:
	void initFromToolPlugins(QListIterator<ToolPluginInterface *> iterator);
	VersioningPluginInterface *activePlugin(QString const &workingDir = "") const;
	WorkingCopyInspectionInterface *activeWorkingCopyInspector(QString const &workingDir = "");

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
