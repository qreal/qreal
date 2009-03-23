#include "Client.h"
#include "Exception.h"

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

IdTypeList Client::children( IdType id )
{
	if (mObjects.contains(id)) {
		return mObjects[id]->children();
	} else {
		throw Exception("Client: Requesting children of nonexistent object " + id);
	}
}

IdTypeList Client::parents( IdType id )
{
	if (mObjects.contains(id)) {
		return mObjects[id]->parents();
	} else {
		throw Exception("Client: Requesting parents of nonexistent object " + id);
	}
}

void Client::addParent( IdType id, IdType parent )
{
	if (mObjects.contains(id)) {
		mObjects[id]->addParent(parent);
	} else {
		throw Exception("Client: Adding parent " + parent + " to nonexistent object " + id);
	}
}

void Client::addChild( IdType id, IdType child )
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

void Client::removeParent( IdType id, IdType parent )
{
	if (mObjects.contains(id)) {
		mObjects[id]->removeParent(parent);
	} else {
		throw Exception("Client: Removing parent " + parent + " from nonexistent object " + id);
	}
}

void Client::removeChild( IdType id, IdType child )
{
	if (mObjects.contains(id)) {
		mObjects[id]->removeChild(child);
	} else {
		throw Exception("Client: removing child " + child + " from nonexistent object " + id);
	}
}

void Client::setProperty( IdType id, PropertyType type, QVariant value )
{
	if (mObjects.contains(id)) {
		mObjects[id]->setProperty(type,value);
	} else {
		throw Exception("Client: Setting property of nonexistent object " + id);
	}
}

void Client::property( IdType id, PropertyType type )
{
	if (mObjects.contains(id)) {
		mObjects[id]->property(type);
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