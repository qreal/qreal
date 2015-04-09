#include "updatesCheckerPlugin.h"

#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QCheckBox>

#include <qrkernel/settingsManager.h>
#include <qrkernel/logging.h>

#include "updater.h"
#include "updateVersionDialog.h"

using namespace updatesChecker;

UpdatesCheckerPlugin::UpdatesCheckerPlugin()
	: mSeparator(new QAction(nullptr))
	, mCheckForUpdatesAction(new QAction(tr("Check for updates"), nullptr))
{
	mSeparator->setSeparator(true);
	connect(mCheckForUpdatesAction, &QAction::triggered, this, &UpdatesCheckerPlugin::checkForUpdates);
}

QList<qReal::ActionInfo> UpdatesCheckerPlugin::actions()
{
	return { qReal::ActionInfo(mSeparator, "", "help")
		, qReal::ActionInfo(mCheckForUpdatesAction, "", "help") };
}

void UpdatesCheckerPlugin::init(const qReal::PluginConfigurator &configurator)
{
	mErrorReporter = configurator.mainWindowInterpretersInterface().errorReporter();
	mMainWindowWidget = configurator.mainWindowInterpretersInterface().windowWidget();
	initSettingsUi(*configurator.mainWindowInterpretersInterface().preferencesPages()["preferencesBehaviourPage"]);
	checkForUpdates(false);
}

QStringList UpdatesCheckerPlugin::defaultSettingsFiles()
{
	return { ":/updatesCheckerDefaultSettings.ini" };
}

void UpdatesCheckerPlugin::checkForUpdates(bool reportNoUpdates)
{
	if (qReal::SettingsManager::value("updaterActive").toBool()) {
		Updater * const updater = new Updater(this);
		connect(updater, &Updater::newVersionAvailable, this, &UpdatesCheckerPlugin::showUpdatesDialog);
		if (reportNoUpdates) {
			connect(updater, &Updater::noNewVersionAvailable, this, &UpdatesCheckerPlugin::reportNoUpdates);
		}

		QLOG_INFO() << "Starting updater...";
		updater->checkForNewVersion();
	}
}

void UpdatesCheckerPlugin::reportNoUpdates()
{
	mErrorReporter->addInformation(tr("No updates available"));
}

void UpdatesCheckerPlugin::initSettingsUi(qReal::gui::PreferencesPage &behaviourPage)
{
	QGridLayout * const automaticsLayout = behaviourPage.findChild<QGridLayout *>("automaticsFrameLayout");
	if (!automaticsLayout) {
		QLOG_ERROR() << "Could not find 'automaticsFrameLayout' on preferences behaviour page";
		return;
	}

	QCheckBox * const box = new QCheckBox(tr("Check for updates on start"), automaticsLayout->widget());
	automaticsLayout->addWidget(box, automaticsLayout->rowCount(), 0, 1, automaticsLayout->columnCount());
	connect(&behaviourPage, &qReal::gui::PreferencesPage::saved, [box]() {
		qReal::SettingsManager::setValue("updaterActive", box->isChecked());
	});
	connect(&behaviourPage, &qReal::gui::PreferencesPage::restored, [box]() {
		box->setChecked(qReal::SettingsManager::value("updaterActive").toBool());
	});
}

void UpdatesCheckerPlugin::showUpdatesDialog()
{
	QLOG_INFO() << "New updates found!";
	Updater * const updater = dynamic_cast<Updater *>(sender());
	if (updater && UpdateVersionDialog::promptUpdate(mMainWindowWidget)) {
		updater->start();

		QLOG_INFO() << "Shutting down to install it";
		QApplication::closeAllWindows();
		QApplication::quit();
	}
}
