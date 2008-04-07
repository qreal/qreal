#pragma once
#include <QThread>
#include <QTcpSocket>

#include "../common/defs.h"
#include "../common/classes.h"
#include "generated/repotypesinfo.h"

namespace reposerver
{
  
  class QRealRepoServerThread: public QThread
  {
    Q_OBJECT
    
  public:
    QRealRepoServerThread(int const &mSocketDescriptor
      , QObject *const parent
      , Root *const mRoot
      , RepoTypesInfo *const info
      , int const &id);
    
    void run();
    
  public slots:
    
  signals:
    void error(QTcpSocket::SocketError socketError);
    
  private:
    void handleCommand(QString const &cmd, QTcpSocket *const socket);
    int getId();
    
    int mSocketDescriptor;
    bool mCanRead;
    Root *mRoot;
    RepoTypesInfo *mTypesInfo;
    int mCounter;
  };
  
}
