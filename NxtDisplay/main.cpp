#include <QtGui/QApplication>

#include "nxtDisplay.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	NxtDisplay x;
	x.show();

	return a.exec();
}
