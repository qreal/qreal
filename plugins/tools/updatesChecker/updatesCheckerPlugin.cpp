#include "updatesCheckerPlugin.h"

#include <QtWidgets/QApplication>

#include <qrkernel/logging.h>

#include "updater.h"
#include "updateVersionDialog.h"

using namespace updatesChecker;

UpdatesCheckerPlugin::UpdatesCheckerPlugin()
	: mSeparator(nullptr)
	, mCheckForUpdatesAction(tr("Check for updates"), nullptr)
{
	mSeparator.setSeparator(true);
	connect(&mCheckForUpdatesAction, &QAction::triggered, this, &UpdatesCheckerPlugin::checkForUpdates);
}

QList<qReal::ActionInfo> UpdatesCheckerPlugin::actions()
{
	return { ActionInfo(&mSeparator, "", "help")
		, ActionInfo(&mCheckForUpdatesAction, "", "help") };
}

void UpdatesCheckerPlugin::init(qReal::PluginConfigurator const &configurator)
{
	mErrorReporter = configurator.mainWindowInterpretersInterface().errorReporter();
	mMainWindowWidget = configurator.mainWindowInterpretersInterface().windowWidget();
	checkForUpdates(false);
}

QStringList UpdatesCheckerPlugin::defaultSettingsFiles()
{
	return { ":/updatesCheckerDefaultSettings.ini" };
}

void UpdatesCheckerPlugin::checkForUpdates(bool reportNoUpdates)
{
	if (SettingsManager::value("updaterActive").toBool()) {
		Updater * const updater = new Updater(this);
		connect(updater, &Updater::newVersionAvailable, this, &UpdatesCheckerPlugin::showUpdatesDialog);
		if (reportNoUpdates) {
			connect(updater, &Updater::noNewVersionAvailable, this, &UpdatesCheckerPlugin::reportNoUpdates);
		}

		/// @todo: Commented out till server unavailability error will be fixed
		QLOG_INFO() << "Starting updater...";
		updater->checkForNewVersion();
	}
}

void UpdatesCheckerPlugin::reportNoUpdates()
{
	mErrorReporter->addInformation(tr("No updates available"));
}

void UpdatesCheckerPlugin::showUpdatesDialog()
{
	QLOG_INFO() << "New updates found!";
	Updater * const updater = dynamic_cast<Updater *>(sender());
	if (updater && UpdateVersionDialog::promptUpdate(mMainWindowWidget)) {
		updater->start();

		QLOG_INFO() << "Shutting down to install it";
		QApplication::quit();
	}
}
