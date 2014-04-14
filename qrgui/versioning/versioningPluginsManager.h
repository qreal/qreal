#pragma once

#include "../../qrutils/versioningUtils/briefVersioningInterface.h"
#include "../pluginManager/toolPluginManager.h"
#include "versioningPluginInterface.h"
#include "../models/models.h"
#include "../versioning/transparentMode/transparentMode.h"
#include "../mainwindow/projectManager/projectManager.h"

namespace qReal
{

class MainWindow;

class DiffPluginBase;

class VersioningPluginsManager
		: public BriefVersioningInterface
		, public qrRepo::WorkingCopyInspectionInterface
{
	Q_OBJECT

public:
	/// Inits plugin list using loaded by plugin manager ones
	VersioningPluginsManager(qrRepo::RepoControlInterface *repoApi
		, ErrorReporterInterface *errorReporter, ProjectManager * projectManager); // MainWindow used till refactoring

	void initFromToolPlugins(QListIterator<ToolPluginInterface *> iterator
			, MainWindow *mainWindow);

	BriefVersioningInterface *activeClient(const QString &workingDir);

	bool onFileAdded(QString const &filePath, QString const &workingDir);
	bool onFileRemoved(QString const &filePath, QString const &workingDir);
	bool onFileChanged(QString const &filePath, QString const &workingDir);
	TransparentMode *getLinkOnTransparentMode();

signals:
	void OnButton(bool);
	void transparentClassIsReady();
	void viewForTransparentModeIsReady(QGraphicsView*);

public slots:
	void beginWorkingCopyDownloading(QString const &repoAddress
			, QString const &targetProject
			, QString revisionNumber = "-1"
			, bool quiet = false);
	void beginWorkingCopyUpdating(QString const &targetProject = QString());
	void beginChangesSubmitting(QString const &description, QString const &targetProject = QString(), bool const &quiet = false);
	bool reinitWorkingCopy(QString const &targetProject = QString());
	QString information(QString const &targetProject = QString());
	int revisionNumber(QString const &targetProject = QString());
	QString remoteRepositoryUrl(QString const &targetProject = QString());
	bool isMyWorkingCopy(QString const &directory = QString(), bool const &quiet = false
						, bool const &prepareAndProcess = false);
	QString friendlyName();
	QString getLog(QString const &format = QString(), bool const &quiet = false);
	void setVersion(QString hash, bool const &quiet = false);
	void switchOffOrOnAllPluginsAction(bool switchOnTranspMode);
	void showDiff(QString fstHash, QString sndHash);

private slots:
	void onWorkingCopyDownloaded(bool const success, QString const &targetProject);
	void onWorkingCopyUpdated(bool const success);
	void onChangesSubmitted(bool const success);

private:
	QString tempFolder() const;
	void prepareWorkingCopy();
	VersioningPluginInterface *activePlugin(bool needPreparation = true, QString const &workingDir = "");
	WorkingCopyInspectionInterface *activeWorkingCopyInspector(QString const &workingDir = "");

	void reportError(QString const &message);
	void reportErrors(QStringList const &messages);
	void reportWarning(QString const &message);
	void reportWarnings(QStringList const &messages);

	qrRepo::RepoControlInterface *mRepoApi;
	QList<VersioningPluginInterface *> mPlugins;
	ErrorReporterInterface *mErrorReporter;
	DiffPluginBase *mDiffPlugin;
	QString mTempDir;

	TransparentMode *mTranspaentMode;
	ProjectManager *mProjectManager;
};

}
