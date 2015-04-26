#pragma once

#include <qrutils/versioningUtils/externalClientPluginBase.h>
#include <qrgui/plugins/toolPluginInterface/customizer.h>

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
	virtual bool onFileAdded(const QList<QString> &list, QString const &workingDir);
	virtual bool onFileRemoved(const QList<QString> &list, QString const &workingDir);
	virtual bool onFileChanged(const QList<QString> &list, QString const &workingDir);

	// Brief VCS interface
	virtual void beginWorkingCopyDownloading(
		QString const &repoAddress
		, QString const &targetProject
		, QString commitId = "-1"
		, bool quiet = false
	);
	virtual void beginWorkingCopyUpdating(QString const &targetProject = QString());
	virtual void beginChangesSubmitting(
		QString const &description
		, QString const &targetProject = QString()
		, bool quiet = false
	);
	virtual bool reinitWorkingCopy(QString const &targetProject = QString());
	virtual QString information(QString const &targetProject = QString());
	virtual QString commitId(QString const &targetProject = QString());
	virtual QString remoteRepositoryUrl(QString const &targetProject = QString());
	virtual bool isMyWorkingCopy(
		QString const &directory = QString()
		, bool quiet = false
		, bool prepareAndProcess = false
	);
	virtual QString friendlyName();
	bool clientExist();

	virtual QString getLog(QString const &format = QString(), bool quiet = false);
	virtual void setVersion(QString hash, bool quiet = false);
	void setDiffViewerInterface(qReal::DiffPluginInterface *diffInterface);

public slots:
	void doInit(QString const &targetFolder = QString()
		, bool prepareAndProcess = true
		, bool quiet = false
	);
	void startClone(QString const &from = QString(), QString const &targetProject = QString());
	void startCommit(
		QString const &message
		, QString const &from  = QString()
		, QString const &sourceProject = QString()
		, bool quiet = false
	);
	void doRemote(QString const &remote, QString const &adress, QString const &targerFolder = QString());
	void startPush(
		QString const &remote
		, QString const &sourceProject = QString()
		, QString const &targetFolder = QString()
	);
	void startPull(QString const &remote, QString const &targetFolder = QString());
	void startReset(QString const &hash = QString(), QString const &targetFolder = QString(), bool quiet = false);
	bool doAdd(QString const &what, QString const &targetFolder, bool force = true);
	bool doRemove(QString const &what, bool prepare = true, bool process = true, bool force = true);
	bool doClean(const QString &targetProject = QString());
	QString doStatus(const QString &targetProject = QString());
	QString doLog(QString const &format = QString(), bool quiet = false, bool showDialog = false);
	QString doRemoteList();
	void doAfterOperationIsFinished(QVariant const &tag, bool result);
	void checkClientInstalling();

	//For branches
	void deleteBranch(QString const &branchName);
	void startCheckoutBranch(QString const &branchName, QString const &targetFolder = QString());
	void createBranch(QString const &branchName);
	QString getBranchesList();
	void startMergeBranch(QString const &targetBranchName);

	//Difference between commits
	void showDiff(const QString &targetProject, QWidget *widget, bool compactMode = true);
	void showDiff(const QString &oldHash, const QString &newHash
				  , QString const &targetProject, QWidget *widget, bool compactMode = true);
	void showDiff(const QString &oldhash, QString const &targetProject, QWidget *widget, bool compactMode = true);

signals:
	void workingCopyDownloaded(bool success, QString const &targetProject);
	void workingCopyUpdated(bool success);
	void changesSubmitted(bool success);
	void clientInstalled(QString client, bool exist);

	void initComplete(bool success);
	void cloneComplete(bool success);
	void commitComplete(bool success);
	void remoteComplete(bool success);
	void pushComplete(bool success);
	void pullComplete(bool success);
	void resetComplete(bool success);
	void cleanComplete(bool success);
	void addComplete(bool success);
	void checkoutComplete(bool success);
	void deleteBranchComplete(bool success);
	void createBranchComplete(bool success);
	void removeComplete(bool success);
	void statusComplete(QString const &answer, bool success);
	void logComplete(QString const &answer, bool success);
	void remoteListComplete(QString const &answer, bool success);
	void operationComplete(QString const &name, bool success);

protected:
	// External client overloads
	friend class TransparentMode;
	virtual int timeout() const;
	virtual QString tempFolder() const;

private:
	void onCloneComplete(bool result, bool quiet);
	void onRemoteComplete(bool result);
	void onPushComplete(bool result);
	void onPullComplete(bool result);
	void onResetComplete(bool result, bool quiet);
	void onCheckoutComplete(bool result);

	QString &getFilePath(QString &adress);
	QString getUsername();
	QString getPassword();
	void doUserNameConfig();
	void doUserEmailConfig();

	details::ViewInteraction *mViewInteraction;
	QString mTempDir;
	qReal::DiffPluginInterface *mDiffInterface;
};
}
