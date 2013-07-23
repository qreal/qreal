#include <QtWidgets/QApplication>

#include "gitPlugin.h"
#include "../../qrkernel/settingsManager.h"
#include "../../qrutils/fileSystemUtils.h"
#include "tags.h"

using namespace versioning;
using namespace qReal::versioning;

QString const tempFolderName = "tempGit";
int const defaultTimeout = 30000;

GitPlugin::GitPlugin()
	: mViewInteraction(new details::ViewInteraction(this))
	, mTempDir(tempFolderName)
{
	qReal::SettingsManager::instance()->setValue("gitTempDir", mTempDir);
	setPathToClient(pathToGit());

	connect(this, SIGNAL(operationIsFinished(QVariant&)), SLOT(doAfterOperationIsFinished(QVariant&)));
}

GitPlugin::~GitPlugin()
{
}

// QReal plugin interface
QString GitPlugin::pathToGit() const
{
	return qReal::SettingsManager::value("pathToGitClient", "").toString();
}

qReal::Customizer *GitPlugin::customizationInterface()
{
	return NULL;
}

void GitPlugin::updateSettings()
{
	setPathToClient(pathToGit());
}

QList<qReal::ActionInfo> GitPlugin::actions()
{
	return mViewInteraction->actions();
}

void GitPlugin::init(qReal::PluginConfigurator const &configurator)
{
	ExternalClientPluginBase::init(configurator);
	mViewInteraction->init(configurator);
}

QPair<QString, PreferencesPage *> GitPlugin::preferencesPage()
{
	return mViewInteraction->preferencesPage();
}

bool GitPlugin::onFileAdded(QString const &filePath, QString const &workingDir)
{
	Q_UNUSED(workingDir)
	return doAdd(filePath, workingDir);
}

bool GitPlugin::onFileRemoved(QString const &filePath, QString const &workingDir)
{
	Q_UNUSED(workingDir)
	return doRemove(filePath);
}

bool GitPlugin::onFileChanged(QString const &filePath, QString const &workingDir)
{
	// Subversion detects modifications itself
	Q_UNUSED(filePath)
	Q_UNUSED(workingDir)
	return true;
}

void GitPlugin::beginWorkingCopyDownloading(QString const &repoAddress
		, QString const &targetProject
		, int revisionNumber, bool quiet)
{
	doInit(tempFolder(), quiet);
}

void GitPlugin::beginWorkingCopyUpdating(QString const &targetProject)
{
	startPush(tempFolder(), targetProject);
}

void GitPlugin::beginChangesSubmitting(QString const &description, QString const &targetProject)
{
	//doAdd(QString(), QString());
	startCommit(tempFolder(), description, targetProject);
}

bool GitPlugin::reinitWorkingCopy(QString const &targetProject)
{
	return doClean();
}

QString GitPlugin::information(QString const &targetProject)
{
	return "d";
	//return info(tempFolder(), true, targetProject);
}

int GitPlugin::revisionNumber(QString const &targetProject)
{
	return 4;
	//return currentRevision(tempFolder(), false, targetProject);
}

QString GitPlugin::remoteRepositoryUrl(QString const &targetProject)
{
	return "r";
	//return repoUrl(tempFolder(), false, targetProject);
}

bool GitPlugin::isMyWorkingCopy(QString const &directory)
{
	//If svn info worked well then it is our dir
	QStringList infoArgs;
	infoArgs << "status" << (directory.isEmpty() ? tempFolder() : directory);
	return invokeOperation(infoArgs, false, directory, false, false, QString(), QString(), false);
}

int GitPlugin::timeout() const
{
	return qReal::SettingsManager::value("gitClientTimeout", defaultTimeout).toInt();
}

QString GitPlugin::tempFolder() const
{
	return qReal::SettingsManager::value("gitTempDir", mTempDir).toString();
}

void GitPlugin::doInit(QString const &targetFolder, bool quiet)
{
	QStringList arguments;
	arguments  << "init";
	invokeOperation(arguments, true, QString(), false, true, QString(), QString(), true);
}

void GitPlugin::startClone(QString const &from
		, QString const &targetFolder)
{
	QString cloneDist = targetFolder.isEmpty() ? tempFolder() : targetFolder;
	QStringList arguments;
	arguments << "clone" << from;

	Tag tagStruct("clone");
	QVariant tagVariant;
	tagVariant.setValue(tagStruct);
	invokeOperationAsync(arguments, tagVariant, false, cloneDist, QString(), false);
}

void GitPlugin::startCommit(QString const &message, QString const &from
		, QString const &sourceProject)
{
	//QString targetDir = from.isEmpty() ? tempFolder() : from;
	bool flag = doUserEmailConfig() && doUserNameConfig();

	QStringList arguments;
	arguments << "commit" << "-m" << message;

	Tag tagStruct("commit");
	QVariant tagVariant;
	tagVariant.setValue(tagStruct);
	invokeOperationAsync(arguments, tagVariant, true, QString(), sourceProject);
}

void GitPlugin::doRemote(QString const &remote, QString const &adress, QString const &targetFolder)
{
	QString targetDir = targetFolder.isEmpty() ? tempFolder() : targetFolder;
	QStringList arguments;
	arguments << "remote" << "add" << remote << adress;

	bool const result = invokeOperation(arguments, false, targetDir, false, false, QString(), QString(), true);
	addComplete(result);
	operationComplete("add", result);
}

void GitPlugin::startPush(QString const &remote
		, QString const &sourceProject
		,QString const &targetFolder)
{
	QString targetDir = targetFolder.isEmpty() ? tempFolder() : targetFolder;
	QStringList arguments;

	arguments << "push" << "--repo" << "https://" + getUsername() + ":" + getPassword() + "@github.com/" + getUsername() + "/" + remote + ".git";

	Tag tagStruct("push");
	QVariant tagVariant;
	tagVariant.setValue(tagStruct);
	invokeOperationAsync(arguments, tagVariant, true, targetDir, sourceProject);
}


void GitPlugin::startPull(const QString &remote, QString const &targetFolder)
{
	QString targetDir = targetFolder.isEmpty() ? tempFolder() : targetFolder;
	QStringList arguments;

	arguments << "pull" << remote;

	Tag tagStruct("pull");
	QVariant tagVariant;
	tagVariant.setValue(tagStruct);
	invokeOperationAsync(arguments, tagVariant, false, QString(), QString(), false);
}

void GitPlugin::startReset(QString const &hash, QString const &targetFolder)
{
	QString targetDir = targetFolder.isEmpty() ? tempFolder() : targetFolder;
	QStringList arguments;
	// TODO: Add different variants
	arguments << "reset" << hash;

	Tag tagStruct("reset");
	QVariant tagVariant;
	tagVariant.setValue(tagStruct);
	invokeOperationAsync(arguments, tagVariant, true, targetDir, QString(), true, true);
}

bool GitPlugin::doUserNameConfig()
{
	QString const enabledKey = ui::AuthenticationSettingsWidget::enabledSettingsName("git");
	QString const usernameKey = ui::AuthenticationSettingsWidget::usernameSettingsName("git");

	bool const authenticationEnabled = qReal::SettingsManager::value(enabledKey, false).toBool();
	if (!authenticationEnabled) {
		return false;
	}

	QString const username = qReal::SettingsManager::value(usernameKey, false).toString();
	if (username.isEmpty()) {
		return false;
	}

	QStringList arguments;
	arguments << "config" << "user.name" << "\"" + username + "\"";
	bool const result = invokeOperation(arguments, false, QString(), false, false, QString(), QString(), true);
	emit operationComplete("userName", result);
	return result;
}

bool GitPlugin::doUserEmailConfig()
{
	QString const enabledKey = ui::AuthenticationSettingsWidget::enabledSettingsName("git");
	QString const emailKey = ui::AuthenticationSettingsWidget::usernameSettingsName("git");

	bool const authenticationEnabled = qReal::SettingsManager::value(enabledKey, false).toBool();
	if (!authenticationEnabled) {
		return false;
	}

	QString const email = qReal::SettingsManager::value(emailKey, false).toString();
	if (email.isEmpty()) {
		return false;
	}

	QStringList arguments;
	arguments << "config" << "user.email" << email;
	bool const result = invokeOperation(arguments, false, QString(), false, false, QString(), QString(), true);
	emit operationComplete("userEmail", result);
	return result;
}

bool GitPlugin::doAdd(QString const &what, QString const &targetFolder, bool force)
{
	QString targetDir = targetFolder.isEmpty() ? tempFolder() : what;
	QStringList arguments;
	arguments << "add" << what;

	QString path = what;
	path = getFilePath(path);

	bool const result = invokeOperation(arguments, true, path, false, false, QString(), QString(), true);
	emit addComplete(result);
	emit operationComplete("add", result);
	return result;
}

QString &GitPlugin::getFilePath(QString &adress)
{
	int pos = adress.lastIndexOf("/");
	int len = adress.length();
	return adress.remove(pos, len - pos);
}

bool GitPlugin::doRemove(QString const &what, bool force)
{
	QStringList arguments;
	arguments << "rm" << what;

	QString path = what;
	path = getFilePath(path);

	bool const result = invokeOperation(arguments, true, path, false, false, QString(), QString(), true);
	emit removeComplete(result);
	emit operationComplete("rm", result);
	return result;
}

bool GitPlugin::doClean()
{
	QStringList arguments;
	arguments << "clean" << "-xdf";
	bool const result = invokeOperation(arguments, true, QString(), true, true, QString(), QString());
	emit cleanComplete(result);
	emit operationComplete("clean", result);
	return result;
}

void GitPlugin::doAfterOperationIsFinished(QVariant &tag)
{
	Tag tagStruct = tag.value<Tag>();
	if (tagStruct.operation == "init"){
		onInitComplete(true);
	} else if (tagStruct.operation == "commit"){
		onCommitComplete(true);
	} else if (tagStruct.operation == "clone"){
		onCloneComplete(true);
	} else if (tagStruct.operation == "remote"){
		onRemoteComplete(true);
	} else if (tagStruct.operation == "push"){
		onPushComplete(true);
	} else if (tagStruct.operation == "pull"){
		onPullComplete(true);
	} else if (tagStruct.operation == "reset"){
		onResetComplete(true);
	}
}

QString GitPlugin::getUsername()
{
	QString const usernameKey = ui::AuthenticationSettingsWidget::usernameSettingsName("git");
	QString const username = qReal::SettingsManager::value(usernameKey, false).toString();
	return username;
}

QString GitPlugin::getPassword()
{
	QString const passwordKey = ui::AuthenticationSettingsWidget::passwordSettingsName("git");
	QString const password = qReal::SettingsManager::value(passwordKey, false).toString();
	return password;
}

void GitPlugin::onInitComplete(bool const result)
{
	processWorkingCopy();
	emit initComplete(result);
	emit operationComplete("init", result);
}

void GitPlugin::onCommitComplete(bool const result)
{
	processWorkingCopy();
	emit commitComplete(result);
	emit operationComplete("commit", result);
	emit changesSubmitted(result);
}

void GitPlugin::onCloneComplete(bool const result)
{
	processWorkingCopy();
	emit cloneComplete(result);
	emit operationComplete("clone", result);
}

void GitPlugin::onRemoteComplete(bool const result)
{
	processWorkingCopy();
	emit remoteComplete(result);
	emit operationComplete("remote", result);
}

void GitPlugin::onPushComplete(bool const result)
{
	processWorkingCopy();
	emit pushComplete(result);
	emit operationComplete("push", result);
}

void GitPlugin::onPullComplete(bool const result)
{
	processWorkingCopy();
	emit pullComplete(result);
	emit operationComplete("pull", result);
}

void GitPlugin::onResetComplete(const bool result)
{
	processWorkingCopy();
	emit resetComplete(result);
	emit operationComplete("reset", result);
}

QString GitPlugin::friendlyName()
{
	return "Git Plugin";
}

void GitPlugin::setVersion(QString hash)
{
	qDebug() << "switch to version " << hash << "ok";
}

void GitPlugin::initializeLocalRepo()
{
	QStringList arguments;
	arguments << "init";
	invokeOperation(arguments, true, QString(), false, true, QString(), QString(),true);
	arguments.clear();
	arguments << "config" << "--local" << "user.name" << "\"testName\"";
	invokeOperation(arguments, true, QString(), false, true, QString(), QString(),true);
	arguments.clear();
	arguments << "config" << "--local" << "user.email" << "testName@mael.com";
	invokeOperation(arguments, true, QString(), false, true, QString(), QString(),true);
}

QString GitPlugin::getLog(QStringList const &format, bool const &quiet)
{
	QString testLog = "7941425c938e7c7ce7f8c23a9a034be34edd6486 - ZiminGrigory,";
	testLog += "Thu Jul 11 15:26:02 2013 +0400 : fix bug ctrl+c, add easyVersioning architecture";
	testLog += "\nc16cea4f66e1a9b3e6ba8b3b4242175a29dfab68";
	testLog += " - Dmitry Mordvinov, Wed Jul 3 20:59:50 2013 +0400 : Corrected path in previous commit";

	return testLog;
}

