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
	virtual bool onFileAdded(const QString &filePath, const QString &workingDir);
	virtual bool onFileRemoved(const QString &filePath, const QString &workingDir);
	virtual bool onFileChanged(const QString &filePath, const QString &workingDir);

	// Brief VCS interface
	virtual bool downloadWorkingCopy(QString const &repoAddress,
									 QString const &targetProject,
									 int revisionNumber = -1);
	virtual bool updateWorkingCopy();
	virtual bool submitChanges(const QString &description);
	virtual bool reinitWorkingCopy();
	virtual QString information();
	virtual int revisionNumber();
	virtual bool isMyWorkingCopy(QString const &directory = "");

	void editProxyConfiguration();

public slots:
	bool doCheckout(QString const &from
			, QString const &targetProject = ""
			, QString const &targetFolder = "");
	bool doUpdate(QString const &to = "");
	bool doCommit(QString const &message = "", QString const &from = "");
	bool doCleanUp(QString const &what = "");
	bool doRevert(QString const &what = "");
	bool doAdd(QString const &what, bool force = true);
	bool doRemove(QString const &what, bool force = true);
	QString info(QString const &target = "");
	QString repoUrl(QString const &target = "");
	int currentRevision(QString const &target = "");

signals:
	void checkoutComplete(bool success);
	void updateComplete(bool success);
	void commitComplete(bool success);
	void revertComplete(bool success);
	void cleanUpComplete(bool success);
	void addComplete(bool success);
	void removeComplete(bool success);
	void operationComplete(QString const &name, bool success);

protected:
	// External client overloads
	virtual int timeout() const;
	virtual QString tempFolder() const;

private:
	QString infoToRepoUrl(QString &repoInfo);
	int infoToRevision(QString const &repoInfo);

	QStringList authenticationArgs() const;

	details::ViewInteraction *mViewInteraction;
	QString mTempCheckoutDir;

};

}
