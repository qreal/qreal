#pragma once
#include <QThread>
#include <QTcpSocket>

#include "../common/defs.h"
#include "../common/classes.h"
#include "generated/repotypesinfo.h"

namespace reposerver
{
  
  typedef QVector<QString> QStringVector;
  
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
    QString handleCommand(QString const &cmd);
    
    QString handleGetName(QStringVector const &params);
    QString handleSetName(QStringVector const &params);
    QString handleSetParent(QStringVector const &params);
    QString handleCreateEntity(QStringVector const &params);    
    QString handleDeleteEntity(QStringVector const &params);
    QString handleGetTypesCount(QStringVector const &params);
    QString handleGetAllTypes(QStringVector const &params);
    QString handleGetTypeInfo(QStringVector const &params);
    QString handleGetTypeByName(QStringVector const &params);
    QString handleGetTypesByMetatype(QStringVector const &params);
    QString handleGetObjectsByType(QStringVector const &params);
    QString handleGetObjectData(QStringVector const &params);
    QString handleGetChildren(QStringVector const &params);
    QString handleGetDescription(QStringVector const &params);
    QString handleSetDescription(QStringVector const &params);
    QString handleGetPosition(QStringVector const &params);
    QString handleSetPosition(QStringVector const &params);
    QString handleGetConfiguration(QStringVector const &params);
    QString handleSetConfiguration(QStringVector const &params);
    QString handleSetProperty(QStringVector const &params);
    QString handleGetProperty(QStringVector const &params);
    QString handleAddLink(QStringVector const &params);
    QString handleRemoveLink(QStringVector const &params);
    QString handleGetEntireObject(QStringVector const &params);
    QString handleGetLinksByObject(QStringVector const &params);
    QString handleGetObjectsByLink(QStringVector const &params);

    bool IsParamsNumberCorrect(QStringVector const &params, QString const &command
      , int const &paramsNum) const;
    void TryToRestoreState();
    
    int mSocketDescriptor;
    Root *mRoot;
    RepoTypesInfo *mTypesInfo;
    int mCounter;
    
    mutable QString mLog;
  };
  
}
