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

int main(int argc, char *argv[])
{
	QDateTime const startedTime = QDateTime::currentDateTime();
	QRealApplication app(argc, argv);

	QTranslator guiTranslator;
	QTranslator utilsTranslator;
	QTranslator qtTranslator;
	if (!app.arguments().contains("--no-locale")) {
		guiTranslator.load(":/qrgui_" + QLocale::system().name());
		utilsTranslator.load(":/qrutils_" + QLocale::system().name());
		qtTranslator.load(":/qt_" + QLocale::system().name());
		app.installTranslator(&guiTranslator);
		app.installTranslator(&utilsTranslator);
		app.installTranslator(&qtTranslator);
	}

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
