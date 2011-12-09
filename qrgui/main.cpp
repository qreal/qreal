#include "mainwindow/mainWindow.h"
#include "thirdparty/windowsmodernstyle.h"

#include <QtCore/QtPlugin>
#include <QtGui/QApplication>

using namespace qReal;

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	QTranslator appTranslator;
	if (app.arguments().count() <= 1 || app.arguments().at(1) != "--no-locale") {
		appTranslator.load(":/qrgui_" + QLocale::system().name());
		app.installTranslator(&appTranslator);
	}

#ifndef NO_STYLE_WINDOWSMODERN
	app.setStyle(new WindowsModernStyle());
#endif

	MainWindow window;
	if (window.isVisible())
		return app.exec();
	else  // The window decided to not show itself, exiting now.
		return 0;
}
