#include <QTcpServer>
//#define _LONG_DEBUG
#include "dbg.h"

#include "server.h"

#include "repothread.h"

QRealRepoServer::QRealRepoServer(QObject *parent) : QTcpServer(parent)
{
dbg;
	listen( QHostAddress("127.0.0.1"), 6666);
	qDebug() << isListening() << serverAddress() << serverPort();

	root = new Root();
	typesInfo = new RepoTypesInfo();
	count = 666;
}

QRealRepoServer::~QRealRepoServer()
{
dbg;
}

void QRealRepoServer::incomingConnection(int socketDescriptor)
{
dbg;
	QRealRepoServerThread *thread = new QRealRepoServerThread(socketDescriptor, this, root, typesInfo, count);
	connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
	connect(thread, SIGNAL(finished()), this, SLOT(killall()));
	thread->start();
	count += 10;
}

void QRealRepoServer::killall()
{
dbg;
	qDebug() << "objects: " << root->getObjectsSize();
	qDebug() << "links: " << root->getLinksSize();
}
