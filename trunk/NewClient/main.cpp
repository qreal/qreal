#include <QtGui/QApplication>
#include "newclient.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	NewClient w;
	w.show();
	return a.exec();
}
