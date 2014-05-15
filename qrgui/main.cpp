#include <QtCore/QtPlugin>
#include <QtWidgets/QApplication>

#include "mainwindow/mainWindow.h"
#include "thirdparty/windowsmodernstyle.h"
#include <qrutils/uxInfo/uxInfo.h>

#include "qrealApplication.h"

using namespace qReal;

void clearConfig()
{
	SettingsManager::clearSettings();
	SettingsManager::instance()->saveData();
}

void setDefaultLocale(bool localizationDisabled)
{
	// If some day it will be requested the language customization from application settings
	// this method can be useful. We can just set default locale to the selected in settings one
	// (not the system locale like now).
	if (localizationDisabled) {
		QLocale::setDefault(QLocale::English);
	}

	// System locale is used when no default locale was set.
	// Here prefered locale can be set some day.
}

int main(int argc, char *argv[])
{
	QDateTime const startedTime = QDateTime::currentDateTime();
	QRealApplication app(argc, argv);

	setDefaultLocale(app.arguments().contains("--no-locale"));

	QTranslator guiTranslator;
	QTranslator utilsTranslator;
	QTranslator qtTranslator;
	guiTranslator.load(":/qrgui_" + QLocale().name());
	utilsTranslator.load(":/qrutils_" + QLocale().name());
	qtTranslator.load(":/qt_" + QLocale().name());
	app.installTranslator(&guiTranslator);
	app.installTranslator(&utilsTranslator);
	app.installTranslator(&qtTranslator);

	QString fileToOpen;
	if (app.arguments().count() > 1) {
		if (app.arguments().contains("--clear-conf")) {
			clearConfig();
			return 0;
		} else {
			int const setIndex = app.arguments().indexOf("--config");
			if (setIndex > -1) {
				QString const settingsFileName = app.arguments().at(setIndex + 1);
				SettingsManager::instance()->loadSettings(settingsFileName);
			}
			for (int i = 0; i < argc; i++) {
				if (app.arguments().at(i).endsWith(".qrs")) {
					fileToOpen = app.arguments().at(i);
					break;
				}
			}
		}
	}

#ifndef NO_STYLE_WINDOWSMODERN
	app.setStyle(new WindowsModernStyle());
#endif

	MainWindow window(fileToOpen);
	int exitCode = 0; // The window decided to not show itself, exiting now.

	if (window.isVisible()) {
		exitCode = app.exec();
	}

	QDateTime const currentTime = QDateTime::currentDateTime();
	QString const totalTime = QString::number(static_cast<qlonglong>(startedTime.secsTo(currentTime)));
	utils::UXInfo::reportTotalTime(totalTime, exitCode);
	return exitCode;
}
