#include "viewInteraction.h"
#include "subversionPlugin.h"
#include "../../qrkernel/settingsManager.h"
#include "../../qrutils/versioningUtils/versionSelector.h"

using namespace versioning::details;
using namespace versioning::ui;

ViewInteraction::ViewInteraction(SubversionPlugin *pluginInstance)
	: mPlugin(pluginInstance)
	, mPreferencesPage(new PreferencesVersioningPage())
{
	initActions();
	connect(mPlugin, SIGNAL(checkoutComplete(bool,QString))
			, this, SLOT(onCheckoutComplete(bool,QString)));
	connect(mPlugin, SIGNAL(updateComplete(bool))
			, this, SLOT(onUpdateComplete(bool)));
	connect(mPlugin, SIGNAL(commitComplete(bool))
			, this, SLOT(onCommitComplete(bool)));
	connect(mPlugin, SIGNAL(revertComplete(bool))
			, this, SLOT(onRevertComplete(bool)));
}

void ViewInteraction::initActions()
{
	// TODO: Add menu enabling/disabling when current project changed

	QMenu *svnMenu = new QMenu(tr("Subversion"));

	QAction *checkoutAction = svnMenu->addAction(tr("Checkout..."));
	connect(checkoutAction, SIGNAL(triggered()), this, SLOT(checkoutClicked()));

	QAction *updateAction = svnMenu->addAction(tr("Update"));
	connect(updateAction, SIGNAL(triggered()), this, SLOT(updateClicked()));

	QAction *commitAction = svnMenu->addAction(tr("Commit..."));
	connect(commitAction, SIGNAL(triggered()), this, SLOT(commitClicked()));

	QAction *revertAction = svnMenu->addAction(tr("Revert"));
	connect(revertAction, SIGNAL(triggered()), this, SLOT(revertClicked()));

	QAction *cleanUpAction = svnMenu->addAction(tr("Clean up"));
	connect(cleanUpAction, SIGNAL(triggered()), this, SLOT(cleanUpClicked()));

	QAction *infoAction = svnMenu->addAction(tr("Info"));
	connect(infoAction, SIGNAL(triggered()), this, SLOT(infoClicked()));

	mMenu << qReal::ActionInfo(svnMenu, "tools");
}

QList<qReal::ActionInfo> ViewInteraction::actions() const
{
	return mMenu;
}

QPair<QString, PreferencesPage *> ViewInteraction::preferencesPage() const
{
	return QPair<QString, PreferencesPage *>("Subversion", mPreferencesPage);
}

void ViewInteraction::init(const qReal::PluginConfigurator &configurator)
{
	mMainWindowIface = &(configurator.mainWindowInterpretersInterface());
	mProjectManager = &(configurator.projectManager());
	mRepoApi = &(configurator.repoControlInterface());
}

void ViewInteraction::checkoutClicked()
{
	CheckoutDialog *dialog = new CheckoutDialog(mMainWindowIface->windowWidget());
	if (QDialog::Accepted != dialog->exec()) {
		return;
	}
	QString const target = dialog->target();
	QString const url = dialog->url();

	SettingsManager::setValue("checkoutTarget", target);
	SettingsManager::setValue("checkoutUrl", url);

	mPlugin->startCheckout(url, target);
}

void ViewInteraction::updateClicked()
{
	mPlugin->startUpdate();
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

void ViewInteraction::revertClicked()
{
	mPlugin->startRevert();
}

void ViewInteraction::cleanUpClicked()
{
	if (mPlugin->doCleanUp()) {
		showMessage(tr("Clean Up success"));
	}
}

void ViewInteraction::infoClicked()
{
	QString info = mPlugin->info();
	if (!info.isEmpty()) {
		showMessage(info);
	}
}

void ViewInteraction::showMessage(const QString &message)
{
	mMainWindowIface->errorReporter()->addInformation(message);
}

void ViewInteraction::onCheckoutComplete(const bool success, const QString &targetProject)
{
	if (success) {
		mProjectManager->openExisting(targetProject);
		showMessage(tr("Checkout succeeded. Working project was set to ") + targetProject);
	}
}

void ViewInteraction::onUpdateComplete(const bool success)
{
	if (success) {
		int revision = mPlugin->currentRevision();
		QString const message = (revision < 0)
				? tr("Updated successfully")
				: tr("Updated to revision ") + QString::number(revision);
		showMessage(message);
		reopenWithoutSavings();
	}
}

void ViewInteraction::onCommitComplete(const bool success)
{
	if (success) {
		showMessage(tr("Commited successfully"));
		reopenWithoutSavings();
	}
}

void ViewInteraction::onRevertComplete(const bool success)
{
	if (success) {
		showMessage(tr("Reverted successfully"));
		reopenWithoutSavings();
	}
}

void ViewInteraction::reopenWithoutSavings()
{
//	QString const currentProject = mRepoApi->workingFile();
	//TODO: obtain current project name
	mProjectManager->close();
	mProjectManager->openExisting(currentProject);
}
