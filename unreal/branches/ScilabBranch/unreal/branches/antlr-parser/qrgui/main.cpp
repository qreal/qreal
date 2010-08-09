#include "mainwindow/mainwindow.h"
#include "thirdparty/windowsmodernstyle.h"

#include <QtPlugin>
#include <QApplication>

using namespace qReal;

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

#ifndef NO_STYLE_WINDOWSMODERN
	app.setStyle(new WindowsModernStyle());
#endif

	MainWindow window;
	if (window.isVisible())
		return app.exec();
	else  // Окно решило в конструкторе не показывать себя, выходим сразу.
		return 0;
}
