#pragma once
#include <QThread>
#include <QTcpSocket>

#include "../common/defs.h"
#include "../common/classes.h"
#include "generated/repotypesinfo.h"

namespace reposerver
{
  
  typedef QVector<QString> QStringVector;
  typedef std::pair<int, QString> IntQStringPair;
  
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
    IntQStringPair handleCommand(QString const &cmd);
    
    IntQStringPair handleGetName(QStringVector const &params);
    IntQStringPair handleSetName(QStringVector const &params);
    IntQStringPair handleSetParent(QStringVector const &params);
    IntQStringPair handleGetParent(QStringVector const &params);
    IntQStringPair handleCreateEntity(QStringVector const &params);    
    IntQStringPair handleDeleteEntity(QStringVector const &params);
    IntQStringPair handleGetTypesCount(QStringVector const &params);
    IntQStringPair handleGetAllTypes(QStringVector const &params);
    IntQStringPair handleGetTypeInfo(QStringVector const &params);
    IntQStringPair handleGetTypeByName(QStringVector const &params);
    IntQStringPair handleGetTypesByMetatype(QStringVector const &params);
    IntQStringPair handleGetObjectsByType(QStringVector const &params);
    IntQStringPair handleGetObjectData(QStringVector const &params);
    IntQStringPair handleGetChildren(QStringVector const &params);
    IntQStringPair handleGetDescription(QStringVector const &params);
    IntQStringPair handleSetDescription(QStringVector const &params);
    IntQStringPair handleGetPosition(QStringVector const &params);
    IntQStringPair handleSetPosition(QStringVector const &params);
    IntQStringPair handleGetConfiguration(QStringVector const &params);
    IntQStringPair handleSetConfiguration(QStringVector const &params);
    IntQStringPair handleSetProperty(QStringVector const &params);
    IntQStringPair handleGetProperty(QStringVector const &params);
    IntQStringPair handleAddLink(QStringVector const &params);
    IntQStringPair handleRemoveLink(QStringVector const &params);
    IntQStringPair handleGetEntireObject(QStringVector const &params);
    IntQStringPair handleGetLinksByObject(QStringVector const &params);
    IntQStringPair handleGetObjectsByLink(QStringVector const &params);
    
    IntQStringPair ReportError(const int &errorCode);
    IntQStringPair ReportSuccess(const QString &data);
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
