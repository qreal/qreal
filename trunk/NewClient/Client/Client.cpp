#include "Client.h"
#include "../Service/Exception/Exception.h"

using namespace qReal;

using namespace client;

Client::Client()
{
	loadFromDisk();
	mObjects.insert(ROOT_ID,new LogicObject(ROOT_ID));
	mObjects[ROOT_ID]->setProperty("Name",ROOT_ID.toString());
}

Client::~Client()
{
	delete mObjects[ROOT_ID];
	mObjects.remove(ROOT_ID);
	saveToDisk();
}

IdTypeList Client::children( const IdType &id ) const
{
	if (mObjects.contains(id)) {
		return mObjects[id]->children();
	} else {
		throw Exception("Client: Requesting children of nonexistent object " + id.toString());
	}
}

IdTypeList Client::parents( const IdType &id ) const
{
	if (mObjects.contains(id)) {
		return mObjects[id]->parents();
	} else {
		throw Exception("Client: Requesting parents of nonexistent object " + id.toString());
	}
}

void Client::addParent( const IdType &id, const IdType &parent )
{
	if (mObjects.contains(id)) {
		if (mObjects.contains(parent)) {
			mObjects[id]->addParent(parent);
			mObjects[parent]->addChild(id);
		} else {
			throw Exception("Client: Adding nonexistent parent " + parent.toString() + " to  object " + id.toString());
		}
	} else {
		throw Exception("Client: Adding parent " + parent.toString() + " to nonexistent object " + id.toString());
	}
}

void Client::addChild( const IdType &id, const IdType &child )
{
	if (id==ROOT_ID) {
		mObjects.insert(child,new LogicObject(child,ROOT_ID));
	} else {
		if (mObjects.contains(id)) {
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
}

void Client::removeParent( const IdType &id, const IdType &parent )
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

void Client::setProperty( const IdType &id, const PropertyName &name, const QVariant &value )
{
	if (mObjects.contains(id)) {
		mObjects[id]->setProperty(name,value);
	} else {
		throw Exception("Client: Setting property of nonexistent object " + id.toString());
	}
}

QVariant Client::property( const IdType &id, const PropertyName &name ) const
{
	if (mObjects.contains(id)) {
		return mObjects[id]->property(name);
	} else {
		throw Exception("Client: Requesting property of nonexistent object " + id.toString());
	}
}

void Client::removeProperty( const IdType &id, const PropertyName &name )
{
	if (mObjects.contains(id)) {
		return mObjects[id]->removeProperty(name);
	} else {
		throw Exception("Client: Removing property of nonexistent object " + id.toString());
	}
}

void Client::loadFromDisk()
{

}

void Client::saveToDisk()
{

}
