#include "mainwindow/mainwindow.h"

#include <QtPlugin>
#include <QApplication>

using namespace qReal;


int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	MainWindow window;
	if (window.isVisible())
		return app.exec();
	else  // Окно решило в конструкторе не показывать себя, выходим сразу.
		return 0;
}
