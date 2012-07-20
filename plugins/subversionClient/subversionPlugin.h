#pragma once

#include "../../qrutils/versioningUtils/externalClientPluginBase.h"
#include "../../qrgui/toolPluginInterface/customizer.h"
#include "viewInteraction.h"

namespace versioning
{

class SubversionPlugin : public qReal::versioning::ExternalClientPluginBase
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)

public:
	SubversionPlugin();
	virtual ~SubversionPlugin();

	QString pathToSvn() const;

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
	virtual void beginWorkingCopyDownloading(QString const &repoAddress,
									 QString const &targetProject,
									 int revisionNumber = -1);
	virtual void beginWorkingCopyUpdating();
	virtual void beginChangesSubmitting(const QString &description);
	virtual bool reinitWorkingCopy();
	virtual QString information();
	virtual int revisionNumber();
	virtual bool isMyWorkingCopy(QString const &directory = "");

	void editProxyConfiguration();

public slots:
	void startCheckout(QString const &from
			, QString const &targetProject = ""
			, QString const &targetFolder = "");
	void startUpdate(QString const &to = "");
	void startCommit(QString const &message = "", QString const &from = "");
	bool doCleanUp(QString const &what = "");
	void startRevert(QString const &what = "");
	bool doAdd(QString const &what, bool force = true);
	bool doRemove(QString const &what, bool force = true);
	QString info(QString const &target = "", bool const reportErrors = true);
	QString repoUrl(QString const &target = "", bool const reportErrors = false);
	int currentRevision(QString const &target = "", bool const reportErrors = false);

signals:
	void workingCopyDownloaded(const bool success);
	void workingCopyUpdated(const bool success);
	void changesSubmitted(const bool success);

	void checkoutComplete(bool const success, QString const &targetProject);
	void updateComplete(bool const success);
	void commitComplete(bool const success);
	void revertComplete(bool const success);
	void cleanUpComplete(bool const success);
	void addComplete(bool const success);
	void removeComplete(bool const success);
	void operationComplete(QString const &name, bool const success);

protected:
	// External client overloads
	virtual int timeout() const;
	virtual QString tempFolder() const;

private:
	QString infoToRepoUrl(QString &repoInfo);
	int infoToRevision(QString const &repoInfo);

	void onCheckoutComplete(bool const result, QString const &targetProject);
	void onUpdateComplete(bool const result);
	void onCommitComplete(bool const result);
	void onRevertComplete(bool const result);

	QStringList authenticationArgs() const;

	details::ViewInteraction *mViewInteraction;
	QString mTempCheckoutDir;

};

}
