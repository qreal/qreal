/** @file server.cpp
 * 	@brief Сервер репозитория
 * */
#include "server.h"

#include <QTcpServer>
#define _LONG_DEBUG
#include "dbg.h"

#include "repothread.h"

using namespace reposerver;

QRealRepoServer::QRealRepoServer(QObject *const parent): QTcpServer(parent)
{
  dbg;
  listen(QHostAddress("127.0.0.1"), 6666);
  qDebug() << isListening() << serverAddress() << serverPort();
  
  mRoot = new Root();
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
    , this, mRoot, mTypesInfo, mCount);
  connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
  connect(thread, SIGNAL(finished()), this, SLOT(printout()));
  thread->start();
  mCount += 10;
}

void QRealRepoServer::printout() const
{
  dbg;
  qDebug() << "objects: " << mRoot->getObjectsSize();
  qDebug() << "links: " << mRoot->getLinksSize();
}
