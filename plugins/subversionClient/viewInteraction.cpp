#include "viewInteraction.h"
#include "subversionPlugin.h"
#include "../../../qrkernel/settingsManager.h"
#include "../../../qrutils/versioningUtils/versionSelector.h"

using namespace versioning::details;
using namespace versioning::ui;

ViewInteraction::ViewInteraction(SubversionPlugin *pluginInstance)
	: mPlugin(pluginInstance),
	  mPreferencesPage(new PreferencesVersioningPage())
{
	initActions();
}

void ViewInteraction::initActions()
{
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
}

void ViewInteraction::checkoutClicked()
{
	CheckoutDialog *dialog = new CheckoutDialog(mMainWindowIface->windowWidget());
	if (QDialog::Accepted != dialog->exec()) {
		return;
	}
	// TODO: Show execution indicator
	QString target = dialog->target();
	QString url = dialog->url();

	if (mPlugin->doCheckout(url, target)) {
		// TODO: Open 'target' project
		showMessage(tr("Checkout succeeded. Working project was set to ") + target);
		SettingsManager::setValue("checkoutTarget", target);
		SettingsManager::setValue("checkoutUrl", url);
	}
	// TODO: hide execution indicator
}

void ViewInteraction::updateClicked()
{
	// TODO: Show execution indicator
	if (mPlugin->doUpdate()) {
//		mMainWindowIface->reinitModels();
		int revision = mPlugin->currentRevision();
		QString message = (revision < 0) ? tr("Updated successfully.") : tr("Updated to revision ") + QString::number(revision);
		showMessage(message);
	}
	// TODO: hide execution indicator
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

	// TODO: Show execution indicator
	if (mPlugin->doCommit(message)) {
		showMessage(tr("Commited succcessfully!"));
	}
	// TODO: hide execution indicator
}

void ViewInteraction::revertClicked()
{
	// TODO: Show execution indicator
	if (mPlugin->doRevert()) {
		showMessage(tr("Reverted successfully"));
	}
	// TODO: hide execution indicator
}

void ViewInteraction::cleanUpClicked()
{
	// TODO: Show execution indicator
	if (mPlugin->doCleanUp()) {
		showMessage(tr("Clean Up success"));
	}
	// TODO: hide execution indicator
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
