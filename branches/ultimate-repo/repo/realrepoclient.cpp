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
	socket->waitForDisconnected();
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

int RealRepoClient::setName( int type, int id, QString name )
{
dbg;
	QString data = QString("%1\t%2\t%3\t%4\t").arg(CMD_SET_NAME).arg(type).arg(id).arg(name);
	return sendData(data).toInt();
}

void RealRepoClient::setPosition( int type, int id, int /*parent*/, int x, int y)
{
dbg;
	QString data = QString("%1\t%2\t%3\t%4\t%5\t").arg(CMD_SET_POSITION).arg(type).arg(id).arg(x).arg(y);
	QString resp = sendData(data);
//	qDebug() << "recvd" << resp;
}

int RealRepoClient::setPropValue( int type, int id, QString name, QString value)
{
dbg;
	QString data = QString("%1\t%2\t%3\t%4\t%5\t").arg(CMD_SET_PROPERTY).arg(type).arg(id).arg(name).arg(value);
	QString resp = sendData(data);
	return resp.toInt();
}

QString RealRepoClient::getPropValue( int type, int id, QString name )
{
dbg;
	QString data = QString("%1\t%2\t%3\t%4\t%5\t").arg(CMD_GET_PROPERTY).arg(type).arg(id).arg(name);
	QString resp = sendData(data);
	return resp;
}

int RealRepoClient::createEntity(int type, QString name)
{
dbg;
	QString data = QString("%1\t%2\t%3\t").arg(CMD_CREATE_ENTITY).arg(type).arg(name);
//	qDebug() << "requesting for" << type << name;
	QString resp = sendData(data);
	return resp.toInt();
//	qDebug() << "recvd" << resp;
}

int RealRepoClient::createEntity(int type, QString name, int parent)
{
dbg;
	QString data = QString("%1\t%2\t%3\t%4\t").arg(CMD_CREATE_ENTITY).arg(type).arg(name).arg(parent);
	QString resp = sendData(data);
	return resp.toInt();
//	qDebug() << "recvd" << resp;
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
	return list;	
}

QString RealRepoClient::getObjectData(int id )
{
	QString cmd = QString("%1\t%2").arg(CMD_GET_OBJECT_DATA).arg(id);
	QString resp = sendData(cmd);
	return resp;
}

QString RealRepoClient::getChildren( int type, int id )
{
	QString cmd = QString("%1\t%2\t%3").arg(CMD_GET_CHILDREN).arg(type).arg(id);
	QString resp = sendData(cmd);
	return resp;	
}

QString RealRepoClient::getPosition( int type, int id )
{
	QString cmd = QString("%1\t%2\t%3").arg(CMD_GET_POSITION).arg(type).arg(id);
	QString resp = sendData(cmd);
	return resp;	
}

int RealRepoClient::setPosition(int type, int id, int x, int y )
{
	QString cmd = QString("%1\t%2\t%3\t%4\t%5").arg(CMD_SET_POSITION).arg(type).arg(id).arg(x).arg(y);
	QString resp = sendData(cmd);
	return resp.toInt();	
}

int RealRepoClient::setConfiguration( int type, int id, QString conf)
{
dbg;
	QString cmd = QString("%1\t%2\t%3\t%4\t").arg(CMD_SET_CONFIGURATION).arg(type).arg(id).arg(conf);
	QString resp = sendData(cmd);
	return resp.toInt();	
}

QString RealRepoClient::getConfiguration( int type, int id)
{
dbg;
	QString cmd = QString("%1\t%2\t%3\t").arg(CMD_GET_CONFIGURATION).arg(type).arg(id);
	QString resp = sendData(cmd);
	return resp;	
}

QString RealRepoClient::getEntireObject( int type, int id )
{
dbg;
	QString cmd = QString("%1\t%2\t%3\t").arg(CMD_GET_ENTIRE_OBJECT).arg(type).arg(id);
	QString resp = sendData(cmd);
	return resp;	
}

RealObject* RealRepoClient::getObjectById( int type, int id )
{
dbg;
	QString data = getEntireObject(type,id);
	RealObject *obj = new RealObject(); // really awful way to do that. need to think it over
	// TODO: add RealObject( const QString& ) constructor to make it creat itself
	obj->setTypeId(data.section("\t",0,0).toInt());
	obj->setId(data.section("\t",1,1).toInt());
	obj->setVisibility(true);
	obj->setName(data.section("\t",3,3));
	obj->setConfiguration(data.section("\t",4,4));

	int childCount = data.section("\t",5,5).toInt();
	int counter = 6;	
	for( int i=0; i<childCount; i++){
		obj->addChildElement(data.section("\t",counter,counter).toInt());
		counter++;
	}	
	
	int linksCount = data.section("\t",counter,counter).toInt();
	counter++;
	for( int i=0; i<linksCount; i++){
		obj->addLink(data.section("\t",counter,counter).toInt());
		counter++;
	}
	
	int propsCount = data.section("\t",counter,counter).toInt();
	counter++;
	for( int i=0; i<propsCount; i++ ){
		QString pair = data.section("\t",counter,counter);
		obj->setProperty(pair.section(";",0,0), pair.section(";",1,1));
		counter++;
	}
	return obj; // implement copy constructor
}

RealLink* RealRepoClient::getLinkById( int type, int id )
{
dbg;
	QString data = getEntireObject(type,id);
	RealLink *link = new RealLink(); // really awful way to do that. need to think it over
	// TODO: add RealLink( const QString& ) constructor to make it creat itself
	link->setTypeId(data.section("\t",0,0).toInt());
	link->setId(data.section("\t",1,1).toInt());
	link->setName(data.section("\t",3,3));

	int fromCount = data.section("\t",4,4).toInt();
	int counter = 5;
	if( fromCount > 0 )
		link->setFromId(data.section("\t",counter,counter).toInt());
	else 	
		link->setFromId(-1);
	counter += fromCount;	
	
	int toCount = data.section("\t",counter,counter).toInt();
	counter++;
	if( toCount > 0 )
		link->setToId(data.section("\t",counter,counter).toInt());
	else 	
		link->setToId(-1);
	counter += toCount;	
	
	int propsCount = data.section("\t",counter,counter).toInt();
	counter++;
	for( int i=0; i<propsCount; i++ ){
		QString pair = data.section("\t",counter,counter);
		link->setProperty(pair.section(";",0,0), pair.section(";",1,1));
		counter++;
	}
	return link; // implement copy constructor
}

QString RealRepoClient::getLinksByObject( int type, int id )
{
dbg;
	QString cmd = QString("%1\t%2\t%3\t").arg(CMD_GET_LINKS_BY_OBJECT).arg(type).arg(id);
	QString resp = sendData(cmd);
	return resp;	
}

QString RealRepoClient::getObjectsByLink( int type, int id )
{
dbg;
	QString cmd = QString("%1\t%2\t%3\t").arg(CMD_GET_OBJECTS_BY_LINK).arg(type).arg(id);
	QString resp = sendData(cmd);
	return resp;	
}

QIntList RealRepoClient::getTypesByMetatype( const MetaType arg )
{
dbg;
	QString cmd = QString("%1\t%2\t").arg(CMD_GET_TYPES_BY_METATYPE).arg(arg);
	QString resp = sendData(cmd);
	QIntList list;
	foreach( QString str, resp.split('\t') )
		list += str.toInt();
	return list;	
}

RealType* RealRepoClient::getTypeById( const int id )
{
dbg;
	QString cmd = QString("%1\t%2").arg(CMD_GET_TYPE_INFO).arg(id);
	QString data = sendData(cmd);
	RealType * type = new RealType();
	// FIXME: add RealType( const QString& ) constructor to make it create itself
	type->loadFromString(data);

	return type;	 // that suxx really hard :D
}

