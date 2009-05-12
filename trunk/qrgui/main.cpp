#include "mainwindow/mainwindow.h"

#include <QtPlugin>
#include <QApplication>

using namespace qReal;

MainWindow *window;

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	window = new MainWindow();
	window->show();
	return app.exec();
}
