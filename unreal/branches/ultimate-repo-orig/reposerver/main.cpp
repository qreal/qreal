/** @file main.cpp
 * 	@brief Главный файл сервера репозитория
 * */
#include <QCoreApplication>
#include <QTextCodec>
#include <QFile>

#include "server.h"

int main(int argc, char *argv[])
{
	QTextCodec *codec = QTextCodec::codecForName("UTF-8");
	QTextCodec::setCodecForCStrings(codec);

	QCoreApplication app(argc, argv);

	int port = 6666;
	while (argc > 1)
	{
		argc--;
		if (!strcmp(argv[argc], "-ie"))
		{
			QFile::remove("repothread_log.txt");
			continue;
		}
		port = QString(argv[argc]).toInt();
		if (port == 0)
			port = 6666;
	}

	repoServer::QRealRepoServer repo(port);

	return app.exec();
}
