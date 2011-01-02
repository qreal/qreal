#include "client.h"
#include "../../qrgui/kernel/exception/exception.h"

#include <QtCore/QDebug>

using namespace qReal;
using namespace qrRepo;
using namespace qrRepo::details;

Client::Client(QString const &workingDirectory)
	: serializer(workingDirectory)
{
	init();
	loadFromDisk();
}

void Client::init()
{
	mObjects.insert(Id::rootId(), new Object(Id::rootId()));
	mObjects[Id::rootId()]->setProperty("name", Id::rootId().toString());
}

Client::~Client()
{
	delete mObjects[Id::rootId()];
	mObjects.remove(Id::rootId());
	serializer.saveToDisk(mObjects.values());
}

IdList Client::children(Id const &id) const
{
	if (mObjects.contains(id)) {
		return mObjects[id]->children();
	} else {
		throw Exception("Client: Requesting children of nonexistent object " + id.toString());
	}
}

Id Client::parent(Id const &id) const
{
	if (mObjects.contains(id)) {
		return mObjects[id]->parent();
	} else {
		throw Exception("Client: Requesting parents of nonexistent object " + id.toString());
	}
}

void Client::setParent(Id const &id, Id const &parent)
{
	if (mObjects.contains(id)) {
		if (mObjects.contains(parent)) {
			mObjects[id]->setParent(parent);
			if (!mObjects[parent]->children().contains(id))
				mObjects[parent]->addChild(id);
		} else {
			throw Exception("Client: Adding nonexistent parent " + parent.toString() + " to  object " + id.toString());
		}
	} else {
		throw Exception("Client: Adding parent " + parent.toString() + " to nonexistent object " + id.toString());
	}
}

void Client::addChild(const Id &id, const Id &child)
{
	addChild(id, child, Id());
}


void Client::addChild(const Id &id, const Id &child, Id const &logicalId)
{
	if (mObjects.contains(id)) {
		if (!mObjects[id]->children().contains(child))
			mObjects[id]->addChild(child);
		if (mObjects.contains(child)) {
			mObjects[child]->setParent(id);
		} else {
			mObjects.insert(child, new Object(child, id, logicalId));
		}
	} else {
		throw Exception("Client: Adding child " + child.toString() + " to nonexistent object " + id.toString());
	}
}

void Client::removeParent(const Id &id, const Id &parent)
{
	if (mObjects.contains(id)) {
		if (mObjects.contains(parent)) {
			mObjects[id]->removeParent();
			mObjects[parent]->removeChild(id);
		} else {
			throw Exception("Client: Removing nonexistent parent " + parent.toString() + " from object " + id.toString());
		}
	} else {
		throw Exception("Client: Removing parent " + parent.toString() + " from nonexistent object " + id.toString());
	}
}

void Client::removeChild(const Id &id, const Id &child)
{
	if (mObjects.contains(id)) {
		if (mObjects.contains(child)) {
			mObjects[id]->removeChild(child);
			if (mObjects[child]->parent() == id) {
				delete mObjects[child];
				mObjects.remove(child);
			} else {
				throw Exception("Client: removing child " + child.toString() + " from object " + id.toString() + ", which is not his parent");
			}
		} else {
			throw Exception("Client: removing nonexistent child " + child.toString() + " from object " + id.toString());
		}
	} else {
		throw Exception("Client: removing child " + child.toString() + " from nonexistent object " + id.toString());
	}
}

void Client::setProperty(const Id &id, const QString &name, const QVariant &value )
{
	if (mObjects.contains(id)) {
		Q_ASSERT(mObjects[id]->hasProperty(name)
				 ? mObjects[id]->property(name).userType() == value.userType()
				 : true);
		mObjects[id]->setProperty(name, value);
	} else {
		throw Exception("Client: Setting property of nonexistent object " + id.toString());
	}
}

QVariant Client::property( const Id &id, const QString &name ) const
{
	if (mObjects.contains(id)) {
		return mObjects[id]->property(name);
	} else {
		throw Exception("Client: Requesting property of nonexistent object " + id.toString());
	}
}

void Client::removeProperty( const Id &id, const QString &name )
{
	if (mObjects.contains(id)) {
		return mObjects[id]->removeProperty(name);
	} else {
		throw Exception("Client: Removing property of nonexistent object " + id.toString());
	}
}

bool Client::hasProperty(const Id &id, const QString &name) const
{
	if (mObjects.contains(id)) {
		return mObjects[id]->hasProperty(name);
	} else {
		throw Exception("Client: Checking the existence of a property of nonexistent object " + id.toString());
	}
}

void Client::loadFromDisk()
{
	serializer.loadFromDisk(mObjects);
	addChildrenToRootObject();
}

void Client::addChildrenToRootObject()
{
	foreach (Object *object, mObjects.values()) {
		if (object->parent() == Id::rootId()) {
			if (!mObjects[Id::rootId()]->children().contains(object->id()))
				mObjects[Id::rootId()]->addChild(object->id());
		}
	}
}

IdList Client::idsOfAllChildrenOf(Id id) const
{
	IdList result;
	result.append(id);
	foreach(Id childId,mObjects[id]->children())
		result.append(idsOfAllChildrenOf(childId));
	return result;
}

QList<Object*> Client::allChildrenOf(Id id) const
{
	QList<Object*> result;
	result.append(mObjects[id]);
	foreach(Id childId,mObjects[id]->children())
		result.append(allChildrenOf(childId));
	return result;
}

bool Client::exist(const Id &id) const
{
	return (mObjects[id] != NULL);
}

void Client::saveAll() const
{
	serializer.clearWorkingDir();
	serializer.saveToDisk(mObjects.values());
}

void Client::save(IdList list) const
{
	QList<Object*> toSave;
	foreach(Id id, list)
		toSave.append(allChildrenOf(id));

	serializer.saveToDisk(toSave);
}

void Client::remove(IdList list) const
{
	qDebug() << "Client::remove(IdList), list.size() > 0 == " << (list.size()>0);
	foreach(Id id, list) {
		qDebug() << id.toString();
		serializer.removeFromDisk(id);
	}
}

void Client::setWorkingDir(QString const &workingDir)
{
	serializer.setWorkingDir(workingDir);
}

void Client::log(QString const message, const qReal::Id diagram)
{
	serializer.log(message, diagram);
}

void Client::printDebug() const
{
	qDebug() << mObjects.size() << " objects in repository";
	foreach (Object *object, mObjects.values()) {
		qDebug() << object->id().toString();
		qDebug() << "Children:";
		foreach (Id id, object->children())
			qDebug() << id.toString();
		qDebug() << "Parent:";
		qDebug() << object->parent().toString();
		qDebug() << "============";
	}
}

void Client::exterminate()
{
	printDebug();
	mObjects.clear();
	serializer.clearWorkingDir();
	serializer.saveToDisk(mObjects.values());
	init();
	printDebug();
}

void Client::open(QString const &workingDir)
{
	serializer.setWorkingDir(workingDir);
	mObjects.clear();
	init();
	loadFromDisk();
}

qReal::IdList Client::elements() const
{
	return mObjects.keys();
}

bool Client::isLogicalId(qReal::Id const &elem) const
{
	return mObjects[elem]->logicalId() == qReal::Id();
}

qReal::Id Client::logicalId(qReal::Id const &elem) const
{
	return mObjects[elem]->logicalId();
}

