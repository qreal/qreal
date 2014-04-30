#include <QtWidgets/QApplication>

#include <QtCore/QRegExp>
#include "gitPlugin.h"
#include "../../qrkernel/settingsManager.h"
#include "../../qrutils/fileSystemUtils.h"
#include "tags.h"

using namespace git;
using namespace qReal::versioning;

QString const tempFolderName = "tempGit";
int const defaultTimeout = 30000;

GitPlugin::GitPlugin()
	: mViewInteraction(new details::ViewInteraction(this))
	, mTempDir(tempFolderName)
{
	qReal::SettingsManager::instance()->setValue("gitTempDir", mTempDir);
	setPathToClient(pathToGit());

	connect(this, SIGNAL(operationIsFinished(QVariant)), SLOT(doAfterOperationIsFinished(QVariant)));
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
		, QString revisionNumber, bool quiet)
{
	if (repoAddress == "" || targetProject == "" || revisionNumber == ""){
		doInit(tempFolder(), quiet);
	} else {
		if(invokeOperation(QStringList(), true, tempFolderName, true, false, QString(), QString(), false)){
			QStringList args;
			args << "reset" << "--hard" << revisionNumber;
			bool result = invokeOperation(args, false, tempFolderName, false, true, targetProject, QString(), false);
			emit workingCopyDownloaded(result, targetProject);
		}
	}
}

void GitPlugin::beginWorkingCopyUpdating(QString const &targetProject)
{
	startPush(tempFolder(), targetProject);
}

void GitPlugin::beginChangesSubmitting(QString const &description, QString const &targetProject, const bool &quiet)
{
	startCommit(description, tempFolder(), targetProject, quiet);
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
	return qReal::SettingsManager::value("remoteAdress", tempFolderName).toString();
}

bool GitPlugin::isMyWorkingCopy(QString const &directory, const bool &quiet, bool const &prepareAndProcess)
{
	QStringList infoArgs;
	infoArgs << "remote" << "show";
	return invokeOperation(infoArgs, prepareAndProcess, directory, false, prepareAndProcess, QString(), QString(), quiet);
}

int GitPlugin::timeout() const
{
	return qReal::SettingsManager::value("gitClientTimeout", defaultTimeout).toInt();
}

QString GitPlugin::tempFolder() const
{
	return qReal::SettingsManager::value("gitTempDir", mTempDir).toString();
}

void GitPlugin::checkClientInstalling()
{
	emit clientInstalled(friendlyName(), clientExist());
}

QString GitPlugin::friendlyName()
{
	return "Git Plugin";
}

void GitPlugin::setVersion(QString hash, bool const &quiet)
{
	//because if we have unsaved changes, it would be a crash
	if (!this->isMyWorkingCopy(QString(), false, true)){
		this->beginWorkingCopyDownloading(QString(), QString(), QString(), true);
	}
	this->beginChangesSubmitting("version was saved in a transparent mode", QString(), quiet);

	//some magic with git branches
	QStringList args;
	args << "branch" << "tmp";
	invokeOperation(args, true, QString(), false, false, QString(), QString(), !quiet);

	args.clear();
	args << "checkout" << "tmp";
	invokeOperation(args, false, QString(), false, false, QString(), QString(), !quiet);

	args.clear();
	args << "reset" << "--hard" << hash;
	invokeOperation(args, false, QString(), false, false, QString(), QString(), !quiet);

	args.clear();
	args << "checkout" << "master";
	invokeOperation(args, false, QString(), false, false, QString(), QString(), !quiet);

	args.clear();
	args << "cherry-pick" << "tmp" << "--allow-empty" << "--keep-redundant-commits" << "--strategy=ours";
	invokeOperation(args, false, QString(), false, false, QString(), QString(), !quiet);

	args.clear();
	args << "branch" << "branchTEMP";
	invokeOperation(args, false, QString(), false, false, QString(), QString(), !quiet);

	args.clear();
	args << "reset" << "--hard" << "tmp";
	invokeOperation(args, false, QString(), false, false, QString(), QString(), !quiet);

	args.clear();
	args << "reset" << "--soft" << "branchTEMP";
	invokeOperation(args, false, QString(), false, false, QString(), QString(), quiet);

	args.clear();
	args << "commit" << "--amend" << "-m" << "version was saved in a transparent mode";
	invokeOperation(args, false, QString(), false, false, QString(), QString(), !quiet);

	args.clear();
	args << "branch" << "-D" << "branchTEMP";
	invokeOperation(args, false, QString(), false, false, QString(), QString(), !quiet);

	args.clear();
	args << "branch" << "-D" << "tmp";
	invokeOperation(args, false, QString(), false, true, QString(), QString(), !quiet);
}

bool GitPlugin::clientExist()
{
	QProcess *process = new QProcess;
	process->start(pathToGit(), QStringList() << "--version");
	process->waitForFinished();
	bool res = process->readAllStandardOutput().startsWith(QString("git").toLocal8Bit());
	qReal::SettingsManager::setValue("gitClientExist", res);
	delete process;
	return res;
}

QString GitPlugin::getLog(QString const &format, bool const &quiet)
{
	return doLog(format, quiet);
}

void GitPlugin::doInit(QString const &targetFolder, bool const &quiet)
{
	QStringList arguments;
	arguments  << "init";
	bool result = invokeOperation(arguments, true, QString(), false, true, QString(), QString(), !quiet);
	if (!quiet){
		emit initComplete(result);
	}
	arguments.clear();
	arguments << "add" << "-A";
	invokeOperation(arguments, true, QString(), true, true, QString(), QString(), !quiet);
	doUserEmailConfig();
	doUserNameConfig();
}

void GitPlugin::startClone(QString const &from
		, QString const &targetFolder)
{
	QStringList arguments;
	arguments << "clone" << from << targetFolder;

	const Tag tagStruct("clone");
	QVariant tagVariant;
	tagVariant.setValue(tagStruct);
	invokeOperationAsync(arguments, tagVariant, false, "", QString(), false);
}

void GitPlugin::startCommit(QString const &message, QString const &from
		, QString const &sourceProject, const bool &quiet)
{
	QStringList arguments;
	arguments << "add" << "-A";
	invokeOperation(arguments, true, QString(), true, true, QString(), QString(), !quiet);

	arguments.clear();
	arguments << "commit" << "-m" << message;

	bool result = invokeOperation(arguments, true, QString(), true, true, QString(), QString(), !quiet);
	if (!quiet){
		emit commitComplete(result);
		emit operationComplete("commit", result);
	}
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

void GitPlugin::startReset(QString const &hash, QString const &targetFolder, const bool &quiet)
{
	QString targetDir = targetFolder.isEmpty() ? tempFolder() : targetFolder;
	QStringList arguments;
	// TODO: Add different variants
	arguments << "reset" << hash << "--hard";

	const Tag tagStruct("reset", QString(), quiet);
	QVariant tagVariant;
	tagVariant.setValue(tagStruct);
	invokeOperationAsync(arguments, tagVariant, true, targetDir, QString(), true, !quiet);
}

void GitPlugin::doUserNameConfig()
{
	QString const enabledKey = qReal::versioning::ui::AuthenticationSettingsWidget::enabledSettingsName("git");
	QString const usernameKey = qReal::versioning::ui::AuthenticationSettingsWidget::usernameSettingsName("git");
	QString username = "qReal";

	bool const authenticationEnabled = qReal::SettingsManager::value(enabledKey, false).toBool();
	if (authenticationEnabled
		&& !qReal::SettingsManager::value(usernameKey, false).toString().isEmpty()){
			username = qReal::SettingsManager::value(usernameKey, false).toString();
		}

	QStringList arguments;
	arguments << "config" << "user.name" << "\"" + username + "\"";
	bool const result = invokeOperation(arguments, false, QString(), true, true, QString(), QString(), true);
	emit operationComplete("userName", result);
}


void GitPlugin::doUserEmailConfig()
{
	QString const enabledKey = qReal::versioning::ui::AuthenticationSettingsWidget::enabledSettingsName("git");
	QString const emailKey = qReal::versioning::ui::AuthenticationSettingsWidget::usernameSettingsName("git");
	QString email = "qReal@qrealmail.com";

	bool const authenticationEnabled = qReal::SettingsManager::value(enabledKey, false).toBool();
	if (authenticationEnabled
		&& !qReal::SettingsManager::value(emailKey, false).toString().isEmpty()){
			email = qReal::SettingsManager::value(emailKey, false).toString();
		}

	QStringList arguments;
	arguments << "config" << "user.email" << email;
	bool const result = invokeOperation(arguments, true, QString(), true, false, QString(), QString(), true);
	emit operationComplete("userEmail", result);
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
	arguments << "rm" << what << "-r";

	if (force) {
		arguments.append("-f");
	}

	QString path = what;
	path = getFilePath(path);

	bool const result = invokeOperation(arguments, false, path, false, false, QString(), QString(), true);
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

QString GitPlugin::doLog(QString const &format, const bool &quiet, bool const &showDialog)
{
	QStringList arguments;
	arguments << "log";
	if (format.size() != 0) {
		arguments << format;
	}

	bool result = invokeOperation(arguments, true, QString(), true, true, QString(), QString(), quiet);

	QString answer = standartOutput();
	if (showDialog){
		emit logComplete(answer, result);
	}
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
	if (tagStruct.operation == "clone"){
		onCloneComplete(true, tagStruct.boolTag);
	} else if (tagStruct.operation == "push"){
		onPushComplete(true);
	} else if (tagStruct.operation == "pull"){
		onPullComplete(true);
	} else if (tagStruct.operation == "reset"){
		onResetComplete(true, tagStruct.boolTag);
	}
}

QString GitPlugin::getUsername()
{
	QString const usernameKey = qReal::versioning::ui::AuthenticationSettingsWidget::usernameSettingsName("git");
	QString const username = qReal::SettingsManager::value(usernameKey, false).toString();
	return username;
}

QString GitPlugin::getPassword()
{
	QString const passwordKey = qReal::versioning::ui::AuthenticationSettingsWidget::passwordSettingsName("git");
	QString const password = qReal::SettingsManager::value(passwordKey, false).toString();
	return password;
}

//to do: made it correct
void GitPlugin::onCloneComplete(bool const result, const bool quiet)
{
	processWorkingCopy();
	if (!quiet) {
		emit cloneComplete(result);
		emit operationComplete("clone", result);
	}
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

void GitPlugin::onResetComplete(const bool result, const bool quiet)
{
	processWorkingCopy();
	if (!quiet) {
		emit resetComplete(result);
		emit operationComplete("reset", result);
	}
	emit workingCopyUpdated(result);
}

