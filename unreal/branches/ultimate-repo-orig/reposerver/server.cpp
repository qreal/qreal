/** @file server.cpp
 * 	@brief Сервер репозитория
 * */
#include "server.h"

#include <QTcpServer>
#define _LONG_DEBUG
#include "dbg.h"

#include "repothread.h"

using namespace repoServer;

QRealRepoServer::QRealRepoServer(int port, QObject *const parent): QTcpServer(parent)
{
	dbg;
	listen(QHostAddress::Any, port);
	qDebug() << isListening() << serverAddress() << serverPort();

	mRepoData = new RepoData();
	mTypesInfo = new RepoTypesInfo();
	mCount = 888; // Ia Bel-Shamharoth fhtagn!
}

QRealRepoServer::~QRealRepoServer()
{
	dbg;
}

void QRealRepoServer::incomingConnection(int socketDescriptor)
{
	dbg;
	QRealRepoServerThread *thread = new QRealRepoServerThread(socketDescriptor
		, this, mRepoData, mTypesInfo, mCount);
	connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
	connect(thread, SIGNAL(finished()), this, SLOT(printout()));
	thread->start();
	mCount += 10;
}

void QRealRepoServer::printout() const
{
	dbg;
	qDebug() << "objects: " << mRepoData->getObjectsSize();
	qDebug() << "links: " << mRepoData->getLinksSize();
}
