#pragma once

#include "qrutils/versioningUtils/briefVersioningInterface.h"
#include <qrgui/plugins/toolPluginInterface/toolPluginInterface.h>
#include <qrgui/plugins/toolPluginInterface/pluginInterface.h>
#include "versioningPluginInterface.h"
#include "qrgui/models/models.h"
#include "qrgui/mainWindow/projectManager/projectManager.h"
#include "qrgui/versioning/versioningPluginsManagerDeclSpec.h"

namespace qReal
{

class /*QRGUI_VERSIONING_PLUGINS_MANAGER_EXPORT*/ VersioningPluginsManager
		: public BriefVersioningInterface
		, public qrRepo::WorkingCopyInspectionInterface
{
	Q_OBJECT

public:
	/// Inits plugin list using loaded by plugin manager ones
	VersioningPluginsManager(qrRepo::RepoControlInterface *repoApi
		, ErrorReporterInterface *errorReporter, ProjectManager * projectManager);

	void initFromToolPlugins(QListIterator<ToolPluginInterface *> iterator
			, EditorManagerInterface *editorManager, QWidget *parent);

	BriefVersioningInterface *activeClient(const QString &workingDir);

	bool onFileAdded(QString const &filePath, QString const &workingDir);
	bool onFileRemoved(QString const &filePath, QString const &workingDir);
	bool onFileChanged(QString const &filePath, QString const &workingDir);

public slots:
	void beginWorkingCopyDownloading(QString const &repoAddress
			, QString const &targetProject
			, QString commitId = "-1"
			, bool quiet = false);
	void beginWorkingCopyUpdating(QString const &targetProject = QString());
	void beginChangesSubmitting(QString const &description, QString const &targetProject = QString(), bool const &quiet = false);
	bool reinitWorkingCopy(QString const &targetProject = QString());
	QString information(QString const &targetProject = QString());
	QString commitId(QString const &targetProject = QString());
	QString remoteRepositoryUrl(QString const &targetProject = QString());
	bool isMyWorkingCopy(QString const &directory = QString(), bool const &quiet = false
						, bool const &prepareAndProcess = false);
	QString friendlyName();
	bool clientExist();

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
	DiffPluginInterface *mDiffInterface;
	ProjectManager *mProjectManager;
	QString mTempDir;
};

}
