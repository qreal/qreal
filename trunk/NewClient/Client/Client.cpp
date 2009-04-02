#include "Client.h"
#include "../Service/Exception/Exception.h"

using namespace qReal;

using namespace client;

Client::Client()
{
	loadFromDisk();
}

Client::~Client()
{
	saveToDisk();
}

IdTypeList Client::children( const IdType &id ) const
{
	if (mObjects.contains(id)) {
		return mObjects[id]->children();
	} else {
		throw Exception("Client: Requesting children of nonexistent object " + id);
	}
}

IdTypeList Client::parents( const IdType &id ) const
{
	if (mObjects.contains(id)) {
		return mObjects[id]->parents();
	} else {
		throw Exception("Client: Requesting parents of nonexistent object " + id);
	}
}

void Client::addParent( const IdType &id, const IdType &parent )
{
	if (mObjects.contains(id)) {
		if (mObjects.contains(parent)) {	
			mObjects[id]->addParent(parent);
			mObjects[parent]->addChild(id);
		} else {
			throw Exception("Client: Adding nonexistent parent " + parent + " to  object " + id);
		}
	} else {
		throw Exception("Client: Adding parent " + parent + " to nonexistent object " + id);
	}
}

void Client::addChild( const IdType &id, const IdType &child )
{
	if (id=="") {
		mObjects.insert(child,new LogicObject(child,""));
	} else {
		if (mObjects.contains(id)) {
			mObjects[id]->addChild(child);
			if (mObjects.contains(child)) {
				mObjects[child]->addParent(id);
			} else {
				mObjects.insert(child,new LogicObject(child,id));
			}
		} else {
			throw Exception("Client: Adding child " + child + " to nonexistent object " + id);
		}
	}
}

void Client::removeParent( const IdType &id, const IdType &parent )
{
	if (mObjects.contains(id)) {
		if (mObjects.contains(parent)) {
			mObjects[id]->removeParent(parent);
			mObjects[parent]->removeChild(id);
		} else {
			throw Exception("Client: Removing nonexistent parent " + parent + " from object " + id);
		}
	} else {
		throw Exception("Client: Removing parent " + parent + " from nonexistent object " + id);
	}
}

void Client::removeChild( const IdType &id, const IdType &child )
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
					throw Exception("Client: removing child " + child + " from object " + id + ", which is not his parent");
				}
			}
		} else {
			throw Exception("Client: removing nonexistent child " + child + " from object " + id);
		}
	} else {
		throw Exception("Client: removing child " + child + " from nonexistent object " + id);
	}
}

void Client::setProperty( const IdType &id, const PropertyName &type, const QVariant &value )
{
	if (mObjects.contains(id)) {
		mObjects[id]->setProperty(type,value);
	} else {
		throw Exception("Client: Setting property of nonexistent object " + id);
	}
}

QVariant Client::property( const IdType &id, const PropertyName &type ) const
{
	if (mObjects.contains(id)) {
		return mObjects[id]->property(type);
	} else {
		throw Exception("Client: Requesting property of nonexistent object " + id);
	}
}

void Client::loadFromDisk()
{

}

void Client::saveToDisk()
{

}