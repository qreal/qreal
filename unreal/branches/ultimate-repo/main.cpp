#include <QDebug>
#include <QtGui>
#include <QApplication>

#include "mainwindow.h"

MainWindow *window;

int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE(static);
	Q_INIT_RESOURCE(real_dynamic);
		
	QApplication app(argc, argv);
	window = new MainWindow;
	window->show();
	return app.exec();
}
