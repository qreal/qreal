#include "client.h"
#include "../../qrgui/kernel/exception/exception.h"

#include <QtCore/QDebug>

using namespace qrRepo;
using namespace qrRepo::details;
using namespace qReal;

bool const failSafe = true;

Client::Client(QString const &workingDirectory)
	: serializer(workingDirectory, failSafe)
{
	init();
	loadFromDisk();
}

void Client::init()
{
	mObjects.insert(ROOT_ID, new LogicObject(ROOT_ID));
	mObjects[ROOT_ID]->setProperty("name", ROOT_ID.toString());
}

Client::~Client()
{
	delete mObjects[ROOT_ID];
	mObjects.remove(ROOT_ID);
	//serializer.saveToDisk(mObjects.values());
}

IdList Client::children(Id const &id) const
{
	if (mObjects.contains(id)) {
		return mObjects[id]->children();
	} else {
		throw Exception("Client: Requesting children of nonexistent object " + id.toString());
	}
}

IdList Client::parents(Id const &id) const
{
	if (mObjects.contains(id)) {
		return mObjects[id]->parents();
	} else {
		throw Exception("Client: Requesting parents of nonexistent object " + id.toString());
	}
}

void Client::addParent(Id const &id, Id const &parent)
{
	if (mObjects.contains(id)) {
		if (mObjects.contains(parent)) {
			mObjects[id]->addParent(parent);
			if (!failSafe || !mObjects[parent]->children().contains(id))
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
	if (mObjects.contains(id)) {
		if (!failSafe || !mObjects[id]->children().contains(child))
			mObjects[id]->addChild(child);
		if (mObjects.contains(child)) {
			mObjects[child]->addParent(id);
		} else {
			mObjects.insert(child,new LogicObject(child,id));
		}
	} else {
		throw Exception("Client: Adding child " + child.toString() + " to nonexistent object " + id.toString());
	}
}

void Client::removeParent(const Id &id, const Id &parent)
{
	if (mObjects.contains(id)) {
		if (mObjects.contains(parent)) {
			mObjects[id]->removeParent(parent);
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
			if (mObjects[child]->parents().size()!=1) {
				mObjects[child]->removeParent(id);
			} else {
				if (mObjects[child]->parents().first()==id) {
					delete mObjects[child];
					mObjects.remove(child);
				} else {
					throw Exception("Client: removing child " + child.toString() + " from object " + id.toString() + ", which is not his parent");
				}
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
	foreach (LogicObject *object, mObjects.values()) {
		if (object->parents().contains(ROOT_ID)) {
			if (!failSafe || !mObjects[ROOT_ID]->children().contains(object->id()))
				mObjects[ROOT_ID]->addChild(object->id());
		}
	}
}

void Client::save() const
{
	serializer.saveToDisk(mObjects.values());
}

void Client::saveTo(QString const &workingDir)
{
	serializer.setWorkingDir(workingDir);
	serializer.saveToDisk(mObjects.values());
}

void Client::printDebug() const
{
	qDebug() << mObjects.size() << " objects in repository";
	foreach (LogicObject *object, mObjects.values()) {
		qDebug() << object->id().toString();
		qDebug() << "Children:";
		foreach (Id id, object->children())
			qDebug() << id.toString();
		qDebug() << "Parents:";
		foreach (Id id, object->parents())
			qDebug() << id.toString();
		qDebug() << "============";
	}
}

void Client::exterminate()
{
	printDebug();
	mObjects.clear();
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
