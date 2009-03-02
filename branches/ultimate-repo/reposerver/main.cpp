/** @file main.cpp
 * 	@brief Главный файл сервера репозитория
 * */
#include <QCoreApplication>
#include <QTextCodec>

#include "server.h"

int main(int argc, char *argv[])
{
	QTextCodec *codec = QTextCodec::codecForName("UTF-8");
	QTextCodec::setCodecForCStrings(codec);

	QCoreApplication app(argc, argv);

	int port = 6666;
	if (argc == 2) {
		port = QString(argv[1]).toInt();
	}

	repoServer::QRealRepoServer repo(port);

	return app.exec();
}
