#include "subversionPlugin.h"
#include "../../qrkernel/settingsManager.h"

#include <QtGui/QApplication>

Q_EXPORT_PLUGIN2(subversion, versioning::SubversionPlugin)

using namespace versioning;
using namespace qReal::versioning;

QString const tempFolderName = "tempSvn";
int const defaultTimeout = 30000;

SubversionPlugin::SubversionPlugin()
	: mViewInteraction(new details::ViewInteraction(this))
	, mTempCheckoutDir(qApp->applicationDirPath() + "/" + tempFolderName)

{
	SettingsManager::instance()->setValue("svnTempDir", mTempCheckoutDir);
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

bool SubversionPlugin::onFileAdded(const QString &filePath, const QString &workingDir)
{
	Q_UNUSED(workingDir)
	return doAdd(filePath);
}

bool SubversionPlugin::onFileRemoved(const QString &filePath, const QString &workingDir)
{
	Q_UNUSED(workingDir)
	return doRemove(filePath);
}

bool SubversionPlugin::onFileChanged(const QString &filePath, const QString &workingDir)
{
	// Subversion detects modifications itself
	Q_UNUSED(filePath)
	Q_UNUSED(workingDir)
	return true;
}

bool SubversionPlugin::downloadWorkingCopy(QString const &repoAddress
		, QString const &targetProject
		, int revisionNumber)
{
	Q_UNUSED(revisionNumber)
	//TODO: get revision
	return doCheckout(repoAddress, targetProject, tempFolder());
}

bool SubversionPlugin::updateWorkingCopy()
{
	return doUpdate(tempFolder());
}

bool SubversionPlugin::submitChanges(const QString &description)
{
	return doCommit(tempFolder(), description);
}

bool SubversionPlugin::reinitWorkingCopy()
{
	return doCleanUp(tempFolder());
}

QString SubversionPlugin::information()
{
	return info(tempFolder());
}

int SubversionPlugin::revisionNumber()
{
	return currentRevision(tempFolder());
}

bool SubversionPlugin::isMyWorkingCopy(QString const &directory)
{
	// If svn info worked well then it is our dir
	QStringList infoArgs;
	infoArgs << "info" << (directory.isEmpty() ? tempFolder() : directory);
	return invokeOperation(infoArgs, false, false, QString(), false);
}

int SubversionPlugin::timeout() const
{
	return SettingsManager::value("svnClientTimeout", defaultTimeout).toInt();
}

QString SubversionPlugin::tempFolder() const
{
	return SettingsManager::value("svnTempDir", mTempCheckoutDir).toString();
}

bool SubversionPlugin::doCheckout(const QString &from, const QString &targetProject, const QString &targetFolder)
{
	QString checkoutDist = targetFolder.isEmpty() ? tempFolder() : targetProject;
	QStringList arguments;
	arguments << "checkout" << from << checkoutDist;
	arguments << authenticationArgs();
	bool const result = invokeOperation(arguments, false, true, targetProject);
	emit checkoutComplete(result);
	emit operationComplete("checkout", result);
	return result;
}

bool SubversionPlugin::doUpdate(const QString &to)
{
	QString targetDir = to.isEmpty() ? tempFolder() : to;
	QStringList arguments;
	arguments << "update" << targetDir;
	arguments << authenticationArgs();
	bool const result = invokeOperation(arguments);
	emit updateComplete(result);
	emit operationComplete("update", result);
	return result;
}

bool SubversionPlugin::doCommit(const QString &message, const QString &from)
{
	QString targetDir = from.isEmpty() ? tempFolder() : from;
	QStringList arguments;
	arguments << "commit" << targetDir << "-m" << message;
	arguments << authenticationArgs();
	bool const result = invokeOperation(arguments);
	emit commitComplete(result);
	emit operationComplete("commit", result);
	return result;
}

bool SubversionPlugin::doCleanUp(const QString &what)
{
	QStringList arguments;
	QString targetDir = what.isEmpty() ? tempFolder() : what;
	arguments << "cleanup" << targetDir;
	bool const result = invokeOperation(arguments);
	emit cleanUpComplete(result);
	emit operationComplete("cleanup", result);
	return result;
}

bool SubversionPlugin::doRevert(const QString &what)
{
	QStringList arguments;
	QString targetDir = what.isEmpty() ? tempFolder() : what;
	// TODO: Add different variants
	arguments << "revert" << "-R" << targetDir;
	bool const result = invokeOperation(arguments);
	emit cleanUpComplete(result);
	emit operationComplete("revert", result);
	return result;
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
	bool const result = invokeOperation(arguments);
	addComplete(result);
	operationComplete("add", result);
	return result;
}

bool SubversionPlugin::doRemove(const QString &what, bool force)
{
	QStringList arguments;
	arguments << "remove" << what;
	if (force) {
		arguments << "--force";
	}
	bool const result = invokeOperation(arguments);
	removeComplete(result);
	operationComplete("remove", result);
	return result;
}

QString SubversionPlugin::info(const QString &target)
{
	QString targetDir = target.isEmpty() ? tempFolder() : target;
	QStringList arguments;
	arguments << "info" << targetDir;
	if (!invokeOperation(arguments)) {
		return "";
	}
	return standartOutput();
}

QString SubversionPlugin::repoUrl(QString const &target)
{
	QString repoInfo = info(target);
	if (repoInfo.isEmpty()) {
		return "";
	}
	return infoToRepoUrl(repoInfo);
}

int SubversionPlugin::currentRevision(QString const &target)
{
	QString const repoInfo = info(target);
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
	// TODO: count default string end width
	repoInfo = repoInfo.mid(0,  ind - 2);
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
	// TODO: get it from AuthenticationSettingsWidget
	bool authenticationEnabled = SettingsManager::value("svnAuthenticationEnabled", false).toBool();
	if (!authenticationEnabled) {
		return result;
	}
	QString const username = SettingsManager::value("svnUsername", false).toString();
	QString const password = SettingsManager::value("svnPassword", false).toString();
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
