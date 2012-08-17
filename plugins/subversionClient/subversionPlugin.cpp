#include <QtGui/QApplication>

#include "subversionPlugin.h"
#include "../../qrkernel/settingsManager.h"
#include "../../qrutils/fileSystemUtils.h"

Q_EXPORT_PLUGIN2(subversion, versioning::SubversionPlugin)

using namespace versioning;
using namespace qReal::versioning;

QString const tempFolderName = "tempSvn";
int const defaultTimeout = 30000;

SubversionPlugin::SubversionPlugin()
	: mViewInteraction(new details::ViewInteraction(this))
	, mTempDir(qApp->applicationDirPath() + "/" + tempFolderName)
{
	SettingsManager::instance()->setValue("svnTempDir", mTempDir);
	setPathToClient(pathToSvn());
}

SubversionPlugin::~SubversionPlugin()
{
}

QString SubversionPlugin::pathToSvn() const
{
	return SettingsManager::value("pathToSvnClient", "").toString();
}

qReal::Customizer *SubversionPlugin::customizationInterface()
{
	return NULL;
}

void SubversionPlugin::updateSettings()
{
	setPathToClient(pathToSvn());
}

QList<qReal::ActionInfo> SubversionPlugin::actions()
{
	return mViewInteraction->actions();
}

void SubversionPlugin::init(qReal::PluginConfigurator const &configurator)
{
	ExternalClientPluginBase::init(configurator);
	mViewInteraction->init(configurator);
}

QPair<QString, PreferencesPage *> SubversionPlugin::preferencesPage()
{
	return mViewInteraction->preferencesPage();
}

bool SubversionPlugin::onFileAdded(QString const &filePath, QString const &workingDir)
{
	Q_UNUSED(workingDir)
	return doAdd(filePath);
}

bool SubversionPlugin::onFileRemoved(QString const &filePath, QString const &workingDir)
{
	Q_UNUSED(workingDir)
	return doRemove(filePath);
}

bool SubversionPlugin::onFileChanged(QString const &filePath, QString const &workingDir)
{
	// Subversion detects modifications itself
	Q_UNUSED(filePath)
	Q_UNUSED(workingDir)
	return true;
}

void SubversionPlugin::beginWorkingCopyDownloading(QString const &repoAddress
		, QString const &targetProject
		, int revisionNumber, bool quiet)
{
	startCheckout(repoAddress, targetProject, tempFolder(), revisionNumber, quiet);
}

void SubversionPlugin::beginWorkingCopyUpdating(QString const &targetProject)
{
	startUpdate(tempFolder(), targetProject);
}

void SubversionPlugin::beginChangesSubmitting(QString const &description, QString const &targetProject)
{
	startCommit(tempFolder(), description, targetProject);
}

bool SubversionPlugin::reinitWorkingCopy(QString const &targetProject)
{
	return doCleanUp(tempFolder(), targetProject);
}

QString SubversionPlugin::information(QString const &targetProject)
{
	return info(tempFolder(), true, targetProject);
}

int SubversionPlugin::revisionNumber(QString const &targetProject)
{
	return currentRevision(tempFolder(), false, targetProject);
}

QString SubversionPlugin::remoteRepositoryUrl(QString const &targetProject)
{
	return repoUrl(tempFolder(), false, targetProject);
}

bool SubversionPlugin::isMyWorkingCopy(QString const &directory)
{
	// If svn info worked well then it is our dir
	QStringList infoArgs;
	infoArgs << "info" << (directory.isEmpty() ? tempFolder() : directory);
	return invokeOperation(infoArgs, false, directory, false, false, QString(), QString(), false);
}

int SubversionPlugin::timeout() const
{
	return SettingsManager::value("svnClientTimeout", defaultTimeout).toInt();
}

QString SubversionPlugin::tempFolder() const
{
	return SettingsManager::value("svnTempDir", mTempDir).toString();
}

void SubversionPlugin::startCheckout(QString const &from
		, QString const &targetProject
		, QString const &targetFolder
		, int revision, bool quiet)
{
	QString checkoutDist = targetFolder.isEmpty() ? tempFolder() : targetFolder;
	utils::FileSystemUtils::removeDir(checkoutDist);
	QStringList arguments;
	arguments << "checkout" << from << checkoutDist;
	if (revision > 0) {
		arguments << "--revision";
		arguments << QString::number(revision);
	}
	arguments << authenticationArgs();
	invokeOperationAsync(arguments
			, new invocation::BoolClassMemberCallback<SubversionPlugin>(this
			, &SubversionPlugin::onCheckoutComplete, targetProject, quiet)
			, false, QString(), QString(), false, !quiet);
}

void SubversionPlugin::startUpdate(QString const &to
		, QString const &sourceProject)
{
	QString targetDir = to.isEmpty() ? tempFolder() : to;
	QStringList arguments;
	arguments << "update" << targetDir;
	arguments << authenticationArgs();
	invokeOperationAsync(arguments
		, new invocation::BoolClassMemberCallback<SubversionPlugin>(this, &SubversionPlugin::onUpdateComplete)
		, true, to, sourceProject);
}

void SubversionPlugin::startCommit(QString const &message, QString const &from
		, QString const &sourceProject)
{
	QString targetDir = from.isEmpty() ? tempFolder() : from;
	QStringList arguments;
	arguments << "commit" << targetDir << "-m" << message;
	arguments << authenticationArgs();
	invokeOperationAsync(arguments
		, new invocation::BoolClassMemberCallback<SubversionPlugin>(this, &SubversionPlugin::onCommitComplete)
		, true, from, sourceProject);
}

bool SubversionPlugin::doCleanUp(QString const &what
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

void SubversionPlugin::startRevert(QString const &what
		, QString const &sourceProject)
{
	QStringList arguments;
	QString targetDir = what.isEmpty() ? tempFolder() : what;
	// TODO: Add different variants
	arguments << "revert" << "-R" << targetDir;
	invokeOperationAsync(arguments
		, new invocation::BoolClassMemberCallback<SubversionPlugin>(this, &SubversionPlugin::onRevertComplete)
		, true, targetDir, sourceProject, true, true);
}

void SubversionPlugin::onCheckoutComplete(bool const result, QString const &targetProject, bool const quiet)
{
	processWorkingCopy(targetProject);
	emit checkoutComplete(result, targetProject, quiet);
	emit operationComplete("checkout", result);
	emit workingCopyDownloaded(result, targetProject);
}

void SubversionPlugin::onUpdateComplete(bool const result)
{
	processWorkingCopy();
	emit updateComplete(result);
	emit operationComplete("update", result);
	emit workingCopyUpdated(result);
}

void SubversionPlugin::onCommitComplete(bool const result)
{
	processWorkingCopy();
	emit commitComplete(result);
	emit operationComplete("commit", result);
	emit changesSubmitted(result);
}

void SubversionPlugin::onRevertComplete(bool const result)
{
	processWorkingCopy();
	emit cleanUpComplete(result);
	emit operationComplete("revert", result);
}

bool SubversionPlugin::doAdd(const QString &what, bool force)
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

bool SubversionPlugin::doRemove(QString const &what, bool force)
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

QString SubversionPlugin::info(QString const &target, bool const reportErrors
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

QString SubversionPlugin::repoUrl(QString const &target, bool const reportErrors
		, QString const &sourceProject)
{
	QString repoInfo = info(target, reportErrors, sourceProject);
	if (repoInfo.isEmpty()) {
		return "";
	}
	return infoToRepoUrl(repoInfo);
}

int SubversionPlugin::currentRevision(QString const &target, bool const reportErrors
		, QString const &sourceProject)
{
	QString const repoInfo = info(target, reportErrors, sourceProject);
	if (repoInfo.isEmpty()) {
		return -1;
	}
	return infoToRevision(repoInfo);
}

QString SubversionPlugin::infoToRepoUrl(QString &repoInfo)
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

int SubversionPlugin::infoToRevision(QString const &repoInfo)
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

QStringList SubversionPlugin::authenticationArgs() const
{
	QStringList result;

	QString const enabledKey = ui::AuthenticationSettingsWidget::enabledSettingsName("svn");
	QString const usernameKey = ui::AuthenticationSettingsWidget::usernameSettingsName("svn");
	QString const passwordKey = ui::AuthenticationSettingsWidget::passwordSettingsName("svn");

	bool const authenticationEnabled = SettingsManager::value(enabledKey, false).toBool();
	if (!authenticationEnabled) {
		return result;
	}
	QString const username = SettingsManager::value(usernameKey, false).toString();
	QString const password = SettingsManager::value(passwordKey, false).toString();
	if (username.isEmpty()) {
		return result;
	}
	result << "--username";
	result << username;
	result << "--password";
	// TODO: make it more sequre
	result << password;
	result << "--non-interactive";
	// TODO: accept SSL certs from gui window, like in tortoize
	result << "--trust-server-cert";
	// TODO: add caching into settings
	result << "--no-auth-cache";
	return result;
}

void SubversionPlugin::editProxyConfiguration()
{
	// TODO: add proxy settings into settings page;
	//   teach to edit config file to set proxy
}
