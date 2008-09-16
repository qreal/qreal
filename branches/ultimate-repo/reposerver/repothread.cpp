#include "repothread.h"
//#define _LONG_DEBUG
#include "dbg.h"

#include <QtNetwork>
#include <QByteArray>
#include <qstring.h>

using namespace reposerver;

QRealRepoServerThread::QRealRepoServerThread(int const &socketDescriptor
    , QObject * const parent, Root * const root, RepoTypesInfo * const info
    , int const &id)
: QThread(parent), mSocketDescriptor(socketDescriptor), mRoot(root)
, mTypesInfo(info), mCounter(id)
{
    dbg;
}

void QRealRepoServerThread::TryToRestoreState()
{
    // Some means to restore repository contents by reading and applying saved 
    // commands. It will work for only 1 client, and IS INTENDED FOR TESTING ONLY!
    QFile file("repothread_log.txt");
  
    // Works only if repo is empty.
    if (file.exists() && mRoot->getObjectsSize() == 0 && mRoot->getLinksSize() == 0)
    {
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug() << "Command log found, restoring";
            QTextStream savedCommands(&file);
            while (!savedCommands.atEnd())
            {
                QString command = savedCommands.readLine();
                qDebug() << command;
                handleCommand(command);        
            }    
            file.close();
        }    
    }
}

void QRealRepoServerThread::run()
{
    dbg;
    QTcpSocket tcpSocket;

    if (!tcpSocket.setSocketDescriptor(mSocketDescriptor))
    {
        emit error(tcpSocket.error());
        return;
    }
  
    TryToRestoreState();  
  
    // Log of commands - to restore repository state in a next run. 
    // Not thread-safe.
    QFile file("repothread_log.txt");  

    bool loggedMode = true;
    if (!file.open(QIODevice::Append | QIODevice::Text))
        loggedMode = false;
    QTextStream commandLog(&file);

    while (tcpSocket.state() != QAbstractSocket::UnconnectedState)
    {

        //bool res =
        tcpSocket.waitForReadyRead();
        //qDebug() << "ready - " << res;

        while (tcpSocket.bytesAvailable() == 0
            && tcpSocket.state() != QAbstractSocket::UnconnectedState)
        {
            sleep(1);
            qDebug() << "server is sleeping. state is " << tcpSocket.state();
            tcpSocket.waitForReadyRead();
        }

        if (tcpSocket.state() == QAbstractSocket::UnconnectedState)
            break;

        QByteArray rawdata = tcpSocket.readAll();
        QString data = QString(rawdata);
    
        if (loggedMode)
            commandLog << data << endl;

        QString resp = handleCommand(data);
        tcpSocket.write(resp.toUtf8());
    }
  
    if (loggedMode)
        file.close();
}

bool QRealRepoServerThread::IsParamsNumberCorrect(QStringVector const &params
  , QString const &command, int const &paramsNum) const
{
    if (params.size() != paramsNum)
    {
        qDebug() << "Incorrect number of arguments in " << command << " command";
        foreach(QString param, params)
        {
            qDebug() << param;
        }
        qDebug() << "----------";
        if (params.size() < paramsNum)
        {
            qDebug() << "Command aborted!";
            return false;
        }
    }
    return true;
}

QString QRealRepoServerThread::handleGetName(QStringVector const &params)
{
    if (!IsParamsNumberCorrect(params, "GetName", 1))
        return QString::number(ERR_INCORRECT_PARAMS);

    int id = params[0].toInt();
    QString resp = "";
    if (Object *obj = mRoot->getObject(id))
        resp = obj->getName();
    else if (Link *link = mRoot->getLink(id))
        resp = link->getName();
    else
    {
        qDebug() << "unknown entity's name requested, id " << id;
        return QString::number(ERR_INCORRECT_REQUEST);
    }

    mLog += QString(", sent %1's name: [%2]").arg(id).arg(resp);
    return resp;
}

QString QRealRepoServerThread::handleSetName(QStringVector const &params)
{
    if (!IsParamsNumberCorrect(params, "SetName", 2))
        return QString::number(ERR_INCORRECT_PARAMS);

    int id = params[0].toInt();
    QString name = params[1];
    if (Object * obj = mRoot->getObject(id))
        obj->setName(name);
    else if (Link * link = mRoot->getLink(id))
        link->setName(name);
    else
    {
        qDebug() << "unknown entity's name set requested, id " << id;
        return QString::number(ERR_INCORRECT_REQUEST);
    }
    mLog += QString(", new %1's name is [%2]").arg(id).arg(name);
    return QString::number(ERR_STATUS_OK);
}

QString QRealRepoServerThread::handleSetParent(QStringVector const &params)
{
    if (!IsParamsNumberCorrect(params, "SetParent", 2))
        return QString::number(ERR_INCORRECT_PARAMS);

    int const id = params[0].toInt();
    int const parentId = params[1].toInt();
    if (Object * obj = mRoot->getObject(id)){
        if (Object * oldParent = mRoot->getObject(obj->getParent()))
            oldParent->removeChild(id);
        if (Object * newParent = mRoot->getObject(parentId))
            newParent->addChild(id);
        obj->setParent(parentId);
    } else if (Link * link = mRoot->getLink(id)){
        if (Object * oldParent = mRoot->getObject(link->getParent()))
            oldParent->removeChild(id);
        if (Object * newParent = mRoot->getObject(parentId))
            newParent->addChild(id);
        link->setParent(parentId);
    } else {
        qDebug() << "unknown entity's parent setting requested, id " << id;
        return QString::number(ERR_INCORRECT_REQUEST);
    }
    mLog += QString(", new %1's parent is %2").arg(id).arg(parentId);
    return QString::number(ERR_STATUS_OK);
}

QString QRealRepoServerThread::handleGetParent(QStringVector const &params)
{
    if (!IsParamsNumberCorrect(params, "GetParent", 1))
        return QString::number(ERR_INCORRECT_PARAMS);

    int const id = params[0].toInt();
	QString resp;
    if (Object * obj = mRoot->getObject(id)){
        resp = QString("%1\t%2\t").arg(ERR_STATUS_OK).arg(obj->getParent());
    } else if (Link * link = mRoot->getLink(id)){
        resp += QString("%1\t%2\t").arg(ERR_STATUS_OK).arg(link->getParent());
    } else {
        qDebug() << "unknown entity's parent requested, id " << id;
        return QString::number(ERR_INCORRECT_REQUEST);
    }
    return resp;
}

QString QRealRepoServerThread::handleCreateEntity(QStringVector const &params)
{
    if (!IsParamsNumberCorrect(params, "CreateEntity", 3))
        return QString::number(ERR_INCORRECT_PARAMS);

    int type = params[0].toInt();
    int id = ++mCounter;
    int parent = params[2].toInt();
    mLog += QString(", id: %1, type: %2, parent: %3 ").arg(id).arg(type).arg(parent);
    QString name = params[1];
    if (mTypesInfo->analyseType(type) == TYPE_OBJECT){
        Object *obj = new Object(id, type, 0, 0);
        obj->setName(name);
        obj->setParent(parent);
        obj->setConfiguration("(0,0);(50,0);(50,70);(0,70)");
        mRoot->addObject(id, obj);
        mLog += QString(", object created, name %1").arg(name);
    } else if (mTypesInfo->analyseType(type) == TYPE_LINK){
        Link *link = new Link(id, type);
        link->setName(name);
        link->setParent(parent);
        link->setConfiguration("(0,0);(200,60)");
        mRoot->addLink(id, link);
        mLog += QString(", link created, name %1").arg(name);
    } else {
        qDebug() << "Wrong analyseType result";
        return QString::number(ERR_INCORRECT_REQUEST);
    }

    if (Object * obj = mRoot->getObject(parent))
    {
        obj->addChild(id);
    }

    mTypesInfo->elementCreated(type, id);
    return QString::number(id); // FIXME: error code should always come first, then the args (if there are any) 
}

QString QRealRepoServerThread::handleDeleteEntity(QStringVector const &params)
{
    if (!IsParamsNumberCorrect(params, "DeleteEntity", 1))
        return QString::number(ERR_INCORRECT_PARAMS);

    int id = params[0].toInt();
    if (Object * obj = mRoot->getObject(id))
    {
        if (Object * parent = mRoot->getObject(obj->getParent()))
            parent->removeChild(id);
        if (obj->childrenCount() != 0){
            QStringList children = obj->childrenToString().split("\t");

            foreach(QString child, children){
                if (child.toInt() != 0)
                // TODO: Make sure that no object can be ancestor of itself so we will
                // not get stack owerflow here.
                handleDeleteEntity(QStringList(child).toVector());
            }
        }
        mTypesInfo->elementDeleted(obj->getType(), id);
        mRoot->deleteObject(id);
    } else if (Link * link = mRoot->getLink(id)){
        if (Object * parent = mRoot->getObject(link->getParent()))
            parent->removeChild(id);
        mTypesInfo->elementDeleted(link->getType(), id);
        mRoot->deleteLink(id);
    } else {
        qDebug() << "Wrong analyseType result";
        return QString::number(ERR_INCORRECT_REQUEST);
    }    
    
    mLog += QString(", deleted entity [%1]").arg(id);
    return QString::number(ERR_STATUS_OK);
}

QString QRealRepoServerThread::handleGetTypesCount(QStringVector const &params)
{
    if (!IsParamsNumberCorrect(params, "GetTypesCount", 0))
        return QString::number(ERR_INCORRECT_PARAMS);

    // FIXME: error code should always come first, then the args (if there are any)
    QString resp = QString::number(mTypesInfo->getTypesCount());  
    mLog += QString(", sending types count: %1").arg(resp);
    return resp;
}

QString QRealRepoServerThread::handleGetAllTypes(QStringVector const &params)
{
    if (!IsParamsNumberCorrect(params, "GetAllTypes", 0))
        return QString::number(ERR_INCORRECT_PARAMS);

    int count = mTypesInfo->getTypesCount();
    QString resp = "";
    // FIXME: error code should always come first, then the args (if there are any)
    for (int i = 1; i <= count; i++)
        resp += QString("%1\t").arg(i); // It's very informative :)
    mLog += QString(", sending types count: %1").arg(resp);
    return resp;
}

QString QRealRepoServerThread::handleGetTypeInfo(QStringVector const &params)
{
    if (!IsParamsNumberCorrect(params, "GetTypeInfo", 1))
        return QString::number(ERR_INCORRECT_PARAMS);

    int id = params[0].toInt();
    // FIXME: error code should always come first, then the args (if there are any)
    QString resp = mTypesInfo->getTypeInfo(id).toString();
    mLog += QString(", sending type info: [%1]").arg(resp);
    return resp;
}

QString QRealRepoServerThread::handleGetTypeByName(QStringVector const &params)
{
    if (!IsParamsNumberCorrect(params, "GetTypeByName", 1))
        return QString::number(ERR_INCORRECT_PARAMS);

    QString name = params[0];
    // FIXME: error code should always come first, then the args (if there are any)
    QString resp = mTypesInfo->getTypeInfo(name).toString();
    mLog += QString(", sending type info: [%1]").arg(resp);
    return resp;
}

QString QRealRepoServerThread::handleGetTypesByMetatype(QStringVector const &params)
{
    if (!IsParamsNumberCorrect(params, "GetTypesByMetatype", 1))
        return QString::number(ERR_INCORRECT_PARAMS);

    MetaType metatype = static_cast<MetaType>(params[0].toInt());
    // FIXME: error code should always come first, then the args (if there are any)
    QString resp = mTypesInfo->getTypesByMetatype(metatype);
    mLog += QString(", sending types list: [%1]").arg(resp);
    return resp;
}

QString QRealRepoServerThread::handleGetObjectsByType(QStringVector const &params)
{
    if (!IsParamsNumberCorrect(params, "GetObjectsByType", 1))
        return QString::number(ERR_INCORRECT_PARAMS);

    QString resp = "";
    int type = params[0].toInt();
    // FIXME: error code should always come first, then the args (if there are any)
    if (mTypesInfo->analyseType(type) == TYPE_OBJECT){
        resp = mRoot->getObjectsByType(type);
    } else if (mTypesInfo->analyseType(type) == TYPE_LINK){
        resp = mRoot->getLinksByType(type);
    } else {
        qDebug() << "Wrong analyseType result";
        return QString::number(ERR_INCORRECT_REQUEST);
    }
    mLog += QString(", sending objects of type: %1 -- [%2]").arg(type).arg(resp);
    return resp;
}

QString QRealRepoServerThread::handleGetObjectData(QStringVector const &params)
{
    if (!IsParamsNumberCorrect(params, "GetObjectData", 1))
        return QString::number(ERR_INCORRECT_PARAMS);

    int id = params[0].toInt();
    int childCount = 0;
    int type = -1;
    QString name = "";
    QString res = "%1\t%2\t%3\t%4\t%5\t";
    QString resp = QString::number(ERR_UNKNOWN_ERROR);
    if (Object * obj = mRoot->getObject(id)){
        name = obj->getName();
        childCount = obj->childrenCount();
        type = obj->getType();
    } else if (Link * link = mRoot->getLink(id)){
        name = link->getName();
        type = link->getType();
    } else{
        qDebug() << "Wrong analyseType result";
        return QString::number(ERR_INCORRECT_REQUEST);
    }    

    if (type != -1){
        RealType info = mTypesInfo->getTypeInfo(type);
    // FIXME: error code should always come first, then the args (if there are any)
        resp = res.arg(id).arg(name).arg(type).arg(info.getDescription()).arg(childCount);
    }
    mLog += QString(", sending object data: [%1]").arg(resp);
    return resp;
}

QString QRealRepoServerThread::handleGetChildren(QStringVector const &params)
{
    if (!IsParamsNumberCorrect(params, "GetChildren", 1))
        return QString::number(ERR_INCORRECT_PARAMS);

    int id = params[0].toInt();
    QString resp = QString::number(ERR_UNKNOWN_ERROR);
    // FIXME: error code should always come first, then the args (if there are any)
    if (Object * obj = mRoot->getObject(id))
        resp = obj->childrenToString();
    mLog += QString(", sending %1's children - [%2]").arg(id).arg(resp);
    return resp;
}

QString QRealRepoServerThread::handleGetDescription(QStringVector const &params)
{
    if (!IsParamsNumberCorrect(params, "GetDescription", 1))
        return QString::number(ERR_INCORRECT_PARAMS);

    int id = params[0].toInt();
    QString resp = "\t";
    // FIXME: error code should always come first, then the args (if there are any)
    if (Object * obj = mRoot->getObject(id))
        resp = obj->getDescription();
    else if (Link * link = mRoot->getLink(id))
        resp = link->getDescription();
    else{
        qDebug() << "Wrong analyseType result";
        return QString::number(ERR_INCORRECT_REQUEST);
    }    
    mLog += QString(", sending description for id %1 - [%2]").arg(id).arg(resp);
    return resp;
}

QString QRealRepoServerThread::handleSetDescription(QStringVector const &params)
{
    if (!IsParamsNumberCorrect(params, "SetDescription", 2))
        return QString::number(ERR_INCORRECT_PARAMS);

    int id = params[0].toInt();
    QString desc = params[1];
    if (Object * obj = mRoot->getObject(id))
        obj->setDescription(desc);
    else if (Link * link = mRoot->getLink(id))
        link->setDescription(desc);
    else{
        qDebug() << "Wrong analyseType result";
        return QString::number(ERR_INCORRECT_REQUEST);
    }    
    mLog += QString(", new description for id %1 - [%2]").arg(id).arg(desc);
    return QString::number(ERR_STATUS_OK); 
}

QString QRealRepoServerThread::handleGetPosition(QStringVector const &params)
{
    if (!IsParamsNumberCorrect(params, "GetPosition", 1))
        return QString::number(ERR_INCORRECT_PARAMS);

    int id = params[0].toInt();
    QString resp = "";
    // FIXME: error code should always come first, then the args (if there are any)
    if (Object * obj = mRoot->getObject(id))
        resp = QString("%1;%2").arg(obj->getX()).arg(obj->getY());
    else if (Link * link = mRoot->getLink(id))
        resp = link->getPosition();
    else {
        qDebug() << "Wrong analyseType result";
        return QString::number(ERR_INCORRECT_REQUEST);
    }    
    if (resp.isEmpty())
        resp = "\t";
    mLog += QString(", sending position for obj %1 - [%2]").arg(id).arg(resp);
    return resp;
}

QString QRealRepoServerThread::handleSetPosition(QStringVector const &params)
{
    if (!IsParamsNumberCorrect(params, "SetPosition", 3))
        return QString::number(ERR_INCORRECT_PARAMS);

    int id = params[0].toInt();
    int x = params[1].toInt();
    int y = params[2].toInt();
    if (Object * obj = mRoot->getObject(id)){
        obj->setX(x);
        obj->setY(y);
    } else if (Link * link = mRoot->getLink(id)){
        link->setPosition(QString("%1;%2").arg(x).arg(y));
    } else {
        qDebug() << "Wrong analyseType result";
        return QString::number(ERR_INCORRECT_REQUEST);
    }    
    mLog += QString(", new %1's position - (%2:%3)").arg(id).arg(x).arg(y);
    return QString::number(ERR_STATUS_OK);
}

QString QRealRepoServerThread::handleGetConfiguration(QStringVector const &params)
{
    if (!IsParamsNumberCorrect(params, "GetConfiguration", 1))
        return QString::number(ERR_INCORRECT_PARAMS);

    int id = params[0].toInt();
    QString resp = "";
    // FIXME: error code should always come first, then the args (if there are any)
    if (Object *obj = mRoot->getObject(id))
        resp = obj->getConfiguration();
    else if (Link *link = mRoot->getLink(id))
        resp = link->getConfiguration();
    else{
        qDebug() << "Wrong analyseType result";
        return QString::number(ERR_INCORRECT_REQUEST);
    }    
    if (resp.isEmpty())
        resp = "\t";
    mLog += QString(", sending conf for %1 - [%2] ").arg(id).arg(resp);
    return resp;
}

QString QRealRepoServerThread::handleSetConfiguration(QStringVector const &params)
{
    if (!IsParamsNumberCorrect(params, "SetConfiguration", 2))
        return QString::number(ERR_INCORRECT_PARAMS);

    int id = params[0].toInt();
    QString conf = params[1];
    if (Object *obj = mRoot->getObject(id))
        obj->setConfiguration(conf);
    else if (Link *link = mRoot->getLink(id))
        link->setConfiguration(conf);
    else{
        qDebug() << "Wrong analyseType result";
        return QString::number(ERR_INCORRECT_REQUEST);
    }    
    mLog += QString(", conf %1").arg(conf);
    return QString::number(ERR_STATUS_OK);
}

QString QRealRepoServerThread::handleSetProperty(QStringVector const &params)
{
    if (!IsParamsNumberCorrect(params, "SetProperty", 3))
        return QString::number(ERR_INCORRECT_PARAMS);

    int id = params[0].toInt();
    QString name = params[1];
    QString val = params[2];

    if (Object * obj = mRoot->getObject(id))
        obj->setProperty(name, val);
    else if (Link * link = mRoot->getLink(id)){
        link->print();
        if (name == "from"){
            int obj_id = link->getFrom();
            if (Object * obj = mRoot->getObject(obj_id)){
                obj->removeLink(id, OUTCOMING_LINK);
                link->removeObjectFrom();
            }
            if (Object * obj = mRoot->getObject(val.toInt())){
                obj->addLink(id, OUTCOMING_LINK);
                link->setObjectFrom(val.toInt());
            }
        } else if (name == "to"){
            int obj_id = link->getTo();
            if (Object * obj = mRoot->getObject(obj_id)){
                obj->removeLink(id, INCOMING_LINK);
                link->removeObjectTo();
            }
            if (Object * obj = mRoot->getObject(val.toInt())) {
                obj->addLink(id, INCOMING_LINK);
                link->setObjectTo(val.toInt());
            }
        } else
        link->setProperty(name, val);
    } else {
        qDebug() << "Wrong analyseType result";
        return QString::number(ERR_INCORRECT_REQUEST);
    }
    mLog += QString(", new property value: %1 - %2").arg(name).arg(val);
    return QString::number(ERR_STATUS_OK);
}

QString QRealRepoServerThread::handleGetProperty(QStringVector const &params)
{
    if (!IsParamsNumberCorrect(params, "GetProperty", 2))
        return QString::number(ERR_INCORRECT_PARAMS);

    int id = params[0].toInt();
    QString name = params[1];
    QString resp = "";
    // FIXME: error code should always come first, then the args (if there are any)
    if (Object * obj = mRoot->getObject(id))
        resp = obj->getProperty(name);
    else if (Link * link = mRoot->getLink(id)){
        link->print();
        if (name == "from"){
            resp = QString::number(link->getFrom());
        } else if (name == "to")
            resp = QString::number(link->getTo());
        else
            resp = link->getProperty(name);
    } else {
        qDebug() << "Wrong analyseType result";
        return QString::number(ERR_INCORRECT_REQUEST); 
    }

    if (resp.isEmpty())
        resp = "\t";
    mLog += QString(", sent property value: %1 - [%2]").arg(name).arg(resp);
    return resp;
}

QString QRealRepoServerThread::handleAddLink(QStringVector const &params)
{
    if (!IsParamsNumberCorrect(params, "AddLink", 3))
        return QString::number(ERR_INCORRECT_PARAMS);

    int id = params[0].toInt();
    int link_id = params[1].toInt();
    int dir = params[2].toInt();
    // qDebug() << "adding link";
    if (Object * obj = mRoot->getObject(id)){        
        // qDebug() << "\tobject found! searching for link";
        obj->addLink(link_id, dir);
        if (Link * link = mRoot->getLink(link_id)){
            // qDebug() << "\tlink found!";
            if (dir == OUTCOMING_LINK)
                link->setObjectFrom(id);
            else if (dir == INCOMING_LINK)
                link->setObjectTo(id);
            link->print();
        }
        obj->print();
    } else {
        qDebug() << "Wrong analyseType result";
        return QString::number(ERR_INCORRECT_REQUEST);
    }
    mLog += QString(", added new link %1 to object %2").arg(link_id).arg(id);
    return QString::number(ERR_STATUS_OK);
}

QString QRealRepoServerThread::handleRemoveLink(QStringVector const &params)
{
    if (!IsParamsNumberCorrect(params, "RemoveLink", 3))
        return QString::number(ERR_INCORRECT_PARAMS);

    int id = params[0].toInt();
    int link_id = params[1].toInt();
    int dir = params[2].toInt();
    //  qDebug() << "\tremoving link " << id << link_id << dir;
    if (Object * obj = mRoot->getObject(id)) {
        obj->removeLink(link_id, dir);
        //  qDebug() << "searching for link" << link_id;
        if (Link * link = mRoot->getLink(link_id)){
        //  qDebug() << "\tlink found!, dir" << dir;
            if (dir == OUTCOMING_LINK)
                link->removeObjectFrom();
            else if (dir == INCOMING_LINK)
                link->removeObjectTo();
            link->print();
        } else {
            qDebug() << "Wrong analyseType result";
            return QString::number(ERR_INCORRECT_REQUEST);
        }
    } else {
        qDebug() << "Wrong analyseType result";
        return QString::number(ERR_INCORRECT_REQUEST);
    }
    mLog += QString(", removed link %1 from object %2").arg(link_id).arg(id);
    return QString::number(ERR_STATUS_OK);
}

QString QRealRepoServerThread::handleGetEntireObject(QStringVector const &params)
{
    if (!IsParamsNumberCorrect(params, "GetEntireObject", 1))
        return QString::number(ERR_INCORRECT_PARAMS);

    int id = params[0].toInt();
    QString resp = "\t";
    // FIXME: error code should always come first, then the args (if there are any)
    if (Object * obj = mRoot->getObject(id))
        resp = obj->toString();
    else if (Link * link = mRoot->getLink(id))
        resp = link->toString();
    else {
        qDebug() << "Wrong analyseType result";
        return QString::number(ERR_INCORRECT_REQUEST);
    }    
    mLog += QString(", sending object %1: %2").arg(id).arg(resp);
    return resp;
}

QString QRealRepoServerThread::handleGetLinksByObject(QStringVector const &params)
{
    if (!IsParamsNumberCorrect(params, "GetLinksByObject", 2))
        return QString::number(ERR_INCORRECT_PARAMS);

    int id = params[0].toInt();
    int dir = params[1].toInt();
    QString resp = "";
    // FIXME: error code should always come first, then the args (if there are any)
    if (Object * obj = mRoot->getObject(id)){
        if (dir == INCOMING_LINK)
            resp = obj->getIncomingLinks();
        else if (dir == OUTCOMING_LINK)
            resp = obj->getOutcomingLinks();
    } else{
        qDebug() << "Wrong analyseType result";
        return QString::number(ERR_INCORRECT_REQUEST);
    }    
    if (resp.isEmpty())
        resp = "\t";
    mLog += QString(", sending object's links %1: [%2]").arg(id).arg(resp);
    return resp;
}

QString QRealRepoServerThread::handleGetObjectsByLink(QStringVector const &params)
{
    if (!IsParamsNumberCorrect(params, "GetObjectsByLink", 1))
        return QString::number(ERR_INCORRECT_PARAMS);

    int id = params[0].toInt();
    QString resp = "\t";
    // FIXME: error code should always come first, then the args (if there are any)
    if (Link * link = mRoot->getLink(id))
        resp = link->getObjects();
    else{
        qDebug() << "Wrong analyseType result";
        return QString::number(ERR_INCORRECT_REQUEST);
    }    
    mLog += QString(", sending link's objects %1: %2").arg(id).arg(resp);
    return resp;
}

QString QRealRepoServerThread::handleCommand(QString const &data)
{
    dbg;
    QStringList command = data.split("\t");
    int cmd = command.front().toInt();
    command.pop_front();  // Discard command code.
    if (!command.empty() && command.back() == "" && cmd != CMD_CREATE_ENTITY)
        command.pop_back();  // Discard empty string that is sent at the end of nearly
                         // every command by some reason.
    mLog = QString("cmd: %1").arg(cmd);
    QString resp = "";

    switch (cmd)
    {
        case CMD_GET_NAME:
        {
            resp = handleGetName(command.toVector());
            break;
        } 
        case CMD_SET_NAME:
        {
            resp = handleSetName(command.toVector());
            break;
        }
        case CMD_SET_PARENT:
        {
            resp = handleSetParent(command.toVector());
            break;
        }
        case CMD_GET_PARENT:
        {
            resp = handleGetParent(command.toVector());
            break;
        }
        case CMD_CREATE_ENTITY:
        {
            resp = handleCreateEntity(command.toVector());
            break;
        }
        case CMD_DELETE_ENTITY:
        {
            resp = handleDeleteEntity(command.toVector());
            break;
        }
        case CMD_GET_TYPES_COUNT:
        {
            resp = handleGetTypesCount(command.toVector());
            break;
        }
        case CMD_GET_ALL_TYPES: // TODO: replace with typesInfo call or something like that
        {
            resp = handleGetAllTypes(command.toVector());
            break;
        }
        case CMD_GET_TYPE_INFO:
        {
            resp = handleGetTypeInfo(command.toVector());
            break;
        }
        case CMD_GET_TYPE_BY_NAME:
        {
            resp = handleGetTypeByName(command.toVector());
            break;
        }
        case CMD_GET_TYPES_BY_METATYPE:
        {
            resp = handleGetTypesByMetatype(command.toVector());
            break;
        }
        case CMD_GET_OBJECTS_BY_TYPE:
        {
            resp = handleGetObjectsByType(command.toVector());
            break;
        }
        case CMD_GET_OBJECT_DATA:
        {
            resp = handleGetObjectData(command.toVector());
            break;
        }
        case CMD_GET_CHILDREN:
        {
            resp = handleGetChildren(command.toVector());
            break;
        }
        case CMD_GET_DESCRIPTION:
        {
            resp = handleGetDescription(command.toVector());
            break;
        }
        case CMD_SET_DESCRIPTION:
        {
            resp = handleSetDescription(command.toVector());
            break;
        }
        case CMD_GET_POSITION:
        {
            resp = handleGetPosition(command.toVector());
            break;
        }
        case CMD_SET_POSITION:
        {
            resp = handleSetPosition(command.toVector());
            break;
        }
        case CMD_GET_CONFIGURATION:
        {
            resp = handleGetConfiguration(command.toVector());
            break;
        }
        case CMD_SET_CONFIGURATION:
        {
            resp = handleSetConfiguration(command.toVector());
            break;
        }
        case CMD_SET_PROPERTY:
        {
            resp = handleSetProperty(command.toVector());
            break;
        }
        case CMD_GET_PROPERTY:
        {
            resp = handleGetProperty(command.toVector());
            break;
        }
        case CMD_ADD_LINK:
        {
            resp = handleAddLink(command.toVector());
            break;
        }
        case CMD_REMOVE_LINK:
        {
            resp = handleRemoveLink(command.toVector());
            break;
        }
        case CMD_GET_ENTIRE_OBJECT:
        {
            resp = handleGetEntireObject(command.toVector());
            break;
        }
        case CMD_GET_LINKS_BY_OBJECT:
        {
            resp = handleGetLinksByObject(command.toVector());
            break;
        }
        case CMD_GET_OBJECTS_BY_LINK:
        {
            resp = handleGetObjectsByLink(command.toVector());
            break;
        }
        default:
        {
            qDebug() << "Unknown command: " << data;
            resp = QString::number(ERR_STATUS_OK);
        }
    }

    qDebug() << mLog;
    return resp;
}
