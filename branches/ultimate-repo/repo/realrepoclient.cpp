/** @file realrepoclient.cpp
 * 	@brief API для доступа к репозиторию
 * */
#include "realrepoclient.h"
//#define _LONG_DEBUG
#include "dbg.h"

#include <QDataStream>
#include <QByteArray>
#include <QStringList>

using namespace qRealTypes;

RealRepoClient::RealRepoClient(const QString &addr, const int port, QObject *parent) : QObject(parent)
{
dbg;
	socket = new QTcpSocket(this);
	connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));

	socket->abort();
	m_error = -1;
	socket->connectToHost(addr, port);
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

QString RealRepoClient::sendData(QString const &data)
{
dbg;
	if (socket->state() != QAbstractSocket::ConnectedState)
		return "";

	//QString data = QString("%1\t%2\t%3\t%4\t").arg(CMD_CREATE_ENTITY).arg(type).arg(id).arg(name);
//	qDebug() << "[CLIENT]: sending" << data;
	//int bytes =
	socket->write(data.toUtf8());
//	qDebug() << "written " << bytes << " bytes";
	//bool res =
	socket->waitForReadyRead();
//	qDebug() << "ready - " << res;
	QByteArray req = socket->readAll();
//	qDebug() << "[CLIENT]: recvd" << req;
	return extractData(QString::fromUtf8(req));
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

QString RealRepoClient::extractData(QString const &resp)
{
	QString errorCode = resp.section('\t', 0, 0);
	bool errCodeCorrect = false;
	if (errorCode.toInt(&errCodeCorrect) != ERR_STATUS_OK || !errCodeCorrect)
	{
		throw errorCode;  // TODO: Throw something more appropriate.
	} else
	{
		return resp.section('\t', 1);
	}
}

QString RealRepoClient::getName(IdType const &id)
{
dbg;
	QString data = QString("%1\t%2\t").arg(CMD_GET_NAME).arg(id);
	return sendData(data);
}

int RealRepoClient::setName(IdType const &id, QString name)
{
dbg;
	QString data = QString("%1\t%2\t%3\t").arg(CMD_SET_NAME).arg(id).arg(name);
	return sendData(data).toInt();
}

void RealRepoClient::setPosition(IdType const &id, IdType const &parent, int x, int y)
{
dbg;
	QString data = QString("%1\t%2\t%3\t%4\t%5\t").arg(CMD_SET_POSITION).arg(id).arg(parent).arg(x).arg(y);
	QString resp = sendData(data);
//	qDebug() << "recvd" << resp;
}

void RealRepoClient::setDescription(IdType const &id, QString desc)
{
dbg;
	QString data = QString("%1\t%2\t%3\t").arg(CMD_SET_DESCRIPTION).arg(id).arg(desc);
	QString resp = sendData(data);
}

QString RealRepoClient::getDescription(IdType const &id)
{
dbg;
	QString data = QString("%1\t%2\t").arg(CMD_GET_DESCRIPTION).arg(id);
	QString resp = sendData(data);
	return resp;
}

int RealRepoClient::setPropValue(IdType const &id, QString name, QString value)
{
dbg;
	QString data = QString("%1\t%2\t%3\t%4\t").arg(CMD_SET_PROPERTY).arg(id).arg(name).arg(value);
	QString resp = sendData(data);
	return resp.toInt();
}

QString RealRepoClient::getPropValue(IdType const &id, QString name)
{
dbg;
	QString data = QString("%1\t%2\t%3\t").arg(CMD_GET_PROPERTY).arg(id).arg(name);
	QString resp = sendData(data);
	return resp;
}

IdType RealRepoClient::createObject(TypeIdType const &type, QString name)
{
dbg;
	qDebug() << "creating a new entity: " << type << name;
	QString data = QString("%1\t%2\t%3\t").arg(CMD_CREATE_ENTITY).arg(type).arg(name);
	QString resp = sendData(data);
	return resp;
}

IdType RealRepoClient::copyEntity(TypeIdType const &type, IdType const &id, IdType const &newParent, IdType const &oldParent, bool full)
{
dbg;
	QString data;
	if (!full)
		data = QString("%1\t%2\t%3\t%4\t%5\t").arg(CMD_COPY_ENTITY).arg(type).arg(id).arg(newParent).arg(oldParent);
	else
		data = QString("%1\t%2\t%3\t%4\t%5\t").arg(CMD_FULLCOPY_ENTITY).arg(type).arg(id).arg(newParent).arg(oldParent);
	QString resp = sendData(data);
	return resp;
}

IdType RealRepoClient::createObjectWithParent(TypeIdType const &type, QString name, IdType const &parent)
{
dbg;
	qDebug() << "creating a new entity: " << type << name << parent;
	QString data = QString("%1\t%2\t%3\t%4\t").arg(CMD_CREATE_ENTITY).arg(type).arg(name).arg(parent);
	QString resp = sendData(data);
	return resp;
//	qDebug() << "recvd" << resp;
}

IdType RealRepoClient::createLink(QString name)
{
	return createObject("krneDirRelationship", name);
}

IdType RealRepoClient::createLinkWithType(QString name, QString type)
{
	IdType typeId = getTypeIdByName(type);
	qDebug() << ">>>>type: "<< typeId;
	return createObject(typeId, name);
}

IdType RealRepoClient::createLinkWithParent(TypeIdType const &type, QString name, IdType const &parent)
{
	return createObjectWithParent(type, name, parent);
}

void RealRepoClient::deleteObject(IdType const &id, IdType const &parent)
{
dbg;
	QString data = QString("%1\t%2\t%3\t").arg(CMD_DELETE_ENTITY).arg(id).arg(parent);
	QString resp = sendData(data);
}

void RealRepoClient::deleteObject(IdType const &id)
{
	IdTypeList parents = getParents(id);
	foreach (IdType parent, parents) {
		deleteObject(id, parent);
	}
}

void RealRepoClient::deleteLink(IdType const &id, IdType const &parent)
{
	deleteObject(id, parent);
}

void RealRepoClient::deleteLink(IdType const &id)
{
	deleteObject(id);
}

int RealRepoClient::getTypesCount()
{
dbg;
	return sendData(QString::number(CMD_GET_TYPES_COUNT)).toInt();
}

TypeIdTypeList RealRepoClient::getAllTypes()
{
dbg;
	QString res = sendData(QString::number(CMD_GET_ALL_TYPES));
	TypeIdTypeList list;
	foreach (QString str, res.split('\t'))
		list += str;
	list.removeLast(); // WTF???
	return list;
}

#if 0 // see header file.
TypeInfo RealRepoClient::getTypeInfo(TypeIdType const &type)
{
dbg;
	TypeInfo info;
	QString cmd = QString("%1\t%2").arg(CMD_GET_TYPE_INFO).arg(type);
	QString resp = sendData(cmd);
	IdType const &id = resp.section("\t", 0, 0);

	if( id == INVALID_ID ){
		// handle error
		// return info;
	}
//	qDebug() << "recvd info " << resp;
	info.fromString(resp);
	return info;
}
#endif

int RealRepoClient::getLastError()
{
dbg;
	return m_error;
}

QString RealRepoClient::getObjectsByType(TypeIdType const &type)
{
	QString cmd = QString("%1\t%2").arg(CMD_GET_OBJECTS_BY_TYPE).arg(type);
	QString resp = sendData(cmd);
	return resp;
}

IdTypeList RealRepoClient::getObjectsListByType(TypeIdType const &type)
{
	QString resp = getObjectsByType(type);

	IdTypeList list;
	foreach (QString str, resp.split('\t', QString::SkipEmptyParts))
		list += str;
	if (resp == "\t")
		list.clear();
	return list;
}

IdTypeList RealRepoClient::getLinks()
{
	// TODO: гм... а) Хотелось бы возвращать не только krneDirRelationship,
	// но и всех потомков krneDirRelationship
	// б) А кто сказал, что в подключённых плагинах krneDirRelationship есть?
	return getObjectsListByType(getTypeIdByName("krneDirRelationship"));
}

QString RealRepoClient::getObjectData(IdType const &id)
{
	QString cmd = QString("%1\t%2").arg(CMD_GET_OBJECT_DATA).arg(id);
	QString resp = sendData(cmd);
	return resp;
}

QString RealRepoClient::getChildren(IdType const &id)
{
	QString cmd = QString("%1\t%2\t").arg(CMD_GET_CHILDREN).arg(id);
	QString resp = sendData(cmd);
	return resp;
}

IdTypeList RealRepoClient::getParents(IdType const &id)
{
	QString cmd = QString("%1\t%2\t").arg(CMD_GET_CONTAINERS).arg(id);
	QString resp = sendData(cmd);
	IdTypeList list;
	foreach (QString str, resp.split('\t', QString::SkipEmptyParts))
		list += str;
	return list;
}

QString RealRepoClient::getPosition(IdType const &id, IdType const &parent)
{
	QString cmd = QString("%1\t%2\t%3\t").arg(CMD_GET_POSITION).arg(id).arg(parent);
	QString resp = sendData(cmd);
	return resp;
}

int RealRepoClient::setConfiguration(IdType const &id, IdType const &parent, QString conf)
{
dbg;
	QString cmd = QString("%1\t%2\t%3\t%4\t").arg(CMD_SET_CONFIGURATION).arg(id).arg(parent).arg(conf);
	QString resp = sendData(cmd);
	return resp.toInt();
}

QString RealRepoClient::getConfiguration(IdType const &id, IdType const &parent)
{
dbg;
	QString cmd = QString("%1\t%2\t%3\t").arg(CMD_GET_CONFIGURATION).arg(id).arg(parent);
	QString resp = sendData(cmd);
	return resp;
}

QString RealRepoClient::getEntireObject(IdType const &id)
{
dbg;
	//QString cmd = QString("%1\t%2\t%3\t").arg(CMD_GET_ENTIRE_OBJECT).arg(type).arg(id);
	QString cmd = QString("%1\t%2\t").arg(CMD_GET_ENTIRE_OBJECT).arg(id);
	QString resp = sendData(cmd);
	return resp;
}

RealObject RealRepoClient::getObjectById(IdType const &id)
{
dbg;
	QString data = getEntireObject(id);
	RealObject obj;
	obj.setTypeId(data.section("\t", 0, 0));
	obj.setId(id);
	obj.setVisibility(true);
	obj.setName(data.section("\t", 2, 2));
	obj.setDescription(data.section("\t", 3, 3));

	int childCount = data.section("\t", 4, 4).toInt();
	int counter = 5;
	for (int i = 0; i < childCount; ++i) {
		obj.addChildElement(data.section("\t", counter, counter));
		++counter;
	}

	int incLinksCount = data.section("\t", counter, counter).toInt();
	++counter;
	for (int i = 0; i < incLinksCount; ++i) {
		obj.addIncomingLink(data.section("\t", counter, counter));
		++counter;
	}

	int outcLinksCount = data.section("\t", counter, counter).toInt();
	++counter;
	for (int i = 0; i < outcLinksCount; ++i) {
		obj.addOutcomingLink(data.section("\t", counter, counter));
		++counter;
	}

	int propsCount = data.section("\t", counter, counter).toInt();
	++counter;
	for (int i = 0; i < propsCount; ++i) {
		QString pair = data.section("\t", counter, counter);
		obj.setProperty(pair.section(";", 0, 0), pair.section(";", 1, 1));
		++counter;
	}
	obj.setRepoClient(this);
	return obj;
}

RealLink RealRepoClient::getLinkById(IdType const &id)
{
dbg;
	QString data = getEntireObject(id);
	RealLink link;
	link.setTypeId(data.section("\t", 0, 0));
	link.setId(data.section("\t", 1, 1));
	link.setName(data.section("\t", 3, 3));
	link.setDescription(data.section("\t", 4, 4));

	IdType fromId = data.section("\t", 5, 5);
	int counter = 6;
	link.setFromId(fromId);

	IdType toId = data.section("\t",counter,counter);
	counter++;
	link.setToId(toId);

	int propsCount = data.section("\t",counter,counter).toInt();
	counter++;
	for (int i=0; i < propsCount; i++) {
		QString pair = data.section("\t", counter, counter);
		link.setProperty(pair.section(";", 0, 0), pair.section(";", 1, 1));
		++counter;
	}
	link.setRepoClient(this);
	return link;
}

QString RealRepoClient::getLinksByObject(IdType const &id, int direction)
{
dbg;
	QString cmd = QString("%1\t%2\t%3\t").arg(CMD_GET_LINKS_BY_OBJECT).arg(id).arg(direction);
	QString resp = sendData(cmd);
	return resp;
}

QString RealRepoClient::getObjectsByLink(IdType const &id)
{
dbg;
	QString cmd = QString("%1\t%2\t").arg(CMD_GET_OBJECTS_BY_LINK).arg(id);
	QString resp = sendData(cmd);
	return resp;
}

TypeIdTypeList RealRepoClient::getTypesByMetaType(const MetaType arg)
{
dbg;
	QString cmd = QString("%1\t%2\t").arg(CMD_GET_TYPES_BY_METATYPE).arg(arg);
	QString resp = sendData(cmd);
	TypeIdTypeList list;
	foreach (QString str, resp.split('\t'))
		if (str != "")
			list += str;
	return list;
}


RealType RealRepoClient::getTypeById(TypeIdType const &id)
{
dbg;
	QString cmd = QString("%1\t%2").arg(CMD_GET_TYPE_INFO).arg(id);
	QString data = sendData(cmd);
	RealType type;
	type.loadFromString(data);

	return type;
}

RealType RealRepoClient::getTypeByName(QString const &name)
{
dbg;
	QString cmd = QString("%1\t%2").arg(CMD_GET_TYPE_BY_NAME).arg(name);
	QString data = sendData(cmd);
	RealType type;
	type.loadFromString(data);

	return type;
}

TypeIdType RealRepoClient::getTypeIdByName(QString const &name)
{
	return getTypeByName(name).getId();
}

void RealRepoClient::addLink(IdType const &obj_id, IdType const &link_id, int direction)
{
dbg;
	QString cmd = QString("%1\t%2\t%3\t%4\t").arg(CMD_ADD_LINK).arg(obj_id).arg(link_id).arg(direction);
	sendData(cmd);
}

void RealRepoClient::removeLink(IdType const &obj_id, IdType const &link_id, int direction)
{
dbg;
	QString cmd = QString("%1\t%2\t%3\t%4\t").arg(CMD_REMOVE_LINK).arg(obj_id).arg(link_id).arg(direction);
	sendData(cmd);
}

IdTypeList RealRepoClient::getAllObjects()
{
	QString cmd = QString("%1\t").arg(CMD_GET_ALL_OBJECTS);
	QString resp = sendData(cmd);
	IdTypeList list;
	foreach (QString str, resp.split('\t', QString::SkipEmptyParts))
		list += str;
	return list;
}
