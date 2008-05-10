#include "realrepoclient.h"
//#define _LONG_DEBUG
#include "dbg.h"
#include <unistd.h>

#include <QDataStream>
#include <QByteArray>
#include <QStringList>

RealRepoClient::RealRepoClient( QObject *parent) : QObject(parent)
{ 
dbg;
	socket = new QTcpSocket(this);
	connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));

	socket->abort();
	blockSize = 0;
	m_error = -1;
	socket->connectToHost("127.0.0.1", 6666);
        if (!socket->waitForConnected(5*1000)) {
//		emit socket->error(socket->error(), socket->errorString());
		qDebug() << "cannot connect to the server" << endl;
            return;
        }
	m_error = socket->error();
}

RealRepoClient::~RealRepoClient()
{
dbg;
	socket->disconnectFromHost();
	if (socket->state() == QAbstractSocket::UnconnectedState ||  socket->waitForDisconnected(1000))
		qDebug("Disconnected!");
}

QString RealRepoClient::sendData( QString data )
{
dbg;
	if( socket->state() != QAbstractSocket::ConnectedState )
		return "";

	//QString data = QString("%1\t%2\t%3\t%4\t").arg(CMD_CREATE_ENTITY).arg(type).arg(id).arg(name);
	qDebug() << "[CLIENT]: sending" << data;
	//int bytes = 
	socket->write(data.toUtf8());	
//	qDebug() << "written " << bytes << " bytes";
	//bool res = 
	socket->waitForReadyRead();
//	qDebug() << "ready - " << res;
	QByteArray req = socket->readAll();
	qDebug() << "[CLIENT]: recvd" << req;
	return QString(req);
}

void RealRepoClient::displayError(QAbstractSocket::SocketError socketError)
{
dbg;
	switch (socketError) {
		case QAbstractSocket::RemoteHostClosedError:
			qDebug() << "QAbstractSocket::RemoteHostClosedError";
			break;
		case QAbstractSocket::ConnectionRefusedError:	
			qDebug() << "ConnectionRefusedError";
			break;
		default:
			qDebug() << socket->errorString();
		break;	
	}
	m_error = socketError;
}

int RealRepoClient::setName( int id, QString name )
{
dbg;
	QString data = QString("%1\t%2\t%3\t").arg(CMD_SET_NAME).arg(id).arg(name);
	return sendData(data).toInt();
}

int RealRepoClient::setParent( int id, int parent )
{
dbg;
	QString data = QString("%1\t%2\t%3\t").arg(CMD_SET_PARENT).arg(id).arg(parent);
	return sendData(data).toInt();
}

void RealRepoClient::setPosition( int id, int /*parent*/, int x, int y)
{
dbg;
	QString data = QString("%1\t%2\t%3\t%4\t").arg(CMD_SET_POSITION).arg(id).arg(x).arg(y);
	QString resp = sendData(data);
//	qDebug() << "recvd" << resp;
}

void RealRepoClient::setDescription( int id, QString desc )
{
dbg;
	QString data = QString("%1\t%2\t%3\t").arg(CMD_SET_DESCRIPTION).arg(id).arg(desc);
	QString resp = sendData(data);
}

QString RealRepoClient::getDescription( int id )
{
dbg;
	QString data = QString("%1\t%2\t").arg(CMD_GET_DESCRIPTION).arg(id);
	QString resp = sendData(data);
	return resp;
}

int RealRepoClient::setPropValue( int id, QString name, QString value)
{
dbg;
	QString data = QString("%1\t%2\t%3\t%4\t").arg(CMD_SET_PROPERTY).arg(id).arg(name).arg(value);
	QString resp = sendData(data);
	return resp.toInt();
}

QString RealRepoClient::getPropValue( int id, QString name )
{
dbg;
	QString data = QString("%1\t%2\t%3\t").arg(CMD_GET_PROPERTY).arg(id).arg(name);
	QString resp = sendData(data);
	return resp;
}

int RealRepoClient::createObject(int type, QString name)
{
dbg;
	QString data = QString("%1\t%2\t%3\t").arg(CMD_CREATE_ENTITY).arg(type).arg(name);
	QString resp = sendData(data);
	return resp.toInt();
}

int RealRepoClient::createObjectWithParent(int type, QString name, int parent)
{
dbg;
	QString data = QString("%1\t%2\t%3\t%4\t").arg(CMD_CREATE_ENTITY).arg(type).arg(name).arg(parent);
	QString resp = sendData(data);
	return resp.toInt();
//	qDebug() << "recvd" << resp;
}

int RealRepoClient::createLink(QString name)
{
	return createObject(getTypeIdByName("krneDirRelationship"), name);
}

void RealRepoClient::deleteObject(int id)
{
dbg;
	QString data = QString("%1\t%2\t").arg(CMD_DELETE_ENTITY).arg(id);
	QString resp = sendData(data);
}

void RealRepoClient::deleteLink(int id)
{
	deleteObject(id);
}


int RealRepoClient::getTypesCount()
{
dbg;
	return sendData(QString::number(CMD_GET_TYPES_COUNT)).toInt();
}

QIntList RealRepoClient::getAllTypes()
{
dbg;
	QString res = sendData(QString::number(CMD_GET_ALL_TYPES));
	QIntList list;
	foreach( QString str, res.split('\t') )
		list += str.toInt();
	list.removeLast();	
	return list;	
}

TypeInfo RealRepoClient::getTypeInfo( int arg )
{
dbg;
	TypeInfo info;
	QString cmd = QString("%1\t%2").arg(CMD_GET_TYPE_INFO).arg(arg);
	QString resp = sendData(cmd);
	int id = resp.section("\t",0,0).toInt();
	
	if( id == INVALID_ID ){
		// handle error
		// return info;
	}
//	qDebug() << "recvd info " << resp;
	info.fromString(resp);
	return info;
}

int RealRepoClient::getLastError()
{
dbg;
	return m_error;
}

QString RealRepoClient::getObjectsByType( int type )
{
	QString cmd = QString("%1\t%2").arg(CMD_GET_OBJECTS_BY_TYPE).arg(type);
	QString resp = sendData(cmd);
	return resp;
}

QIntList RealRepoClient::getObjectsListByType( int type )
{
	QString resp = getObjectsByType(type);

	QIntList list;
	foreach( QString str, resp.split('\t') )
		list += str.toInt();
	if( resp == "\t" )
		list.clear();
	return list;
}

QIntList RealRepoClient::getLinks()
{
	return getObjectsListByType(getTypeIdByName("krneRelationship"));
}


QString RealRepoClient::getObjectData(int id )
{
	QString cmd = QString("%1\t%2").arg(CMD_GET_OBJECT_DATA).arg(id);
	QString resp = sendData(cmd);
	return resp;
}

QString RealRepoClient::getChildren( int id )
{
	QString cmd = QString("%1\t%2\t").arg(CMD_GET_CHILDREN).arg(id);
	QString resp = sendData(cmd);
	return resp;	
}

QString RealRepoClient::getPosition( int id )
{
	QString cmd = QString("%1\t%2\t").arg(CMD_GET_POSITION).arg(id);
	QString resp = sendData(cmd);
	return resp;	
}

int RealRepoClient::setPosition( int id, int x, int y )
{
	QString cmd = QString("%1\t%2\t%3\t%4\t").arg(CMD_SET_POSITION).arg(id).arg(x).arg(y);
	QString resp = sendData(cmd);
	return resp.toInt();	
}

int RealRepoClient::setConfiguration( int id, QString conf)
{
dbg;
	QString cmd = QString("%1\t%2\t%3\t").arg(CMD_SET_CONFIGURATION).arg(id).arg(conf);
	QString resp = sendData(cmd);
	return resp.toInt();	
}

QString RealRepoClient::getConfiguration( int id)
{
dbg;
	QString cmd = QString("%1\t%2\t").arg(CMD_GET_CONFIGURATION).arg(id);
	QString resp = sendData(cmd);
	return resp;	
}

QString RealRepoClient::getEntireObject( int id )
{
dbg;
	//QString cmd = QString("%1\t%2\t%3\t").arg(CMD_GET_ENTIRE_OBJECT).arg(type).arg(id);
	QString cmd = QString("%1\t%2\t").arg(CMD_GET_ENTIRE_OBJECT).arg(id);
	QString resp = sendData(cmd);
	return resp;	
}

RealObject RealRepoClient::getObjectById( int id )
{
dbg;
	QString data = getEntireObject(id);
	RealObject obj;
	obj.setTypeId(data.section("\t",0,0).toInt());
	obj.setId(id);
	obj.setContainerId(data.section("\t",2,2).toInt());
	obj.setVisibility(true);
	obj.setName(data.section("\t",3,3));
	obj.setConfiguration(data.section("\t",4,4));
	obj.setDescription(data.section("\t",7,7));

	int childCount = data.section("\t",8,8).toInt();
	int counter = 9;	
	for( int i=0; i<childCount; i++){
		obj.addChildElement(data.section("\t",counter,counter).toInt());
		counter++;
	}	
	
	int incLinksCount = data.section("\t",counter,counter).toInt();
	qDebug() << "inc links: " << incLinksCount;
	counter++;
	for( int i=0; i<incLinksCount; i++){
		obj.addIncomingLink(data.section("\t",counter,counter).toInt());
		counter++;
	}
	
	int outcLinksCount = data.section("\t",counter,counter).toInt();
	qDebug() << "out links: " << outcLinksCount;
	counter++;
	for( int i=0; i<outcLinksCount; i++){
		obj.addOutcomingLink(data.section("\t",counter,counter).toInt());
		counter++;
	}
	
	int propsCount = data.section("\t",counter,counter).toInt();
	counter++;
	for( int i=0; i<propsCount; i++ ){
		QString pair = data.section("\t",counter,counter);
		obj.setProperty(pair.section(";",0,0), pair.section(";",1,1));
		counter++;
	}
	obj.setRepoClient(this);
	return obj;
}

RealLink RealRepoClient::getLinkById( int id )
{
dbg;
	QString data = getEntireObject(id);
	RealLink link;
	// TODO: add RealLink( const QString& ) constructor to make it creat itself
	link.setTypeId(data.section("\t",0,0).toInt());
	link.setId(data.section("\t",1,1).toInt());
	link.setName(data.section("\t",3,3));
	link.setDescription(data.section("\t",4,4));
	qDebug() << "desc: " << data.section("\t",4,4);

	int fromCount = data.section("\t",5,5).toInt();
	int counter = 6;
	if( fromCount > 0 )
		link.setFromId(data.section("\t",counter,counter).toInt());
	else 	
		link.setFromId(-1);
	counter += fromCount;	
	
	int toCount = data.section("\t",counter,counter).toInt();
	counter++;
	if( toCount > 0 )
		link.setToId(data.section("\t",counter,counter).toInt());
	else 	
		link.setToId(-1);
	counter += toCount;	
	
	int propsCount = data.section("\t",counter,counter).toInt();
	counter++;
	for( int i=0; i<propsCount; i++ ){
		QString pair = data.section("\t",counter,counter);
		link.setProperty(pair.section(";",0,0), pair.section(";",1,1));
		counter++;
	}
	link.setRepoClient(this);
	return link;
}

QString RealRepoClient::getLinksByObject( int id, int direction )
{
dbg;
	QString cmd = QString("%1\t%2\t%3\t").arg(CMD_GET_LINKS_BY_OBJECT).arg(id).arg(direction);
	QString resp = sendData(cmd);
	return resp;	
}

QString RealRepoClient::getObjectsByLink( int id )
{
dbg;
	QString cmd = QString("%1\t%2\t").arg(CMD_GET_OBJECTS_BY_LINK).arg(id);
	QString resp = sendData(cmd);
	return resp;	
}

QIntList RealRepoClient::getTypesByMetaType( const MetaType arg )
{
dbg;
	QString cmd = QString("%1\t%2\t").arg(CMD_GET_TYPES_BY_METATYPE).arg(arg);
	QString resp = sendData(cmd);
	QIntList list;
	foreach( QString str, resp.split('\t') )
		if( str.toInt() != 0 )
			list += str.toInt();
	return list;	
}


RealType RealRepoClient::getTypeById( const int id )
{
dbg;
	QString cmd = QString("%1\t%2").arg(CMD_GET_TYPE_INFO).arg(id);
	QString data = sendData(cmd);
	RealType type;
	type.loadFromString(data);

	return type;	 
}

RealType RealRepoClient::getTypeByName( const QString name ) 
{
dbg;
	QString cmd = QString("%1\t%2").arg(CMD_GET_TYPE_BY_NAME).arg(name);
	QString data = sendData(cmd);
	RealType type;
	type.loadFromString(data);

	return type;	 
}

int RealRepoClient::getTypeIdByName( const QString name ) 
{
	return getTypeByName(name).getId();
}

void RealRepoClient::addLink( int obj_id, int link_id, int direction )
{
dbg;
	QString cmd = QString("%1\t%2\t%3\t%4\t").arg(CMD_ADD_LINK).arg(obj_id).arg(link_id).arg(direction);
	sendData(cmd);
}

void RealRepoClient::removeLink( int obj_id, int link_id, int direction )
{
dbg;
	QString cmd = QString("%1\t%2\t%3\t%4\t").arg(CMD_REMOVE_LINK).arg(obj_id).arg(link_id).arg(direction);
	sendData(cmd);
}

