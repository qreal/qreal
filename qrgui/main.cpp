#include <QtCore/QtPlugin>
#include <QtWidgets/QApplication>

#include "mainwindow/mainWindow.h"
#include "thirdparty/windowsmodernstyle.h"
#include "qRealApplication.h"

using namespace qReal;

void clearConfig()
{
	SettingsManager::clearSettings();
	SettingsManager::instance()->saveData();
}

int main(int argc, char *argv[])
{
	QRealApplication app(argc, argv);

	QTranslator guiTranslator;
	QTranslator utilsTranslator;
	QTranslator qtTranslator;
	if (app.arguments().count() <= 1 || app.arguments().at(1) != "--no-locale") {
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
			for (int i = 0; i < argc - 1; i++)
			{
				if (app.arguments().at(i) == "--config"
						&& app.arguments().at(i + 1).endsWith(".ini"))
				{
					QString settingsFileName = app.arguments().at(i + 1);
					SettingsManager::instance()->loadSettings(settingsFileName);
				}
				else
				{
					if (app.arguments().at(i).endsWith(".qrs"))
					{
						fileToOpen = app.arguments().at(i);
					}
				}
			}
			if (app.arguments().at(argc - 1).endsWith(".qrs"))
			{
				fileToOpen = app.arguments().at(argc - 1);
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
