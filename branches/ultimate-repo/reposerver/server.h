#pragma once

#include <QTcpServer>
#include "../common/classes.h"
#include "generated/repotypesinfo.h"

namespace reposerver
{
  
  class QRealRepoServer: public QTcpServer
  {
    Q_OBJECT
    
  public:
    QRealRepoServer(QObject *const parent = 0);
    ~QRealRepoServer();
    
  public slots:
    void killall() const;
    
  protected:
    void incomingConnection(int const &socketDescriptor);
    
  private:
    int mCount;
    
    RepoTypesInfo *mTypesInfo;
    Root *mRoot;
  };
  
}
