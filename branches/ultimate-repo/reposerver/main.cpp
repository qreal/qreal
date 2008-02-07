#include <QApplication>
#include <QtCore>
#include <QTextCodec>

#include "server.h"

int main(int argc, char *argv[])
{
	QTextCodec *codec = QTextCodec::codecForName("UTF-8");
	QTextCodec::setCodecForCStrings(codec);

	QApplication app(argc, argv);
	QRealRepoServer repo;

	return app.exec();
}	
