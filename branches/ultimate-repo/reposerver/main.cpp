#include <QApplication>
#include <QtCore>
#include <QTextCodec>

#include "server.h"

int main(int argc, char *argv[])
{
	QTextCodec *codec = QTextCodec::codecForName("UTF-8");
	QTextCodec::setCodecForCStrings(codec);

	QApplication app(argc, argv);
	reposerver::QRealRepoServer repo;

	return app.exec();
}	
