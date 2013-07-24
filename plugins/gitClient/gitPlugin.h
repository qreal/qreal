#pragma once

#include "../../qrutils/versioningUtils/externalClientPluginBase.h"
#include "../../qrgui/toolPluginInterface/customizer.h"
#include "viewInteraction.h"

namespace versioning
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
	virtual QPair<QString, PreferencesPage *> preferencesPage();

	// Working copy inspection
	virtual bool onFileAdded(QString const &filePath, QString const &workingDir);
	virtual bool onFileRemoved(QString const &filePath, QString const &workingDir);
	virtual bool onFileChanged(QString const &filePath, QString const &workingDir);

	// Brief VCS interface
	virtual void beginWorkingCopyDownloading(QString const &repoAddress
			, QString const &targetProject
			, int revisionNumber = -1
			, bool quiet = false);
	virtual void beginWorkingCopyUpdating(QString const &targetProject = QString());
	virtual void beginChangesSubmitting(const QString &description, QString const &targetProject = QString());
	virtual bool reinitWorkingCopy(QString const &targetProject = QString());
	virtual QString information(QString const &targetProject = QString());
	virtual int revisionNumber(QString const &targetProject = QString());
	virtual QString remoteRepositoryUrl(QString const &targetProject = QString());
	virtual bool isMyWorkingCopy(QString const &directory = QString());
	virtual QString friendlyName();
	virtual QString getLog(QStringList const &format = QStringList(), bool const &quiet = false);
	virtual void setVersion(QString hash);

public slots:
	void doInit(QString const &targetFolder = QString(), bool quiet = false);
	void startClone(QString const &from = QString()
					, QString const &targetFolder = QString());
	void startCommit(QString const &message, QString const &from  = QString()
			, QString const &sourceProject = QString());
	void doRemote(QString const &remote, QString const &adress
			, QString const &targerFolder = QString());
	void startPush(QString const &remote, QString const &sourceProject = QString()
			, QString const &targetFolder = QString());
	void startPull(QString const &remote, QString const &targetFolder = QString());
	void startReset(QString const &hash = QString(), QString const &targetFolder = QString());
	bool doAdd(QString const &what, QString const &targetFolder, bool force = true);
	bool doRemove(QString const &what, bool force = true);
	bool doClean();
	bool doUserNameConfig();
	bool doUserEmailConfig();
	QString doStatus();
	QString doLog(QStringList const &format = QStringList(), bool const &quiet = false);
	QString doRemoteList();
	void doAfterOperationIsFinished(QVariant const &tag);
signals:
	void workingCopyDownloaded(const bool success, QString const &targetProject);
	void workingCopyUpdated(const bool success);
	void changesSubmitted(const bool success);

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
	void onCloneComplete(bool const result);
	void onRemoteComplete(bool const result);
	void onCommitComplete(bool const result);
	void onPushComplete(bool const result);
	void onPullComplete(bool const result);
	void onResetComplete(bool const result);

	QString &getFilePath(QString &adress);
	QString getUsername();
	QString getPassword();

	details::ViewInteraction *mViewInteraction;
	QString mTempDir;

};
}
