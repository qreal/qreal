#include <QtWidgets/QApplication>

#include <QtCore/QRegExp>
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
	startCommit(tempFolder(), description, targetProject);
}

bool GitPlugin::reinitWorkingCopy(QString const &targetProject)
{
	return doClean();
}

QString GitPlugin::information(QString const &targetProject)
{
	return doStatus();
}

int GitPlugin::revisionNumber(QString const &targetProject)
{
	return 4;
	//return currentRevision(tempFolder(), false, targetProject);
}

QString GitPlugin::remoteRepositoryUrl(QString const &targetProject)
{
	return qReal::SettingsManager::value("remoteAdress", "").toString();
}

bool GitPlugin::isMyWorkingCopy(QString const &directory)
{
	QStringList infoArgs;
	infoArgs << "remote" << "show";
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

QString GitPlugin::friendlyName()
{
	return "Git Plugin";
}

void GitPlugin::setVersion(QString hash)
{
	this->startReset(hash);
}

QString GitPlugin::getLog(QStringList const &format, bool const &quiet)
{
	return doLog(format, quiet);
}

void GitPlugin::doInit(QString const &targetFolder, bool quiet)
{
	QStringList arguments;
	arguments  << "init";
	bool result = invokeOperation(arguments, true, QString(), false, true, targetFolder, QString(), quiet);
	emit initComplete(result);
	arguments.clear();
	arguments << "add" << "-A";
	invokeOperation(arguments, true, QString(), true, true, targetFolder, QString(), quiet);
}

void GitPlugin::startClone(QString const &from
		, QString const &targetFolder)
{
	//QString cloneDist = targetFolder.isEmpty() ? tempFolder() : targetFolder;
	QStringList arguments;
	arguments << "clone" << from + ".git";

	const Tag tagStruct("clone");
	QVariant tagVariant;
	tagVariant.setValue(tagStruct);
	invokeOperationAsync(arguments, tagVariant, false, "", QString(), false);
}

void GitPlugin::startCommit(QString const &message, QString const &from
		, QString const &sourceProject)
{
	if (!(doUserEmailConfig() && doUserNameConfig()))
		return;

	QStringList arguments;
	arguments << "commit" << "-m" << message;

	const Tag tagStruct("commit");
	QVariant tagVariant;
	tagVariant.setValue(tagStruct);
	invokeOperationAsync(arguments, tagVariant, true, QString(), sourceProject);
}

void GitPlugin::doRemote(QString const &remote, QString const &adress, QString const &targetFolder)
{
	QStringList arguments;
	arguments << "remote" << "add" << remote << adress + ".git";

	bool const result = invokeOperation(arguments, true, QString(), false, true, QString(), QString());
	addComplete(result);
	operationComplete("add", result);
	emit operationComplete("remote", result);
}

void GitPlugin::startPush(QString const &remote
						, QString const &sourceProject
						, QString const &targetFolder)
{
	//QString targetDir = targetFolder.isEmpty() ? tempFolder() : targetFolder;
	QStringList arguments;

	arguments << "push" << "--repo" << "https://" + getUsername() + ":" + getPassword() + "@github.com/" + getUsername() + "/" + remote + ".git";

	const Tag tagStruct("push");
	QVariant tagVariant;
	tagVariant.setValue(tagStruct);
	invokeOperationAsync(arguments, tagVariant, true, QString(), sourceProject);
}

void GitPlugin::startPull(const QString &remote, QString const &targetFolder)
{
	QStringList arguments;
	arguments << "pull" << remote;

	const Tag tagStruct("pull");
	QVariant tagVariant;
	tagVariant.setValue(tagStruct);
	invokeOperationAsync(arguments, tagVariant, false, QString(), QString(), false);
}

void GitPlugin::startReset(QString const &hash, QString const &targetFolder)
{
	//QString targetDir = targetFolder.isEmpty() ? tempFolder() : targetFolder;
	QStringList arguments;
	// TODO: Add different variants
	arguments << "reset" << hash;

	const Tag tagStruct("reset");
	QVariant tagVariant;
	tagVariant.setValue(tagStruct);
	invokeOperationAsync(arguments, tagVariant, true, QString(), QString(), true, true);
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
	QStringList arguments;
	arguments << "add" << what;

	QString path = what;
	path = getFilePath(path);

	bool const result = invokeOperation(arguments, false, path, false, false, targetFolder, QString(), force);
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

	if (force) {
		arguments.append("-f");
	}

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

QString GitPlugin::doStatus()
{
	int result = invokeOperation(QStringList() << "status");
	QString answer = standartOutput();
	emit statusComplete(answer, result);
	return answer;
}

QString GitPlugin::doLog(const QStringList &format, const bool &quiet)
{
	int result = invokeOperation(QStringList() << "log" << format, true, QString(), true, true, QString(), QString(), quiet);
	QString answer = standartOutput();
	emit logComplete(answer, result);
	return answer;
}

QString GitPlugin::doRemoteList()
{
	int result = invokeOperation(QStringList() << "remote" << "-v");
	QString answer = standartOutput();
	emit remoteListComplete(answer, result);
	return answer;
}

void GitPlugin::doAfterOperationIsFinished(QVariant const &tag)
{
	Tag tagStruct = tag.value<Tag>();
	if (tagStruct.operation == "commit"){
		onCommitComplete(true);
	} else if (tagStruct.operation == "clone"){
		onCloneComplete(true);
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

