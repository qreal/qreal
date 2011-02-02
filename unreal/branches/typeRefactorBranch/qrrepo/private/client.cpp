#include "client.h"
#include "../../qrgui/kernel/exception/exception.h"

#include <QtCore/QDebug>

using namespace qReal;
using namespace qrRepo;
using namespace qrRepo::details;

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

TypeList Client::children(NewType const &type) const
{
	if (mObjects.contains(type)) {
		return mObjects[type]->children();
	} else {
		throw Exception("Client: Requesting children of nonexistent object " + type.toString());
	}
}

TypeList Client::parents(NewType const &type) const
{
	if (mObjects.contains(type)) {
		return mObjects[type]->parents();
	} else {
		throw Exception("Client: Requesting parents of nonexistent object " + type.toString());
	}
}

void Client::addParent(NewType const &type, NewType const &parent)
{
	if (mObjects.contains(type)) {
		if (mObjects.contains(parent)) {
			mObjects[type]->addParent(parent);
			if (!failSafe || !mObjects[parent]->children().contains(type))
				mObjects[parent]->addChild(type);
		} else {
			throw Exception("Client: Adding nonexistent parent " + parent.toString() + " to  object " + type.toString());
		}
	} else {
		throw Exception("Client: Adding parent " + parent.toString() + " to nonexistent object " + type.toString());
	}
}

void Client::addChild(const NewType &type, const NewType &child)
{
	if (mObjects.contains(type)) {
		if (!failSafe || !mObjects[type]->children().contains(child))
			mObjects[type]->addChild(child);
		if (mObjects.contains(child)) {
                        mObjects[child]->addParent(type);
		} else {
                        mObjects.insert(child,new LogicObject(child,type));
		}
	} else {
                throw Exception("Client: Adding child " + child.toString() + " to nonexistent object " + type.toString());
	}
}

void Client::removeParent(const NewType &type, const NewType &parent)
{
        if (mObjects.contains(type)) {
		if (mObjects.contains(parent)) {
                        mObjects[type]->removeParent(parent);
                        mObjects[parent]->removeChild(type);
		} else {
                        throw Exception("Client: Removing nonexistent parent " + parent.toString() + " from object " + type.toString());
		}
	} else {
                throw Exception("Client: Removing parent " + parent.toString() + " from nonexistent object " + type.toString());
	}
}

void Client::removeChild(const NewType &type, const NewType &child)
{
        if (mObjects.contains(type)) {
		if (mObjects.contains(child)) {
                        mObjects[type]->removeChild(child);
			if (mObjects[child]->parents().size()!=1) {
                                mObjects[child]->removeParent(type);
			} else {
                                if (mObjects[child]->parents().first()==type) {
					delete mObjects[child];
					mObjects.remove(child);
				} else {
                                        throw Exception("Client: removing child " + child.toString() + " from object " + type.toString() + ", which is not his parent");
				}
			}
		} else {
                        throw Exception("Client: removing nonexistent child " + child.toString() + " from object " + type.toString());
		}
	} else {
                throw Exception("Client: removing child " + child.toString() + " from nonexistent object " + type.toString());
	}
}

void Client::setProperty(const NewType &type, const QString &name, const QVariant &value )
{
        if (mObjects.contains(type)) {
                Q_ASSERT(mObjects[type]->hasProperty(name)
                                 ? mObjects[type]->property(name).userType() == value.userType()
				 : true);
                mObjects[type]->setProperty(name, value);
	} else {
                throw Exception("Client: Setting property of nonexistent object " + type.toString());
	}
}

QVariant Client::property( const NewType &type, const QString &name ) const
{
        if (mObjects.contains(type)) {
                return mObjects[type]->property(name);
	} else {
                throw Exception("Client: Requesting property of nonexistent object " + type.toString());
	}
}

void Client::removeProperty( const NewType &type, const QString &name )
{
        if (mObjects.contains(type)) {
                return mObjects[type]->removeProperty(name);
	} else {
                throw Exception("Client: Removing property of nonexistent object " + type.toString());
	}
}

bool Client::hasProperty(const NewType &type, const QString &name) const
{
        if (mObjects.contains(type)) {
                return mObjects[type]->hasProperty(name);
	} else {
                throw Exception("Client: Checking the existence of a property of nonexistent object " + type.toString());
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
                        if (!failSafe || !mObjects[ROOT_ID]->children().contains(object->type()))
                                mObjects[ROOT_ID]->addChild(object->type());
		}
	}
}

TypeList Client::typesOfAllChildrenOf(NewType type) const
{
	qDebug() << "ID: " << mObjects[type];

        TypeList result;
	result.append(type);
        foreach(NewType childId,mObjects[type]->children())
                result.append(typesOfAllChildrenOf(childId));
	return result;
}

QList<LogicObject*> Client::allChildrenOf(NewType type) const
{
	QList<LogicObject*> result;
	result.append(mObjects[type]);
        foreach(NewType childId,mObjects[type]->children())
		result.append(allChildrenOf(childId));
	return result;
}

bool Client::exist(const NewType &type) const
{
	return (mObjects[type] != NULL);
}

void Client::saveAll() const
{
	serializer.clearWorkingDir();
	serializer.saveToDisk(mObjects.values());
}

void Client::save(TypeList list) const
{
	QList<LogicObject*> toSave;
        foreach(NewType type, list)
		toSave.append(allChildrenOf(type));

	serializer.saveToDisk(toSave);
}

void Client::remove(TypeList list) const
{
        qDebug() << "Client::remove(TypeList), list.size() > 0 == " << (list.size()>0);
        foreach(NewType type, list) {
		qDebug() << type.toString();
		serializer.removeFromDisk(type);
	}
}

void Client::setWorkingDir(QString const &workingDir)
{
	serializer.setWorkingDir(workingDir);
}

void Client::log(QString const message, const qReal::NewType diagram)
{
	serializer.log(message, diagram);
}

void Client::printDebug() const
{
	qDebug() << mObjects.size() << " objects in repository";
	foreach (LogicObject *object, mObjects.values()) {
                qDebug() << object->type().toString();
		qDebug() << "Children:";
                foreach (NewType type, object->children())
			qDebug() << type.toString();
		qDebug() << "Parents:";
                foreach (NewType type, object->parents())
			qDebug() << type.toString();
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

qReal::TypeList Client::elements() const
{
	return mObjects.keys();
}
