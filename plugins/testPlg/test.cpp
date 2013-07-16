#include <QtWidgets/QApplication>

#include "test.h"
#include "../../qrkernel/settingsManager.h"
#include "../../qrutils/fileSystemUtils.h"
#include <QDebug>

using namespace versioning;
using namespace qReal::versioning;


QString const tempFolderName = "tempSvn";
int const defaultTimeout = 30000;

GitPlugin::GitPlugin():
	mTempDir(qApp->applicationDirPath() + "/" + tempFolderName)
{
	qReal::SettingsManager::instance()->setValue("svnTempDir", mTempDir);
	setPathToClient(pathToGit());
}

GitPlugin::~GitPlugin()
{
}

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
	return QList<qReal::ActionInfo>();
}

void GitPlugin::init(qReal::PluginConfigurator const &configurator)
{
	ExternalClientPluginBase::init(configurator);
}

QPair<QString, PreferencesPage *> GitPlugin::preferencesPage()
{
	return QPair<QString, PreferencesPage *>();
}

bool GitPlugin::onFileAdded(QString const &filePath, QString const &workingDir)
{
	Q_UNUSED(workingDir)
	return doAdd(filePath);
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
	startCheckout(repoAddress, targetProject, tempFolder(), revisionNumber, quiet);
}

void GitPlugin::beginWorkingCopyUpdating(QString const &targetProject)
{
	startUpdate(tempFolder(), targetProject);
}

void GitPlugin::beginChangesSubmitting(QString const &description, QString const &targetProject)
{
	startCommit(tempFolder(), description, targetProject);
}

bool GitPlugin::reinitWorkingCopy(QString const &targetProject)
{
	return doCleanUp(tempFolder(), targetProject);
}

QString GitPlugin::information(QString const &targetProject)
{
	return info(tempFolder(), true, targetProject);
}

int GitPlugin::revisionNumber(QString const &targetProject)
{
	return currentRevision(tempFolder(), false, targetProject);
}

QString GitPlugin::remoteRepositoryUrl(QString const &targetProject)
{
	return repoUrl(tempFolder(), false, targetProject);
}

bool GitPlugin::isMyWorkingCopy(QString const &directory)
{
	// If svn info worked well then it is our dir
	QStringList infoArgs;
	infoArgs << "info" << (directory.isEmpty() ? tempFolder() : directory);
	return invokeOperation(infoArgs, false, directory, false, false, QString(), QString(), false);
}

QString GitPlugin::friendlyName()
{
	return "Git Plugin";
}

int GitPlugin::timeout() const
{
	return qReal::SettingsManager::value("svnClientTimeout", defaultTimeout).toInt();
}

QString GitPlugin::tempFolder() const
{
	return qReal::SettingsManager::value("svnTempDir", mTempDir).toString();
}

void GitPlugin::startCheckout(QString const &from
		, QString const &targetProject
		, QString const &targetFolder
		, int revision, bool quiet)
{
	QString checkoutDist = targetFolder.isEmpty() ? tempFolder() : targetFolder;
	qReal::FileSystemUtils::removeDir(checkoutDist);
	QStringList arguments;
	arguments << "checkout" << from << checkoutDist;
	if (revision > 0) {
		arguments << "--revision";
		arguments << QString::number(revision);
	}
	arguments << authenticationArgs();
	invokeOperationAsync(arguments
			, new invocation::BoolClassMemberCallback<GitPlugin>(this
			, &GitPlugin::onCheckoutComplete, targetProject, quiet)
			, false, QString(), QString(), false, !quiet);
}

void GitPlugin::startUpdate(QString const &to
		, QString const &sourceProject)
{
	QString targetDir = to.isEmpty() ? tempFolder() : to;
	QStringList arguments;
	arguments << "update" << targetDir;
	arguments << authenticationArgs();
	invokeOperationAsync(arguments
		, new invocation::BoolClassMemberCallback<GitPlugin>(this, &GitPlugin::onUpdateComplete)
		, true, to, sourceProject);
}

void GitPlugin::startCommit(QString const &message, QString const &from
		, QString const &sourceProject)
{
	QString targetDir = from.isEmpty() ? tempFolder() : from;
	QStringList arguments;
	arguments << "commit" << targetDir << "-m" << message;
	arguments << authenticationArgs();
	invokeOperationAsync(arguments
		, new invocation::BoolClassMemberCallback<GitPlugin>(this, &GitPlugin::onCommitComplete)
		, true, from, sourceProject);
}

bool GitPlugin::doCleanUp(QString const &what
		, QString const &sourceProject)
{
	QStringList arguments;
	QString targetDir = what.isEmpty() ? tempFolder() : what;
	arguments << "cleanup" << targetDir;
	bool const result = invokeOperation(arguments, true, what, true, true, QString(), sourceProject);
	emit cleanUpComplete(result);
	emit operationComplete("cleanup", result);
	return result;
}

void GitPlugin::startRevert(QString const &what
		, QString const &sourceProject)
{
	QStringList arguments;
	QString targetDir = what.isEmpty() ? tempFolder() : what;
	// TODO: Add different variants
	arguments << "revert" << "-R" << targetDir;
	invokeOperationAsync(arguments
		, new invocation::BoolClassMemberCallback<GitPlugin>(this, &GitPlugin::onRevertComplete)
		, true, targetDir, sourceProject, true, true);
}

void GitPlugin::onCheckoutComplete(bool const result, QString const &targetProject, bool const quiet)
{
	processWorkingCopy(targetProject);
	emit checkoutComplete(result, targetProject, quiet);
	emit operationComplete("checkout", result);
	emit workingCopyDownloaded(result, targetProject);
}

void GitPlugin::onUpdateComplete(bool const result)
{
	processWorkingCopy();
	emit updateComplete(result);
	emit operationComplete("update", result);
	emit workingCopyUpdated(result);
}

void GitPlugin::onCommitComplete(bool const result)
{
	processWorkingCopy();
	emit commitComplete(result);
	emit operationComplete("commit", result);
	emit changesSubmitted(result);
}

void GitPlugin::onRevertComplete(bool const result)
{
	processWorkingCopy();
	emit cleanUpComplete(result);
	emit operationComplete("revert", result);
}

bool GitPlugin::doAdd(const QString &what, bool force)
{
	QStringList arguments;
	arguments << "add" << what;
	if (force) {
		arguments << "--force";
	}
	// This feature requires svn with version >= 1.5
	arguments << "--parents";
	bool const result = invokeOperation(arguments, false, QString(), false, false, QString(), QString(), true);
	addComplete(result);
	operationComplete("add", result);
	return result;
}

bool GitPlugin::doRemove(QString const &what, bool force)
{
	QStringList arguments;
	arguments << "remove" << what;
	if (force) {
		arguments << "--force";
	}
	bool const result = invokeOperation(arguments, false, QString(), false, false, QString(), QString(), true);
	removeComplete(result);
	operationComplete("remove", result);
	return result;
}

QString GitPlugin::info(QString const &target, bool const reportErrors
		, QString const &sourceProject)
{
	QString targetDir = target.isEmpty() ? tempFolder() : target;
	QStringList arguments;
	arguments << "info" << targetDir;
	if (!invokeOperation(arguments, true, QString(), false, false, QString(), sourceProject, reportErrors)) {
		return "";
	}
	return standartOutput();
}

QString GitPlugin::repoUrl(QString const &target, bool const reportErrors
		, QString const &sourceProject)
{
	QString repoInfo = info(target, reportErrors, sourceProject);
	if (repoInfo.isEmpty()) {
		return "";
	}
	return infoToRepoUrl(repoInfo);
}

int GitPlugin::currentRevision(QString const &target, bool const reportErrors
		, QString const &sourceProject)
{
	QString const repoInfo = info(target, reportErrors, sourceProject);
	if (repoInfo.isEmpty()) {
		return -1;
	}
	return infoToRevision(repoInfo);
}

QString GitPlugin::infoToRepoUrl(QString &repoInfo)
{
	//TODO: make it ok
	int ind = repoInfo.indexOf("Repository Root: ");
	if (ind == -1) {
		onErrorOccured(tr("Can`t find repository root in svn info"));
		return "";
	}
	repoInfo = repoInfo.mid(ind + QString("Repository Root: ").length());
	ind = repoInfo.indexOf("Repository UUID: ");
	if (ind == -1) {
		onErrorOccured(tr("Can`t find repository UUID in svn info"));
		return "";
	}
	int lineSeparatorLength = 1;
	#ifdef Q_OS_WIN // TODO: Change it to something more flexible
	lineSeparatorLength = 2;
	#endif
	repoInfo = repoInfo.mid(0, ind - lineSeparatorLength);
	return repoInfo;
}

int GitPlugin::infoToRevision(QString const &repoInfo)
{
	// TODO: make it ok
	int ind = repoInfo.indexOf("Revision: ");
	if (ind == -1) {
		onErrorOccured(tr("Can`t find revision number in svn info"));
		return -1;
	}
	ind += QString("Revision: ").length();
	QString revision = "";
	for (; repoInfo[ind] < '9' && repoInfo[ind] > '0'; ++ind) {
		revision += repoInfo[ind];
	}
	return revision.toInt();
}

QStringList GitPlugin::authenticationArgs() const
{
	QStringList result;
	return result;
}

void GitPlugin::editProxyConfiguration()
{
	// TODO: add proxy settings into settings page;
	//   teach to edit config file to set proxy
}


void GitPlugin::setVersion(QString hash)
{
	qDebug() << "switch to version " << hash << "ok";
}

QString GitPlugin::getLog(QString format)
{
	QString testLog = "7941425c938e7c7ce7f8c23a9a034be34edd6486 - ZiminGrigory, Thu Jul 11 15:26:02 2013 +0400 : fix bug ctrl+c, add easyVersioning architecture";

	testLog += "\nc16cea4f66e1a9b3e6ba8b3b4242175a29dfab68 - Dmitry Mordvinov, Wed Jul 3 20:59:50 2013 +0400 : Corrected path in previous commit";

	return testLog;
}
