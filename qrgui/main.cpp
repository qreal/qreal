#include <QtCore/QtPlugin>
#include <QtWidgets/QApplication>

#include "mainwindow/mainWindow.h"
#include "thirdparty/windowsmodernstyle.h"

using namespace qReal;

void clearConfig()
{
	SettingsManager::clearSettings();
	SettingsManager::instance()->saveData();
}

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

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
			int setIndex = app.arguments().indexOf("--config");
			if (setIndex > -1) {
				QString settingsFileName = app.arguments().at(setIndex + 1);
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
	if (window.isVisible()) {
		return app.exec();
	} else {  // The window decided to not show itself, exiting now.
		return 0;
	}
}
