#pragma once

#include "../../qrutils/versioningUtils/externalClientPluginBase.h"
#include "../../qrgui/plugins/toolPluginInterface/customizer.h"
#include "viewInteraction.h"


namespace git
{

class GitPlugin : public qReal::versioning::ExternalClientPluginBase
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)
	Q_PLUGIN_METADATA(IID "qReal.versioning.git")
public:
	GitPlugin();

	virtual ~GitPlugin();

	QString pathToGit() const;

	// QReal plugin interface
	virtual qReal::Customizer* customizationInterface();
	virtual void updateSettings();
	virtual QList<qReal::ActionInfo> actions();
	virtual void init(qReal::PluginConfigurator const &configurator);
	virtual QPair<QString, qReal::gui::PreferencesPage *> preferencesPage();

	// Working copy inspection
	virtual bool onFileAdded(QString const &filePath, QString const &workingDir);
	virtual bool onFileRemoved(QString const &filePath, QString const &workingDir);
	virtual bool onFileChanged(QString const &filePath, QString const &workingDir);

	// Brief VCS interface
	virtual void beginWorkingCopyDownloading(QString const &repoAddress
			, QString const &targetProject
			, QString commitId = "-1"
			, bool quiet = false);
	virtual void beginWorkingCopyUpdating(QString const &targetProject = QString());
	virtual void beginChangesSubmitting(const QString &description, QString const &targetProject = QString()
											, bool const &quiet = false);
	virtual bool reinitWorkingCopy(QString const &targetProject = QString());
	virtual QString information(QString const &targetProject = QString());
	virtual QString commitId(QString const &targetProject = QString());
	virtual QString remoteRepositoryUrl(QString const &targetProject = QString());
	virtual bool isMyWorkingCopy(QString const &directory = QString(), bool const &quiet = false
								, bool const &prepareAndProcess = false);
	virtual QString friendlyName();
	bool clientExist();

	virtual QString getLog(QString const &format = QString(), bool const &quiet = false);
	virtual void setVersion(QString hash, bool const &quiet = false);
//	void setDiffViewerInterface(DiffPluginInterface *diffInterface);

public slots:
	void doInit(QString const &targetFolder = QString(), bool const &quiet = false);
	void startClone(QString const &from = QString()
					, QString const &targetProject = QString());
	void startCommit(QString const &message, QString const &from  = QString()
			, QString const &sourceProject = QString(), bool const &quiet = false);
	void doRemote(QString const &remote, QString const &adress
			, QString const &targerFolder = QString());
	void startPush(QString const &remote, QString const &sourceProject = QString()
			, QString const &targetFolder = QString());
	void startPull(QString const &remote, QString const &targetFolder = QString());
	void startReset(QString const &hash = QString(), QString const &targetFolder = QString(), bool const &quiet = false);
	bool doAdd(QString const &what, QString const &targetFolder, bool force = true);
	bool doRemove(QString const &what, bool force = true);
	bool doClean(const QString &targetProject = QString());
	QString doStatus(const QString &targetProject = QString());
	QString doLog(QString const &format = QString(), bool const &quiet = false, bool const &showDialog = false);
	QString doRemoteList();
	void doAfterOperationIsFinished(QVariant const &tag);
	void checkClientInstalling();

	void showDiff(QString const &targetProject, QWidget *widget, bool const &compactMode = true);
	void showDiff(QString oldHash, QString newHash
				  , QString const &targetProject, QWidget *widget, const bool &compactMode = true);
	void showDiff(QString oldhash, QString const &targetProject, QWidget *widget, const bool &compactMode = true);

signals:
	void workingCopyDownloaded(const bool success, QString const &targetProject);
	void workingCopyUpdated(const bool success);
	void changesSubmitted(const bool success);
	void clientInstalled(QString client, bool exist);

	void initComplete(bool const success);
	void cloneComplete(bool const success);
	void commitComplete(bool const success);
	void remoteComplete(bool const success);
	void pushComplete(bool const success);
	void pullComplete(bool const success);
	void resetComplete(bool const success);
	void cleanComplete(bool const success);
	void addComplete(bool const success);
	void removeComplete(bool const success);
	void statusComplete(QString const &answer, bool const success);
	void logComplete(QString const &answer, bool const success);
	void remoteListComplete(QString const &answer, bool const success);
	void operationComplete(QString const &name, bool const success);

protected:
	// External client overloads
	virtual int timeout() const;
	virtual QString tempFolder() const;

private:
	void onCloneComplete(bool const result, const bool quiet);
	void onRemoteComplete(bool const result);
	void onPushComplete(bool const result);
	void onPullComplete(bool const result);
	void onResetComplete(bool const result, const bool quiet);

	QString &getFilePath(QString &adress);
	QString getUsername();
	QString getPassword();
	void doUserNameConfig();
	void doUserEmailConfig();

	details::ViewInteraction *mViewInteraction;
	QString mTempDir;
//	DiffPluginInterface *mDiffInterface;
};
}
