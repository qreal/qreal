/** @file repothread.cpp
 * 	@brief Поток, обслуживающий клиента репозитория
 * */

#include <qstring.h>

#include "repothread.h"
//#define _LONG_DEBUG
#include "dbg.h"

#include <QtNetwork>
#include <QByteArray>
#include <qstring.h>

#define case_cmdname_to_string(cmd) case cmd: return QString(#cmd); break

static QString cmd_to_string(int cmd)
{
	switch (cmd)
	{
	case_cmdname_to_string(CMD_GET_NAME);
	case_cmdname_to_string(CMD_SET_NAME);
	case_cmdname_to_string(CMD_CREATE_ENTITY);
	case_cmdname_to_string(CMD_COPY_ENTITY);
	case_cmdname_to_string(CMD_REPARENT_ENTITY);
	case_cmdname_to_string(CMD_FULLCOPY_ENTITY);
	case_cmdname_to_string(CMD_DELETE_ENTITY);
	case_cmdname_to_string(CMD_GET_TYPES_COUNT);
	case_cmdname_to_string(CMD_GET_ALL_TYPES);
	case_cmdname_to_string(CMD_GET_TYPE_INFO);
	case_cmdname_to_string(CMD_GET_TYPE_BY_NAME);
	case_cmdname_to_string(CMD_GET_TYPES_BY_METATYPE);
	case_cmdname_to_string(CMD_GET_OBJECTS_BY_TYPE);
	case_cmdname_to_string(CMD_GET_OBJECT_DATA);
	case_cmdname_to_string(CMD_GET_CHILDREN);
	case_cmdname_to_string(CMD_GET_DESCRIPTION);
	case_cmdname_to_string(CMD_SET_DESCRIPTION);
	case_cmdname_to_string(CMD_GET_POSITION);
	case_cmdname_to_string(CMD_SET_POSITION);
	case_cmdname_to_string(CMD_GET_CONFIGURATION);
	case_cmdname_to_string(CMD_SET_CONFIGURATION);
	case_cmdname_to_string(CMD_SET_PROPERTY);
	case_cmdname_to_string(CMD_GET_PROPERTY);
	case_cmdname_to_string(CMD_ADD_LINK);
	case_cmdname_to_string(CMD_REMOVE_LINK);
	case_cmdname_to_string(CMD_GET_ENTIRE_OBJECT);
	case_cmdname_to_string(CMD_GET_LINKS_BY_OBJECT);
	case_cmdname_to_string(CMD_GET_OBJECTS_BY_LINK);
	case_cmdname_to_string(CMD_GET_CONTAINERS);
	case_cmdname_to_string(CMD_GET_ALL_OBJECTS);
	case_cmdname_to_string(CMD_CLEAR_ALL);
	case_cmdname_to_string(CMD_GET_REFERRALS);
	case_cmdname_to_string(CMD_INC_REFERRAL);
	case_cmdname_to_string(CMD_DEC_REFERRAL);
	default:
		break;
	}
	return QString("UNKNOWN!");
}

using namespace repoServer;
using namespace qRealTypes;

QRealRepoServerThread::QRealRepoServerThread(int const &socketDescriptor
	, QObject * const parent, RepoData * const repoData, RepoTypesInfo * const info
	, int const &id)
: QThread(parent), mSocketDescriptor(socketDescriptor), mRepoData(repoData)
, mTypesInfo(info), mCounter(id)
{
	dbg;
}

void QRealRepoServerThread::tryToRestoreState()
{
	// Some means to restore repository contents by reading and applying saved
	// commands. It will work for only 1 client, and IS INTENDED FOR TESTING ONLY!
	QFile file("repothread_log.txt");

	// Works only if repo contains only root object.
	if (file.exists() && mRepoData->getObjectsSize() == 1 && mRepoData->getLinksSize() == 0)
	{
		if (file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			qDebug() << "Command log found, restoring";
			QTextStream savedCommands(&file);
			// QTextStream does not honour setCodecForCStrings(codec).
			// Set UTF8 explicitly.
			savedCommands.setCodec("UTF-8");
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

void QRealRepoServerThread::clearLog()
{
	qDebug() << mLoggedMode << mLogFile.fileName();
	if (mLoggedMode && mLogFile.exists()) {
		mLogFile.close();
		if (!mLogFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
			mLoggedMode = false;
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

	tryToRestoreState();

	// Log of commands - to restore repository state in a next run.
	// Not thread-safe.
	mLoggedMode = true;
	mLogFile.setFileName("repothread_log.txt");
	if (!mLogFile.open(QIODevice::Append | QIODevice::Text))
		mLoggedMode = false;
	QTextStream commandLog(&mLogFile);
	// QTextStream does not honour setCodecForCStrings(codec).
	// Set UTF8 explicitly.
	commandLog.setCodec("UTF-8");

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

		if (mLoggedMode)
			commandLog << data << endl;

		IntQStringPair resp = handleCommand(data);
		QString response = QString::number(resp.first) + '\t' + resp.second;
		tcpSocket.write(response.toUtf8());
	}

	if (mLoggedMode)
		mLogFile.close();
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

IntQStringPair QRealRepoServerThread::ReportError(const int &errorCode)
{
  return IntQStringPair (errorCode, "");
}

IntQStringPair QRealRepoServerThread::ReportSuccess(const QString &data)
{
  return IntQStringPair(ERR_STATUS_OK, data);
}

IntQStringPair QRealRepoServerThread::handleGetName(QStringVector const &params)
{
	if (!IsParamsNumberCorrect(params, "GetName", 1))
		return ReportError(ERR_INCORRECT_PARAMS);

	IdType id = params[0];
	QString resp = "";
	if (Object *obj = mRepoData->getObject(id))
		resp = obj->getName();
	else if (Link *link = mRepoData->getLink(id))
		resp = link->getName();
	else
	{
		qDebug() << "unknown entity's name requested, id " << id;
		return ReportError(ERR_INCORRECT_REQUEST);
	}

	mLog += QString(", sent %1's name: [%2]").arg(id).arg(resp);
	return ReportSuccess(resp);
}

IntQStringPair QRealRepoServerThread::handleSetName(QStringVector const &params)
{
	if (!IsParamsNumberCorrect(params, "SetName", 2))
		return ReportError(ERR_INCORRECT_PARAMS);

	IdType id = params[0];
	QString name = params[1];
	if (Object * obj = mRepoData->getObject(id))
		obj->setName(name);
	else if (Link * link = mRepoData->getLink(id))
		link->setName(name);
	else
	{
		qDebug() << "unknown entity's name set requested, id " << id;
		return ReportError(ERR_INCORRECT_REQUEST);
	}
	mLog += QString(", new %1's name is [%2]").arg(id).arg(name);
	return ReportSuccess("");
}

IntQStringPair QRealRepoServerThread::handleCreateEntity(QStringVector const &params)
{
	qDebug() << params;
	if (!IsParamsNumberCorrect(params, "CreateEntity", 3))
		return ReportError(ERR_INCORRECT_PARAMS);

	TypeIdType type = params[0];
	IdType id = QString::number(++mCounter);
	QString name = params[1];
	IdType parent = params[2];
	mLog += QString(", id: %1, type: %2, parent: %3 ").arg(id).arg(type).arg(parent);
	if (mTypesInfo->analyseType(type) == TYPE_OBJECT){
		Object *obj = new Object(id, type);
		obj->setName(name);
		if (Object * par = mRepoData->getObject(parent))
		{
			par->addNodeChild(id);
			obj->addRef(parent);
		} else {
			// Родителя нет в репозитории. По идее, так может быть только в одном
			// случае - когда родитель не является объектом (например он - метатип),
			// как в случае корневых диаграмм. Тогда родителем станет искусственный
			// объект - корень репозитория.
			Q_ASSERT(parent == 0);
			obj->addRef("repoRoot");
			mRepoData->getRootObject()->addNodeChild(id);
		}
		mRepoData->addObject(id, obj);
		mLog += QString(", object created, name %1").arg(name);
	} else if (mTypesInfo->analyseType(type) == TYPE_LINK){
		Link *link = new Link(id, type);
		link->setName(name);
		if (Object * obj = mRepoData->getObject(parent)){
			obj->addEdgeChild(id);
			link->addRef(parent);
		} else {
			// Корневых линков у нас вроде как быть не должно, если что - использовать getRootObject.
			Q_ASSERT(!"Parent for a link not found in repo");
		}
		mRepoData->addLink(id, link);
		mLog += QString(", link created, name %1").arg(name);
	} else {
		qDebug() << __FUNCTION__ << "Wrong analyseType(\"" << type <<"\") result";
		return ReportError(ERR_INCORRECT_REQUEST);
	}

	mTypesInfo->elementCreated(type, id);
	return ReportSuccess(id);
}

IntQStringPair QRealRepoServerThread::handleReparentEntity(QStringVector const &params)
{
	if (!IsParamsNumberCorrect(params, "ReparentEntity", 3))
		return ReportError(ERR_INCORRECT_PARAMS);

	IdType const id = params[0];
	IdType const newParentId = params[1];
	IdType const oldParentId = params[2];

	if (Object * newparent = mRepoData->getObject(newParentId))
	{
		if( Object *oldparent = mRepoData->getObject(oldParentId) ){
			if( mRepoData->getObject(id) ){
				oldparent->removeNodeChild(id);
				newparent->addNodeChild(id);
				if( oldparent ){
					newparent->setChildConfiguration(id, oldparent->getChildConfiguration(id));
					newparent->setChildCoord(id, oldparent->getChildCoord(id));
				}
			}
			else if( mRepoData->getLink(id) ){
				oldparent->removeEdgeChild(id);
				newparent->addEdgeChild(id);
			}
		}	
	}

	mLog += QString(" element with id: %1, old parent: %2, new parent:  %3").arg(id).arg(oldParentId).arg(newParentId);
	return ReportSuccess(id);
}

IntQStringPair QRealRepoServerThread::handleCopyEntity(QStringVector const &params)
{
	if (!IsParamsNumberCorrect(params, "CopyEntity", 4))
		return ReportError(ERR_INCORRECT_PARAMS);

	// TODO: type вроде как не нужен.
	//	int type = params[0].toInt();
	IdType const id = params[1];
	IdType newParentId = params[2];
	IdType oldParentId = params[3];

	// TODO: Интересно, а как это должно копировать диаграмму в корень?
	// Что в этом случае будет newParentId?
	if (Object * newparent = mRepoData->getObject(newParentId))
	{
		Object * oldparent;
		if( Object * node = mRepoData->getObject(id) ){
			oldparent = mRepoData->getObject(oldParentId);
			newparent->addNodeChild(id);
			node->addRef(newParentId);
			if( oldparent ){
				newparent->setChildConfiguration(id, oldparent->getChildConfiguration(id));
				newparent->setChildCoord(id, oldparent->getChildCoord(id));
			}
		}
		else if( Link* edge = mRepoData->getLink(id) ){
			newparent->addEdgeChild(id);
			edge->addRef(newParentId);
		}
	}

	mLog += QString(" element  with id: %1, new parent --  %2").arg(id).arg(newParentId);
	return ReportSuccess(id);
}

IntQStringPair QRealRepoServerThread::handleFullCopyEntity(QStringVector const &params)
{
//	if (!IsParamsNumberCorrect(params, "FullCopyEntity", 4))
//		return ReportError(ERR_INCORRECT_PARAMS);

	IdType id = params[1];
	IdType parentId = params[2];
	IdType oldparentId = params[3];
	QStringVector par;
	par << params[0];
	if( Object * node = mRepoData->getObject(id) )
		par << node->getName();
	else if( Link* edge = mRepoData->getLink(id) )
		par << edge->getName();
	else
		qDebug() << __FUNCTION__ << "something really awful happened!";

	par << params[2];

	IntQStringPair result = handleCreateEntity(par);
	IdType newid = result.second;
	if( Object * node = mRepoData->getObject(newid) ){
		Object * oldnode = mRepoData->getObject(id);

		*node = *oldnode; // replace by copy constructor or something else
		node->setId(newid);
		node->clearChildren();
		node->removeAllRefs();
		node->addRef(parentId);

		Object * newparent = mRepoData->getObject(parentId);
		Object * oldparent = mRepoData->getObject(oldparentId);

		if( oldparent && newparent ){
			newparent->setChildConfiguration(node->getId(), oldparent->getChildConfiguration(oldnode->getId()));
			newparent->setChildCoord(node->getId(), oldparent->getChildCoord(oldnode->getId()));
		}

		QStringList list = oldnode->childrenToString().split("\t");
		for( int i=0; i<list.count()-1; i++ ){
			if( Object * child = mRepoData->getObject(list[i]) ){
				QStringVector par;
				par << child->getType() << list[i] <<newid << id;
				handleFullCopyEntity(par).second.toInt();
			} else if( Link* child = mRepoData->getLink(list[i]) ){
				QStringVector par;
				par << child->getType() << list[i] << newid << id;
				handleFullCopyEntity(par);
			}
		}
	}
	else //if( Link* edge = mRepoData->getLink(newid) ){
		qDebug() << "not implemented yet";

	return ReportSuccess(newid);
}

IntQStringPair QRealRepoServerThread::handleDeleteEntity(QStringVector const &params)
{
	if (!IsParamsNumberCorrect(params, "DeleteEntity", 2))
		return ReportError(ERR_INCORRECT_PARAMS);

	IdType id = params[0];
	IdType parent = params[1];

	Object * obj = mRepoData->getObject(parent);
	if (!obj && mRepoData->getRootObject()->isParentOf(id)) {
		obj = mRepoData->getRootObject();
	}

	if (obj)
	{
		if( Object * child = mRepoData->getObject(id) ){
			QString refs = child->referralsToString();
			if (refs != "")
			{
				qDebug() << "cannot delete popular entity" << id << ": " << refs;
				return ReportError(ERR_INCORRECT_REQUEST);
			}
			obj->removeNodeChild(id);
			child->removeRef(obj->getId());
			if( child->refCount() == 0 ){
				QString childrenString = child->childrenToString();
				if (childrenString != "") {
					QStringList children = childrenString.split("\t");
					// TODO: Make sure that no object can be itself's ancestor so we won't get stack overflow here.
					foreach(QString childId, children){
						QStringList l = QStringList(childId);
						l += id;
						handleDeleteEntity(l.toVector());
					}
				}
				mTypesInfo->elementDeleted(child->getType(), id);
				mRepoData->deleteObject(id);
			}
		} else if (Link * child = mRepoData->getLink(id)){
			obj->removeEdgeChild(id);
			child->removeRef(obj->getId());
			if( child->refCount() == 0 ){
				mTypesInfo->elementDeleted(child->getType(), id);
				mRepoData->deleteLink(id);
			}
		} else
			qDebug() << __FUNCTION__ << "unknown element: " << id;
	} else {
		qDebug() << __FUNCTION__ << "WTF??? Object " << parent << "not found";
		return ReportError(ERR_INCORRECT_REQUEST);
	}

	mLog += QString(", deleted entity [%1]").arg(id);
	return ReportSuccess("");
}

IntQStringPair QRealRepoServerThread::handleGetTypesCount(QStringVector const &params)
{
	if (!IsParamsNumberCorrect(params, "GetTypesCount", 0))
		return ReportError(ERR_INCORRECT_PARAMS);

	// FIXME: error code should always come first, then the args (if there are any)
	QString resp = QString::number(mTypesInfo->getTypes().count());
	mLog += QString(", sending types count: %1").arg(resp);
	return ReportSuccess(resp);
}

IntQStringPair QRealRepoServerThread::handleGetAllTypes(QStringVector const &params)
{
	if (!IsParamsNumberCorrect(params, "GetAllTypes", 0))
		return ReportError(ERR_INCORRECT_PARAMS);

	TypeIdTypeList types = mTypesInfo->getTypes();
	QString resp = "";
	foreach (TypeIdType type, types) {
		resp += QString("%1\t").arg(type);
	}
	mLog += QString(", sending types: %1").arg(resp);
	return ReportSuccess(resp);
}

IntQStringPair QRealRepoServerThread::handleGetTypeInfo(QStringVector const &params)
{
	if (!IsParamsNumberCorrect(params, "GetTypeInfo", 1))
		return ReportError(ERR_INCORRECT_PARAMS);

	IdType id = params[0];
	QString resp = mTypesInfo->getTypeInfoById(id).toString();
	mLog += QString(", sending type info: [%1]").arg(resp);
	return ReportSuccess(resp);
}

IntQStringPair QRealRepoServerThread::handleGetTypeByName(QStringVector const &params)
{
	if (!IsParamsNumberCorrect(params, "GetTypeByName", 1))
		return ReportError(ERR_INCORRECT_PARAMS);

	QString name = params[0];
	QString resp = mTypesInfo->getTypeInfoByName(name).toString();
	mLog += QString(", sending type info: [%1]").arg(resp);
	return ReportSuccess(resp);
}

IntQStringPair QRealRepoServerThread::handleGetTypesByMetatype(QStringVector const &params)
{
	if (!IsParamsNumberCorrect(params, "GetTypesByMetatype", 1))
		return ReportError(ERR_INCORRECT_PARAMS);

	MetaType metatype = static_cast<MetaType>(params[0].toInt());
	QString resp = mTypesInfo->getTypesByMetatype(metatype);
	mLog += QString(", sending types list: [%1]").arg(resp);
	return ReportSuccess(resp);
}

IntQStringPair QRealRepoServerThread::handleGetObjectsByType(QStringVector const &params)
{
	qDebug() << params;
	if (!IsParamsNumberCorrect(params, "GetObjectsByType", 1))
		return ReportError(ERR_INCORRECT_PARAMS);

	QString resp = "";
	TypeIdType type = params[0];
	if (mTypesInfo->analyseType(type) == TYPE_OBJECT)
	{
		resp = mRepoData->getObjectsByType(type);
	} else if (mTypesInfo->analyseType(type) == TYPE_LINK)
	{
		resp = mRepoData->getLinksByType(type);
	} else
	{
		qDebug() << __FUNCTION__ << "Wrong analyseType(" << type <<") result";
		return ReportError(ERR_INCORRECT_REQUEST);
	}
	mLog += QString(", sending objects of type: %1 -- [%2]").arg(type).arg(resp);
	return ReportSuccess(resp);
}

IntQStringPair QRealRepoServerThread::handleGetObjectData(QStringVector const &params)
{
	if (!IsParamsNumberCorrect(params, "GetObjectData", 1))
		return ReportError(ERR_INCORRECT_PARAMS);

	IdType id = params[0];
	int childCount = 0;
	TypeIdType type = "";
	QString name = "";
	QString res = "%1\t%2\t%3\t%4\t%5\t";
	QString resp = QString::number(ERR_UNKNOWN_ERROR);
	if (Object * obj = mRepoData->getObject(id)){
		name = obj->getName();
		childCount = obj->childrenCount();
		type = obj->getType();
	} else if (Link * link = mRepoData->getLink(id)){
		name = link->getName();
		type = link->getType();
	} else{
		qDebug() << __FUNCTION__ << "WTF " << id << "is???";
		return ReportError(ERR_INCORRECT_REQUEST);
	}

	if (type != ""){
		RealType info = mTypesInfo->getTypeInfoById(type);
	// FIXME: error code should always come first, then the args (if there are any)
		resp = res.arg(id).arg(name).arg(type).arg(info.getDescription()).arg(childCount);
	}
	mLog += QString(", sending object data: [%1]").arg(resp);
	return ReportSuccess(resp);
}

IntQStringPair QRealRepoServerThread::handleGetChildren(QStringVector const &params)
{
	if (!IsParamsNumberCorrect(params, "GetChildren", 1))
		return ReportError(ERR_INCORRECT_PARAMS);

	IdType id = params[0];
	QString resp = QString::number(ERR_UNKNOWN_ERROR);
	if (Object * obj = mRepoData->getObject(id))
		resp = obj->childrenToString();
	else if (mRepoData->getLink(id))
		resp = ""; // No children for link
	mLog += QString(", sending %1's children - [%2]").arg(id).arg(resp);
	return ReportSuccess(resp);
}

IntQStringPair QRealRepoServerThread::handleGetContainers(QStringVector const &params)
{
	if (!IsParamsNumberCorrect(params, "GetContainers", 1))
		return ReportError(ERR_INCORRECT_PARAMS);
	IdType id = params[0];
	QString resp = "";
	if (Object *obj = mRepoData->getObject(id)) {
		resp += obj->parentsToString();
	} else if (Link *link = mRepoData->getLink(id)) {
		resp += link->parentsToString();
	} else {
		qDebug() << __FUNCTION__ << "WTF " << id << "is???";
		return ReportError(ERR_INCORRECT_REQUEST);
	}

	return ReportSuccess(resp);
}

IntQStringPair QRealRepoServerThread::handleGetDescription(QStringVector const &params)
{
	if (!IsParamsNumberCorrect(params, "GetDescription", 1))
		return ReportError(ERR_INCORRECT_PARAMS);

	IdType id = params[0];
	QString resp = "";
	if (Object * obj = mRepoData->getObject(id))
		resp = obj->getDescription();
	else if (Link * link = mRepoData->getLink(id))
		resp = link->getDescription();
	else{
		qDebug() << __FUNCTION__ << "WTF " << id << "is???";
		return ReportError(ERR_INCORRECT_REQUEST);
	}
	mLog += QString(", sending description for id %1 - [%2]").arg(id).arg(resp);
	return ReportSuccess(resp);
}

IntQStringPair QRealRepoServerThread::handleSetDescription(QStringVector const &params)
{
	if (!IsParamsNumberCorrect(params, "SetDescription", 2))
		return ReportError(ERR_INCORRECT_PARAMS);

	IdType id = params[0];
	QString desc = params[1];
	if (Object * obj = mRepoData->getObject(id))
		obj->setDescription(desc);
	else if (Link * link = mRepoData->getLink(id))
		link->setDescription(desc);
	else
	{
		qDebug() << __FUNCTION__ << "WTF " << id << "is???";
		return ReportError(ERR_INCORRECT_REQUEST);
	}
	mLog += QString(", new description for id %1 - [%2]").arg(id).arg(desc);
	return ReportSuccess("");
}

IntQStringPair QRealRepoServerThread::handleGetPosition(QStringVector const &params)
{
	if (!IsParamsNumberCorrect(params, "GetPosition", 2))
		return ReportError(ERR_INCORRECT_PARAMS);

	IdType id = params[0];
	IdType parent = params[1];
	QString resp = "";
	if (Object * obj = mRepoData->getObject(parent)){
		if( mRepoData->getObject(id) ){
			QPoint p = obj->getChildCoord(id);
			resp = QString("%1;%2").arg(p.x()).arg(p.y());
		} else if ( mRepoData->getLink(id) ){
			resp = obj->getChildPos(id);
		}else{ 
			qDebug() << __FUNCTION__ << "Wrong analyseType result";			
			return ReportError(ERR_INCORRECT_REQUEST);
		}	
	}
	mLog += QString(", sending position for obj %1 - [%2]").arg(id).arg(resp);
	return ReportSuccess(resp);
}

IntQStringPair QRealRepoServerThread::handleSetPosition(QStringVector const &params)
{
	if (!IsParamsNumberCorrect(params, "SetPosition", 4))
		return ReportError(ERR_INCORRECT_PARAMS);

	IdType id = params[0];
	IdType parent = params[1];
	int x = params[2].toInt();
	int y = params[3].toInt();
	if (Object * obj = mRepoData->getObject(parent)){
		if( mRepoData->getObject(id) )
			obj->setChildCoord(id, QPoint(x, y));
		else if ( mRepoData->getLink(id) )
			obj->setChildPos(id, QString("%1;%2").arg(x).arg(y));
	} else {
		qDebug() << __FUNCTION__ << "Wrong analyseType result";
		return ReportError(ERR_INCORRECT_REQUEST);
	}
	mLog += QString(", new %1's position within %2 - (%3:%4)").arg(id).arg(parent).arg(x).arg(y);
	return ReportSuccess("");
}

IntQStringPair QRealRepoServerThread::handleGetConfiguration(QStringVector const &params)
{
	if (!IsParamsNumberCorrect(params, "GetConfiguration", 2))
		return ReportError(ERR_INCORRECT_PARAMS);

	IdType id = params[0];
	IdType parent = params[1];
	QString resp = "";
	if (Object *obj = mRepoData->getObject(parent)){
		resp = obj->getChildConfiguration(id);
	}
	else{
		qDebug() << __FUNCTION__ << "Wrong analyseType result";
		return ReportError(ERR_INCORRECT_REQUEST);
	}
	mLog += QString(", sending conf for %1 - [%2] ").arg(id).arg(resp);
	return ReportSuccess(resp);
}

IntQStringPair QRealRepoServerThread::handleSetConfiguration(QStringVector const &params)
{
	if (!IsParamsNumberCorrect(params, "SetConfiguration", 3))
		return ReportError(ERR_INCORRECT_PARAMS);

	IdType id = params[0];
	IdType parent = params[1];
	QString conf = params[2];
	if (Object *obj = mRepoData->getObject(parent)){
		if( !obj->setChildConfiguration(id, conf) )
			qDebug() << __FUNCTION__ << "setChildConfiguration failed for object " << id;
	}
	else{
		qDebug() << __FUNCTION__ << "Wrong analyseType result";
		return ReportError(ERR_INCORRECT_REQUEST);
	}
	mLog += QString(", new %1's config within %2: %3").arg(id).arg(parent).arg(conf);
	return ReportSuccess("");
}

IntQStringPair QRealRepoServerThread::handleSetProperty(QStringVector const &params)
{
	if (!IsParamsNumberCorrect(params, "SetProperty", 3))
		return ReportError(ERR_INCORRECT_PARAMS);

	IdType id = params[0];
	QString name = params[1];
	QString val = params[2];

	if (Object * obj = mRepoData->getObject(id))
		obj->setProperty(name, val);
	else if (Link * link = mRepoData->getLink(id)){
		link->print();
		if (name == "from"){
			IdType obj_id = link->getFrom();
			if (Object * obj = mRepoData->getObject(obj_id)){
				obj->removeLink(id, OUTCOMING_LINK);
				link->removeObjectFrom(obj_id);
			}
			if (Object * obj = mRepoData->getObject(val)){
				obj->addLink(id, OUTCOMING_LINK);
				link->addObjectFrom(val);
			}
		} else if (name == "to"){
			IdType obj_id = link->getTo();
			if (Object * obj = mRepoData->getObject(obj_id)){
				obj->removeLink(id, INCOMING_LINK);
				link->removeObjectTo(obj_id);
			}
			if (Object * obj = mRepoData->getObject(val)) {
				obj->addLink(id, INCOMING_LINK);
				link->addObjectTo(val);
			}
		} else
		link->setProperty(name, val);
	} else {
		qDebug() << __FUNCTION__ << "Wrong analyseType result";
		return ReportError(ERR_INCORRECT_REQUEST);
	}
	mLog += QString(", new property value: %1 - %2").arg(name).arg(val);
	return ReportSuccess("");
}

IntQStringPair QRealRepoServerThread::handleGetProperty(QStringVector const &params)
{
	if (!IsParamsNumberCorrect(params, "GetProperty", 2))
		return ReportError(ERR_INCORRECT_PARAMS);

	IdType id = params[0];
	QString name = params[1];
	QString resp = "";
	if (Object * obj = mRepoData->getObject(id))
		resp = obj->getProperty(name);
	else if (Link * link = mRepoData->getLink(id)){
		link->print();
		if (name == "from"){
			resp = link->getFrom();
		} else if (name == "to")
			resp = link->getTo();
		else
			resp = link->getProperty(name);
	} else {
		qDebug() << __FUNCTION__ << "Wrong analyseType result";
		return ReportError(ERR_INCORRECT_REQUEST);
	}

	mLog += QString(", sent %1's property value: %2 - [%3]").arg(id).arg(name).arg(resp);
	return ReportSuccess(resp);
}

IntQStringPair QRealRepoServerThread::handleAddLink(QStringVector const &params)
{
	if (!IsParamsNumberCorrect(params, "AddLink", 3))
		return ReportError(ERR_INCORRECT_PARAMS);

	IdType id = params[0];
	IdType link_id = params[1];
	int dir = params[2].toInt();
	// qDebug() << "adding link";
	if (Object * obj = mRepoData->getObject(id)){
		// qDebug() << "\tobject found! searching for link";
		obj->addLink(link_id, dir);
		if (Link * link = mRepoData->getLink(link_id)){
			// qDebug() << "\tlink found!";
			if (dir == OUTCOMING_LINK)
				link->addObjectFrom(id);
			else if (dir == INCOMING_LINK)
				link->addObjectTo(id);
			link->print();
		}
		obj->print();
	} else {
		qDebug() << __FUNCTION__ << "Wrong analyseType result";
		return ReportError(ERR_INCORRECT_REQUEST);
	}
	mLog += QString(", added new link %1 to object %2").arg(link_id).arg(id);
	return ReportSuccess("");
}

IntQStringPair QRealRepoServerThread::handleRemoveLink(QStringVector const &params)
{
	if (!IsParamsNumberCorrect(params, "RemoveLink", 3))
		return ReportError(ERR_INCORRECT_PARAMS);

	IdType id = params[0];
	IdType link_id = params[1];
	int dir = params[2].toInt();
	//  qDebug() << "\tremoving link " << id << link_id << dir;
	if (Object * obj = mRepoData->getObject(id)) {
		obj->removeLink(link_id, dir);
		//  qDebug() << "searching for link" << link_id;
		if (Link * link = mRepoData->getLink(link_id)){
		//  qDebug() << "\tlink found!, dir" << dir;
			if (dir == OUTCOMING_LINK)
				link->removeObjectFrom(id);
			else if (dir == INCOMING_LINK)
				link->removeObjectTo(id);
			link->print();
		} else {
			qDebug() << __FUNCTION__ << "Wrong analyseType result";
			return ReportError(ERR_INCORRECT_REQUEST);
		}
	} else {
		qDebug() << __FUNCTION__ << "Wrong analyseType result";
		return ReportError(ERR_INCORRECT_REQUEST);
	}
	mLog += QString(", removed link %1 from object %2").arg(link_id).arg(id);
	return ReportSuccess("");
}

IntQStringPair QRealRepoServerThread::handleGetEntireObject(QStringVector const &params)
{
	if (!IsParamsNumberCorrect(params, "GetEntireObject", 1))
		return ReportError(ERR_INCORRECT_PARAMS);

	IdType id = params[0];
	QString resp = "";
	if( Object * obj = mRepoData->getObject(id) )
		resp = obj->toString();
	else if( Link * edge = mRepoData->getLink(id) )
		resp = edge->toString();
	else {
		qDebug() << __FUNCTION__ << "WTF " << id << "is???";
		return ReportError(ERR_INCORRECT_REQUEST);
	}
	mLog += QString(", sending object %1: %2").arg(id).arg(resp);
	return ReportSuccess(resp);
}

IntQStringPair QRealRepoServerThread::handleGetLinksByObject(QStringVector const &params)
{
	if (!IsParamsNumberCorrect(params, "GetLinksByObject", 2))
		return ReportError(ERR_INCORRECT_PARAMS);

	IdType id = params[0];
	int dir = params[1].toInt();
	QString resp = "";
	if (Object * obj = mRepoData->getObject(id)){
		if (dir == INCOMING_LINK)
			resp = obj->getIncomingLinks();
		else if (dir == OUTCOMING_LINK)
			resp = obj->getOutcomingLinks();
	} else{
		qDebug() << __FUNCTION__ << "Wrong analyseType result";
		return ReportError(ERR_INCORRECT_REQUEST);
	}
	mLog += QString(", sending object's links %1: [%2]").arg(id).arg(resp);
	return ReportSuccess(resp);
}

IntQStringPair QRealRepoServerThread::handleGetObjectsByLink(QStringVector const &params)
{
	if (!IsParamsNumberCorrect(params, "GetObjectsByLink", 1))
		return ReportError(ERR_INCORRECT_PARAMS);

	IdType id = params[0];
	QString resp = "";
	if (Link * link = mRepoData->getLink(id))
		resp = link->getObjects();
	else{
		qDebug() << __FUNCTION__ << "Wrong analyseType result";
		return ReportError(ERR_INCORRECT_REQUEST);
	}
	mLog += QString(", sending link's objects %1: %2").arg(id).arg(resp);
	return ReportSuccess(resp);
}

IntQStringPair QRealRepoServerThread::handleGetAllObjects(QStringVector const &params)
{
	if (!IsParamsNumberCorrect(params, "GetAllObjects", 0))
		return ReportError(ERR_INCORRECT_PARAMS);

	QString resp = "";

	foreach (QString objectId, mRepoData->getAllObjects()) {
		resp += objectId + '\t';
	}

	mLog += QString(", sending all objects in repo [%1]").arg(resp);
	return ReportSuccess(resp);
}

IntQStringPair QRealRepoServerThread::handleClearAll(QStringVector const &/*params*/)
{
	foreach (IdType id, mRepoData->getAllObjects())
	{
		TypeIdType type = mRepoData->getObject(id)->getType();
		mTypesInfo->elementDeleted(type, id);
	}

	foreach (IdType id, mRepoData->getAllLinks())
	{
		TypeIdType type = mRepoData->getLink(id)->getType();
		mTypesInfo->elementDeleted(type, id);
	}

	delete mRepoData;
	mRepoData = new RepoData();
	clearLog();
	mLog += QString(", repository is cleared");
	return ReportSuccess("");
}

IntQStringPair QRealRepoServerThread::handleGetReferrals(QStringVector const & params)
{
	if (!IsParamsNumberCorrect(params, "GetReferrals", 1))
		return ReportError(ERR_INCORRECT_PARAMS);

	IdType id = params[0];
	QString resp = "";
	if( Object * obj = mRepoData->getObject(id) )
		resp = obj->referralsToString();
	else if( Link * edge = mRepoData->getLink(id) )
		resp = edge->referralsToString();
	else {
		qDebug() << __FUNCTION__ << "WTF " << id << "is???";
		return ReportError(ERR_INCORRECT_REQUEST);
	}
	mLog += QString(", sending referrals of %1: %2").arg(id).arg(resp);
	return ReportSuccess(resp);
}

IntQStringPair QRealRepoServerThread::handleIncReferral(QStringVector const & params)
{
	if (!IsParamsNumberCorrect(params, "IncReferral", 2))
		return ReportError(ERR_INCORRECT_PARAMS);

	IdType id = params[0];
	IdType ref = params[1];

	qDebug() << "Adding" << ref << "to" << id;
	QString resp = "";
	if( Object * obj = mRepoData->getObject(id) )
		obj->incReferral(ref);
	else if( Link * edge = mRepoData->getLink(id) )
		edge->incReferral(ref);
	else {
		qDebug() << __FUNCTION__ << "WTF " << id << "is???";
		return ReportError(ERR_INCORRECT_REQUEST);
	}
	return ReportSuccess(resp);
}


IntQStringPair QRealRepoServerThread::handleDecReferral(QStringVector const & params)
{
	if (!IsParamsNumberCorrect(params, "DecReferral", 2))
		return ReportError(ERR_INCORRECT_PARAMS);

	IdType id = params[0];
	IdType ref = params[1];
	qDebug() << "Removing" << ref << "from" << id;
	QString resp = "";
	if( Object * obj = mRepoData->getObject(id) )
		obj->decReferral(ref);
	else if( Link * edge = mRepoData->getLink(id) )
		edge->decReferral(ref);
	else {
		qDebug() << __FUNCTION__ << "WTF " << id << "is???";
		return ReportError(ERR_INCORRECT_REQUEST);
	}
	return ReportSuccess(resp);
}

IntQStringPair QRealRepoServerThread::handleCommand(QString const &data)
{
	dbg;
	QStringList command = data.split("\t");
	int cmd = command.front().toInt();
	command.pop_front();  // Discard command code.
	if (!command.empty() && command.back() == "" && cmd != CMD_CREATE_ENTITY)
		command.pop_back();  // Discard empty string that is sent at the end of nearly
							 // every command by some reason.
	mLog = QString("cmd: %1(%2)").arg(cmd_to_string(cmd)).arg(cmd);
	IntQStringPair resp = IntQStringPair(ERR_INCORRECT_REQUEST, "");

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
		case CMD_CREATE_ENTITY:
		{
			resp = handleCreateEntity(command.toVector());
			break;
		}
		case CMD_COPY_ENTITY:
		{
			resp = handleCopyEntity(command.toVector());
			break;
		}
		case CMD_REPARENT_ENTITY:
		{
			resp = handleReparentEntity(command.toVector());
			break;
		}
		case CMD_FULLCOPY_ENTITY:
		{
			resp = handleFullCopyEntity(command.toVector());
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
		case CMD_GET_CONTAINERS:
		{
			resp = handleGetContainers(command.toVector());
			break;
		}
		case CMD_GET_ALL_OBJECTS:
		{
			resp = handleGetAllObjects(command.toVector());
			break;
		}
		case CMD_CLEAR_ALL:
		{
			resp = handleClearAll(command.toVector());
			break;
		}
		case CMD_GET_REFERRALS:
		{
			resp = handleGetReferrals(command.toVector());
			break;
		}
		case CMD_INC_REFERRAL:
		{
			resp = handleIncReferral(command.toVector());
			break;
		}
		case CMD_DEC_REFERRAL:
		{
			resp = handleDecReferral(command.toVector());
			break;
		}
		default:
		{
			qDebug() << "Unknown command: " << data;
		}
	}

	qDebug() << mLog;
	return resp;
}
