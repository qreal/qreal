/** @file main.cpp
 * 	@brief Главный файл сервера репозитория
 * */
#include <QApplication>
#include <QtCore>
#include <QTextCodec>

#include "server.h"

int main(int argc, char *argv[])
{
	QTextCodec *codec = QTextCodec::codecForName("UTF-8");
	QTextCodec::setCodecForCStrings(codec);

	QApplication app(argc, argv);

	int port = 6666;
	if (argc == 2) {
		port = QString(argv[1]).toInt();
	}

	reposerver::QRealRepoServer repo(port);

	return app.exec();
}
