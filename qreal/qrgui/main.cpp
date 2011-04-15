#include "mainwindow/mainwindow.h"
#include "thirdparty/windowsmodernstyle.h"

#include <QtPlugin>
#include <QApplication>

using namespace qReal;

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	QTranslator appTranslator;
	appTranslator.load(":/qrgui_" + QLocale::system().name());
	app.installTranslator(&appTranslator);

#ifndef NO_STYLE_WINDOWSMODERN
	app.setStyle(new WindowsModernStyle());
#endif

	MainWindow window;
	if (window.isVisible())
		return app.exec();
	else  // The window decided to not show itself, exiting now.
		return 0;
}
