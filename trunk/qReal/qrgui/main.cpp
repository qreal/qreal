#include "mainwindow/mainwindow.h"

#include <QtPlugin>
#include <QApplication>

using namespace qReal;


int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	MainWindow window;
	window.show();
	return app.exec();
}
