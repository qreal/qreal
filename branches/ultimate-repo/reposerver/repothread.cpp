#include "repothread.h"
//#define _LONG_DEBUG
#include "dbg.h"

#include <QtNetwork>
#include <QByteArray>

using namespace reposerver;

QRealRepoServerThread::QRealRepoServerThread(int const &socketDescriptor
, QObject *const parent, Root *const root, RepoTypesInfo *const info
, int const &id)
: QThread(parent), mSocketDescriptor(socketDescriptor), mRoot(root)
, mTypesInfo(info), mCounter(id)
{
  dbg;
}

void QRealRepoServerThread::run()
{
  dbg;
  QTcpSocket tcpSocket;
  
  if (!tcpSocket.setSocketDescriptor(mSocketDescriptor)) {
    emit error(tcpSocket.error());
    return;
  }
  
  while (tcpSocket.state() != QAbstractSocket::UnconnectedState ) {
    
    //bool res =
    tcpSocket.waitForReadyRead();
    //qDebug() << "ready - " << res;
    
    while (tcpSocket.bytesAvailable() == 0
    && tcpSocket.state() != QAbstractSocket::UnconnectedState) {
      sleep(1);
      qDebug() << "server is sleeping. state is " << tcpSocket.state();
      tcpSocket.waitForReadyRead();
    }
    
    if (tcpSocket.state() == QAbstractSocket::UnconnectedState)
      break;
    
    QByteArray rawdata = tcpSocket.readAll();
    QString data = QString(rawdata);
    
    handleCommand(data, &tcpSocket);
  }
  
}

void QRealRepoServerThread::handleCommand(QString const &data
, QTcpSocket *const socket)
{
  dbg;
  QString log = "";
  int cmd = data.section("\t", 0, 0).toInt();
  log += QString("cmd: %1").arg(cmd);
  QString resp = "";
  
  // TODO: Refactor this into many small methods.
  switch (cmd) {
    case CMD_SET_NAME:
    {
      int type = data.section("\t", 1, 1).toInt();
      int id = data.section("\t", 2, 2).toInt();
      QString name = data.section("\t", 3, 3);
      if (mTypesInfo->analyseType(type) == TYPE_OBJECT) {
        if (Object *obj = mRoot->getObject(id))
          obj->setName(name);
      } else if ( mTypesInfo->analyseType(type) == TYPE_LINK ){
        if( Link *link = mRoot->getLink(id) )
          link->setName(name);
      } else
        qDebug() << "unknown element's name set requesetd, id " << id;
      resp = QString::number(STATUS_OK);
      log += QString(", new %1's name is %2").arg(id).arg(name);
      break;
    }
    case CMD_CREATE_ENTITY:
    {
      int type = data.section("\t", 1, 1).toInt();
      int id = ++mCounter;
      int parent = data.section("\t", 3, 3).toInt();
      log += QString(", id: %1, type: %2, parent: %3 ").arg(id).arg(type).arg(parent);
      QString name = data.section("\t", 2, 2);
      if( mTypesInfo->analyseType(type) == TYPE_OBJECT ){
        Object *obj = new Object(id, type, 0, 0);
        obj->setName(name);
	obj->setParent(parent);
        mRoot->addObject(id, obj);
        log += QString(", object created, name %1").arg(name);
      } else if( mTypesInfo->analyseType(type) == TYPE_LINK ){
        Link *link = new Link(id, type);
        link->setName(name);
	link->setParent(parent);
        mRoot->addLink(id, link);
        log += QString(", link created, name %1").arg(name);
      }
      if( Object *obj = mRoot->getObject(parent) ){
        obj->addChild(id);
      }
      
      mTypesInfo->elementCreated(type, id);
      resp = QString::number(id);
      break;
    }
    case CMD_GET_TYPES_COUNT:
    {
      resp = QString::number(mTypesInfo->getTypesCount());
      log += QString(", sending types count: %1").arg(resp);
      break;
    }
    case CMD_GET_ALL_TYPES: // TODO: replace with typesInfo call or something like that
    {
      int count = mTypesInfo->getTypesCount();
      for( int i=1; i<=count; i++ )
        resp += QString("%1\t").arg(i);
      log += QString(", sending types count: %1").arg(resp);
      break;
    }
    case CMD_GET_TYPE_INFO:
    {
      int id = data.section("\t", 1, 1).toInt();
      resp = mTypesInfo->getTypeInfo(id).toString();
      log += QString(", sending type info: [%1]").arg(resp);
      break;
    }
    case CMD_GET_TYPE_BY_NAME:
    {
      QString name = data.section("\t", 1, 1);
      resp = mTypesInfo->getTypeInfo(name).toString();
      log += QString(", sending type info: [%1]").arg(resp);
      break;
    }
    case CMD_GET_TYPES_BY_METATYPE:
    {
      MetaType metatype = (MetaType) data.section("\t", 1, 1).toInt();
      resp = mTypesInfo->getTypesByMetatype(metatype);
      log += QString(", sending types list: [%1]").arg(resp);
      break;
    }
    case CMD_GET_OBJECTS_BY_TYPE:
    {
      int type = data.section("\t", 1, 1).toInt();
      if( mTypesInfo->analyseType(type) == TYPE_OBJECT ){
        resp = mRoot->getObjectsByType(type);
      }
      else if( mTypesInfo->analyseType(type) == TYPE_LINK )	{
        resp = mRoot->getLinksByType(type);
      }
      log += QString(", sending objects of type: %1 -- [%2]").arg(type).arg(resp);
      break;
    }
    case CMD_GET_OBJECT_DATA:
    {
      int id = data.section("\t", 1, 1).toInt();
      int childCount = 0;
      int type = -1;
      QString name;
      QString res = "%1\t%2\t%3\t%4\t%5\t";
      resp = "\t";
      if( Object *obj = mRoot->getObject(id) ){
        name = obj->getName();
        childCount = obj->childrenCount();
        type = obj->getType();
      } else	if( Link *link = mRoot->getLink(id) ){
        name = link->getName();
        type = link->getType();
      } else
        qDebug() << "unknown element's data requested, id " << id;
      
      if( type != -1 ){
        RealType info = mTypesInfo->getTypeInfo(type);
        resp = res.arg(id).arg(name).arg(type).arg(info.getDescription()).arg(childCount);
      }
      log += QString(", sending object data: [%1]").arg(resp);
      break;
    }
    case CMD_GET_CHILDREN:
    {
      int type = data.section("\t", 1, 1).toInt();
      int id = data.section("\t", 2, 2).toInt();
      if ( mTypesInfo->analyseType(type) == TYPE_OBJECT ){
        Object *obj = mRoot->getObject(id);
        if( obj )
          resp = obj->childrenToString();
      }
      log += QString(", sending %1's children - [%2]").arg(id).arg(resp);
      break;
    }
    case CMD_GET_DESCRIPTION:
    {
      int type = data.section("\t", 1, 1).toInt();
      int id = data.section("\t", 2, 2).toInt();
      resp = "\t";
      if ( mTypesInfo->analyseType(type) == TYPE_OBJECT ){
        if( Object *obj = mRoot->getObject(id) )
          resp = obj->getDescription();
        else
          qDebug() << "incorrect object requested. id" << id;
      } else if ( mTypesInfo->analyseType(type) == TYPE_LINK ){
        if( Link *link = mRoot->getLink(id) )
          resp = link->getDescription();
	else
	  qDebug() << "incorrect link requested. id" << id;
      }
      log += QString(", sending description for id %1 - [%2]").arg(id).arg(resp);
      break;
    }
    case CMD_SET_DESCRIPTION:
    {
      int type = data.section("\t", 1, 1).toInt();
      int id = data.section("\t", 2, 2).toInt();
      QString desc = data.section("\t", 3, 3);
      resp = "\t";
      if ( mTypesInfo->analyseType(type) == TYPE_OBJECT ){
        if( Object *obj = mRoot->getObject(id) )
          obj->setDescription(desc);
        else
          qDebug() << "incorrect object requested. id" << id;
      } else if ( mTypesInfo->analyseType(type) == TYPE_LINK ){
        if( Link *link = mRoot->getLink(id) )
          link->setDescription(desc);
	else
	  qDebug() << "incorrect link requested. id" << id;
      }
      log += QString(", new description for id %1 - [%2]").arg(id).arg(desc);
      break;
    }
    case CMD_GET_POSITION:
    {
      int type = data.section("\t", 1, 1).toInt();
      int id = data.section("\t", 2, 2).toInt();
      resp = "\t";
      if ( mTypesInfo->analyseType(type) == TYPE_OBJECT ){
        if( Object *obj = mRoot->getObject(id) )
          resp = QString("%1;%2").arg(obj->getX()).arg(obj->getY());
        else
          qDebug() << "incorrect object requested. id" << id;
      } else if ( mTypesInfo->analyseType(type) == TYPE_LINK ){
        if( Link *link = mRoot->getLink(id) )
          resp = link->getPosition();
      }
      log += QString(", sending position for obj %1 - [%2]").arg(id).arg(resp);
      break;
    }
    case CMD_SET_POSITION:
    {
      int type = data.section("\t", 1, 1).toInt();
      int id = data.section("\t", 2, 2).toInt();
      int x = data.section("\t", 3, 3).toInt();
      int y = data.section("\t", 4, 4).toInt();
      if ( mTypesInfo->analyseType(type) == TYPE_OBJECT ){
        if( Object *obj = mRoot->getObject(id) ){
          obj->setX(x);
          obj->setY(y);
        }
      } else if ( mTypesInfo->analyseType(type) == TYPE_LINK ){
        if( Link *link = mRoot->getLink(id) ){
          link->setPosition(QString("%1;%2").arg(x).arg(y));
          qDebug() << link->getPosition();
        }
      }
      
      log += QString(", new %1's position - (%2:%3)").arg(id).arg(x).arg(y);
      resp = QString::number(STATUS_OK);
      break;
    }
    case CMD_GET_CONFIGURATION:
    {
      int type = data.section("\t", 1, 1).toInt();
      int id = data.section("\t", 2, 2).toInt();
      resp = "\t";
      if ( mTypesInfo->analyseType(type) == TYPE_OBJECT ){
        if( Object *obj = mRoot->getObject(id) )
          resp = obj->getConfiguration();
        else
          qDebug() << "error -- cannot find object " << id;
      } else if ( mTypesInfo->analyseType(type) == TYPE_LINK ){
        //	qDebug() << "warning - get conf for link requested";
        if( Link *link = mRoot->getLink(id) )
          resp = link->getConfiguration();
        else
          qDebug() << "error -- cannot find link " << id;
      }
      log += QString(", sending conf for %1 - [%2] ").arg(id).arg(resp);
      break;
    }
    case CMD_SET_CONFIGURATION:
    {
      int type = data.section("\t", 1, 1).toInt();
      int id = data.section("\t", 2, 2).toInt();
      QString conf = data.section("\t", 3, 3);
      if ( mTypesInfo->analyseType(type) == TYPE_OBJECT ){
        if( Object *obj = mRoot->getObject(id) )
          obj->setConfiguration(conf);
      } else 	if ( mTypesInfo->analyseType(type) == TYPE_LINK ){
        if( Link *link = mRoot->getLink(id) )
          link->setConfiguration(conf);
      }
      resp = QString::number(STATUS_OK);
      log += QString(", conf %1").arg(conf);
      break;
    }
    case CMD_SET_PROPERTY:
    {
      int type = data.section("\t", 1, 1).toInt();
      int id = data.section("\t", 2, 2).toInt();
      QString name = data.section("\t", 3, 3);
      QString val = data.section("\t", 4, 4);
      qDebug() << "set property" << name << val;
      
      if ( mTypesInfo->analyseType(type) == TYPE_OBJECT ){
        if( Object *obj = mRoot->getObject(id) ){
          obj->setProperty(name, val);
        }
      } else if ( mTypesInfo->analyseType(type) == TYPE_LINK ){
        if( Link *link = mRoot->getLink(id) ){
          link->setProperty(name, val);
          if( name == "from"){
            if( Object *obj = mRoot->getObject(val.toInt()) ){
              	obj->addLink(id, OUTCOMING_LINK);
                link->addObjectFrom(val.toInt());		
	    }	  
	  } else if( name == "to" ){
	    if( Object *obj = mRoot->getObject(val.toInt()) ){
	    	obj->addLink(id, INCOMING_LINK);
		link->addObjectTo(val.toInt());
	    }
	  }
        }
      }
      resp = QString::number(STATUS_OK);
      log += QString(", new property value: %1 - %2").arg(name).arg(val);
      break;
    }
    case CMD_GET_PROPERTY:
    {
      int type = data.section("\t", 1, 1).toInt();
      int id = data.section("\t", 2, 2).toInt();
      QString name = data.section("\t", 3, 3);
      resp = "\t";
      if ( mTypesInfo->analyseType(type) == TYPE_OBJECT ){
        if( Object *obj = mRoot->getObject(id) )
          resp = obj->getProperty(name);
      } else if ( mTypesInfo->analyseType(type) == TYPE_LINK ){
        if( Link *link = mRoot->getLink(id) )
          resp = link->getProperty(name);
      }
      log += QString(", sent property value: %1 - %2").arg(name).arg(resp);
      break;
    }
    case CMD_ADD_LINK:
    {
      int type = data.section("\t", 1, 1).toInt();
      int id = data.section("\t", 2, 2).toInt();
      int link_id = data.section("\t", 3, 3).toInt();
      int dir = data.section("\t", 4, 4).toInt();
      qDebug() << "dir: " << dir;
      if( Object *obj = mRoot->getObject(id) ){
        qDebug() << "a1";
        obj->addLink(link_id, dir);
        qDebug() << "a2";
        if( Link *link = mRoot->getLink(link_id) ){
          if( dir == INCOMING_LINK )
            link->addObjectTo(id);
	  else if( dir == OUTCOMING_LINK )
            link->addObjectFrom(id);
	  } else
	  	qDebug() << "incorrect link requested. id: " << id;
      } else 
      	qDebug() << "incorrect object requested. type: " << type << ", id: " << id;
      
      log += QString(", added new link: %1").arg(link_id);
      resp = QString::number(STATUS_OK);
      break;
    }
    case CMD_GET_ENTIRE_OBJECT:
    {
//      int type = data.section("\t", 1, 1).toInt();
      int id = data.section("\t", 1, 1).toInt();
      resp = "\t";
  //    if ( mTypesInfo->analyseType(type) == TYPE_OBJECT ){
       if( Object *obj = mRoot->getObject(id) )
          resp = obj->toString();
     // } else if ( mTypesInfo->analyseType(type) == TYPE_LINK ){
      else if( Link *link = mRoot->getLink(id) )
          resp = link->toString();
      //}
      log += QString(", sending object %1: %2").arg(id).arg(resp);
      break;
    }
    case CMD_GET_LINKS_BY_OBJECT:
    {
      int type = data.section("\t", 1, 1).toInt();
      int id = data.section("\t", 2, 2).toInt();
      int dir = data.section("\t", 3, 3).toInt();
      resp = "\t";
      if ( mTypesInfo->analyseType(type) == TYPE_OBJECT ){
        if( Object *obj = mRoot->getObject(id) )
	  if( dir == INCOMING_LINK )
            resp = obj->getIncomingLinks();
	  else if( dir == OUTCOMING_LINK )
	    resp = obj->getOutcomingLinks();
	    
      } else
        qDebug() << "bad object id";
      log += QString(", sending object's links %1: %2").arg(id).arg(resp);
      break;
    }
    case CMD_GET_OBJECTS_BY_LINK:
    {
      int type = data.section("\t", 1, 1).toInt();
      int id = data.section("\t", 2, 2).toInt();
      resp = "\t";
      if ( mTypesInfo->analyseType(type) == TYPE_LINK ){
        if( Link *link = mRoot->getLink(id) )
          resp = link->getObjects();
      } else
        qDebug() << "bad link id";
      log += QString(", sending link's objects %1: %2").arg(id).arg(resp);
      break;
    }
    
    default:
      resp = QString::number(STATUS_OK);
  }
  
  socket->write(resp.toUtf8());
  qDebug() << log;
  
}

int QRealRepoServerThread::getId()
{
  return mCounter++;
}
