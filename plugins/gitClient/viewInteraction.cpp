#include "viewInteraction.h"
#include "../../qrkernel/settingsManager.h"
#include "../../qrutils/versioningUtils/versionSelector.h"
#include "gitPlugin.h"


using namespace versioning::details;
using namespace versioning::ui;

ViewInteraction::ViewInteraction(GitPlugin *pluginInstance)
	: mPlugin(pluginInstance)
	, mPreferencesPage(new PreferencesVersioningPage())
{
	initActions();
	connect(mPlugin, SIGNAL(initComplete(bool)), this, SLOT(onInitComplete(bool)));
	connect(mPlugin, SIGNAL(cloneComplete(bool)), this, SLOT(onCloneComplete(bool)));
	connect(mPlugin, SIGNAL(remoteComplete(bool)), this, SLOT(onRemoteComplete(bool)));
	connect(mPlugin, SIGNAL(commitComplete(bool)), this, SLOT(onCommitComplete(bool)));
	connect(mPlugin, SIGNAL(pushComplete(bool)), this, SLOT(onPushComplete(bool)));
	connect(mPlugin, SIGNAL(pullComplete(bool)), this, SLOT(onPullComplete(bool)));
	connect(mPlugin, SIGNAL(resetComplete(bool)), this, SLOT(onResetComplete(bool)));
	connect(mPlugin, SIGNAL(infoComplete(QString,bool)), this, SLOT(onInfoComplete(QString,bool)));

}

void ViewInteraction::initActions()
{
	// TODO: Add menu enabling/disabling when current project changed

	QMenu *gitMenu = new QMenu(tr("Git"));

	QAction *initAction = gitMenu->addAction(tr("Init"));
	connect(initAction, SIGNAL(triggered()), this, SLOT(initClicked()));

	QAction *cloneAction = gitMenu->addAction(tr("Clone..."));
	connect(cloneAction, SIGNAL(triggered()), this, SLOT(cloneClicked()));

	QAction *remoteAction = gitMenu->addAction(tr("Remote..."));
	connect(remoteAction, SIGNAL(triggered()), this, SLOT(remoteClicked()));

	QAction *commitAction = gitMenu->addAction(tr("Commit..."));
	connect(commitAction, SIGNAL(triggered()), this, SLOT(commitClicked()));

	QAction *pushAction = gitMenu->addAction(tr("Push..."));
	connect(pushAction, SIGNAL(triggered()), this, SLOT(pushClicked()));

	QAction *pullAction = gitMenu->addAction(tr("Pull..."));
	connect(pullAction, SIGNAL(triggered()), this, SLOT(pullClicked()));

	QAction *resetAction = gitMenu->addAction(tr("Reset..."));
	connect(resetAction, SIGNAL(triggered()), this, SLOT(resetClicked()));

	QAction *cleanAction = gitMenu->addAction(tr("Clean"));
	connect(cleanAction, SIGNAL(triggered()), this, SLOT(cleanClicked()));

	QAction *infoAction = gitMenu->addAction(tr("Info"));
	connect(infoAction, SIGNAL(triggered()), this, SLOT(infoClicked()));

	mMenu << qReal::ActionInfo(gitMenu, "tools");
}

QList<qReal::ActionInfo> ViewInteraction::actions() const
{
	return mMenu;
}

QPair<QString, PreferencesPage *> ViewInteraction::preferencesPage() const
{
	return QPair<QString, PreferencesPage *>("Git", mPreferencesPage);
}

void ViewInteraction::init(const qReal::PluginConfigurator &configurator)
{
	mMainWindowIface = &(configurator.mainWindowInterpretersInterface());
	mProjectManager = &(configurator.projectManager());
	mRepoApi = &(configurator.repoControlInterface());
}

void ViewInteraction::initClicked()
{
	mPlugin->doInit();
}

void ViewInteraction::cloneClicked()
{
	ui::CloneDialog *dialog = new ui::CloneDialog(mMainWindowIface->windowWidget());
	if (QDialog::Accepted != dialog->exec()) {
		return;
	}

	mPlugin->startClone(dialog->url());
}

void ViewInteraction::remoteClicked()
{
	ui::RemoteDialog *dialog = new ui::RemoteDialog(mMainWindowIface->windowWidget());
	if (QDialog::Accepted != dialog->exec()) {
		return;
	}

	mPlugin->doRemote(dialog->remoteName(), dialog->remoteAdress());
}

void ViewInteraction::commitClicked()
{
	ui::CommitDialog *dialog = new ui::CommitDialog(mMainWindowIface->windowWidget());
	if (QDialog::Accepted != dialog->exec()) {
		return;
	}
	QString message = dialog->message();
	if (message.isEmpty()) {
		message = "<no message>";
	}

	mPlugin->startCommit(message);
}

void ViewInteraction::pushClicked()
{
	ui::PushDialog *dialog = new ui::PushDialog(mMainWindowIface->windowWidget());
	if (QDialog::Accepted != dialog->exec()) {
		return;
	}
	//QString const target = dialog->target();
	QString const remote = dialog->url();

	//qReal::SettingsManager::setValue("pushTarget", target);
	qReal::SettingsManager::setValue("remote", remote);

	mPlugin->startPush(remote);
}

void ViewInteraction::pullClicked()
{
	ui::PullDialog *dialog = new ui::PullDialog(mMainWindowIface->windowWidget());
	if (QDialog::Accepted != dialog->exec()) {
		return;
	}

	//qReal::SettingsManager::setValue("pushUrl", dialog->url);

	mPlugin->startPull(dialog->url());
}

void ViewInteraction::resetClicked()
{
	ui::ResetDialog *dialog = new ResetDialog(mMainWindowIface->windowWidget());
	if (QDialog::Accepted != dialog->exec()) {
		return;
	}

	mPlugin->startReset(dialog->hashCommit());
}

void ViewInteraction::cleanClicked()
{
	if (mPlugin->doClean()) {
		showMessage(tr("Clean successfully."));
	}
}

void ViewInteraction::infoClicked()
{
	QString answer = mPlugin->information();
}

void ViewInteraction::showMessage(const QString &message)
{
	mMainWindowIface->errorReporter()->addInformation(message);
}

void ViewInteraction::onInitComplete(const bool success)
{
	if (success){
		//mProjectManager->openExisting(targetProject);
		showMessage(tr("Init successfully."));
	}
}

void ViewInteraction::onCloneComplete(const bool success)
{
	if (success){
		showMessage(tr("Clone successfully."));
	}
}

void ViewInteraction::onCommitComplete(const bool success)
{
	if (success) {
		showMessage(tr("Commited successfully."));
		reopenWithoutSavings();
	}
}

void ViewInteraction::onRemoteComplete(const bool success)
{
	if (success){
		showMessage(tr("Remote successfully."));
		reopenWithoutSavings();
	}
}

void ViewInteraction::onPushComplete(const bool success)
{
	if (success) {
		showMessage(tr("Pushed successfully."));
		reopenWithoutSavings();
	}
}

void ViewInteraction::onPullComplete(const bool success)
{
	if (success) {
		showMessage(tr("Pulled successfully."));
		reopenWithoutSavings();
	}
}

void ViewInteraction::onResetComplete(const bool success)
{
	if (success) {
		showMessage(tr("Reseted successfully."));
		reopenWithoutSavings();
	}
}

void ViewInteraction::onInfoComplete(QString answer, const bool success)
{
	if (!success)
		return;

	//showMessage(tr("Reseted successfully."));

	ui::InfoDialog *dialog = new ui::InfoDialog(mMainWindowIface->windowWidget());
	dialog->message(answer);
	if (QDialog::Accepted != dialog->exec()) {
		return;
	}

}

void ViewInteraction::reopenWithoutSavings()
{
	QString const currentProject = mRepoApi->workingFile();
	mProjectManager->close();
	mProjectManager->openExisting(currentProject);
}
