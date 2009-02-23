/** @file realrepoapiclasses.cpp
 * 	@class Классы, используемые в API репозитория
 * */
#include "realrepoapiclasses.h"
#include <QDebug>
#include <QStringList>

#include "../repo/realrepoclient.h"

using namespace qRealTypes;

IdType RealNamedEntity::getId()
{
	return m_id;
}

void RealNamedEntity::setId( IdType const &id )
{
	m_id = id;
}

QString RealNamedEntity::getName()
{
	if( client )
		m_name = client->getName(m_id);
	return m_name;
}

void RealNamedEntity::setName( const QString& arg )
{
	m_name = arg;
}

TypeIdType RealNamedEntity::getTypeId() const
{
	return m_type;
}

void RealNamedEntity::setTypeId( TypeIdType const &arg )
{
	m_type = arg;
}

QString RealNamedEntity::getDescription()
{
	if( client )
		m_description = client->getDescription(m_id);
	return m_description;
}

void RealNamedEntity::setDescription( const QString& arg )
{
	m_description = arg;
	if( client )
		client->setDescription(m_id, arg);
}

void RealNamedEntity::setProperty( const QString& name, const QString& val )
{
	m_properties[name] = val;
	if( client )
		client->setPropValue(m_id, name, val);
}

QString RealNamedEntity::getProperty( const QString& name )
{
	if( client )
		m_properties[name] = client->getPropValue(m_id, name);
	else
		return "";
	return m_properties[name];
}

int RealNamedEntity::getPropertiesCount() const
{
	return m_properties.count();
}

void RealNamedEntity::setRepoClient( RealRepoClient * arg )
{
	client = arg;
}

// ================================================== //

MetaType RealType::getMetaType() const
{
	return m_metatype;
}

void RealType::setMetaType( const MetaType arg )
{
	m_metatype = arg;
}

QList<QString> RealType::getObjects() const
{
	return m_objects;
}

void RealType::addObject(IdType const &id)
{
	if( !m_objects.contains(id) )
		m_objects << id;
}

void RealType::deleteObject(IdType const &id)
{
	m_objects.removeAll(id);
}

QString RealType::toString() const
{
	QString res = QString("%1\t%2\t%3\t%4\t").arg(m_id).arg(m_name).arg(m_description).arg(m_metatype);

	res += QString("%1\t").arg(m_objects.size());
	for( int i=0; i<m_objects.size(); i++)
		res += QString("%1\t").arg(m_objects.at(i));

	res += QString("%1\t").arg(m_properties.size());
	for( int i=0; i<m_properties.keys().size(); i++){
		 res += QString("%1;%2\t").arg(m_properties.keys().at(i)).arg(m_properties.value(m_properties.keys().at(i)));
	}
	return res;
}

void RealType::loadFromString( const QString& data )
{
	m_id = data.section("\t", 0, 0);
	m_name = data.section("\t", 1, 1);
	m_description = data.section("\t", 2, 2);
	m_metatype = (MetaType) data.section("\t", 3, 3).toInt();

	int objCounter = data.section("\t", 4, 4).toInt();
	int counter = 5;
	for (int i = 0; i < objCounter; ++i) {
		m_objects << data.section("\t", counter, counter);
		++counter;
	}

	int propsCount = data.section("\t", counter, counter).toInt();
	++counter;
	for (int i = 0; i < propsCount; ++i) {
		QString pair = data.section("\t", counter, counter);
		setProperty(pair.section(";", 0, 0), pair.section(";", 1, 1));
		++counter;
	}
}

// ================================================== //

bool RealObject::getVisibility() const
{
	return m_visibility;
}

void RealObject::setVisibility( const bool arg )
{
	m_visibility = arg;
}

int RealObject::setConfiguration(IdType const &parent, QString conf)
{
	if (client)
		return client->setConfiguration(m_id, parent, conf);
	return -1;
}

QString RealObject::getConfiguration(IdType const &parent)
{
	if (client)
		return client->getConfiguration(m_id, parent);
	return "";
}

IdTypeList RealObject::getContainerId()
{
	if (client)
		return client->getParents(m_id);
	Q_ASSERT(!"No repo client");
	return IdTypeList();
}

void RealObject::addToContainer(IdType const &id)
{
	if (client)
		client->copyEntity(m_type, m_id, id, INVALID_ID);
}

void RealObject::deleteFromContainer(IdType const &id)
{
	if (client)
		client->deleteObject(m_id, id);
}

void RealObject::setContainerId(IdType const &id)
{
	IdTypeList containers = getContainerId();
	foreach (IdType containerId, containers) {
		deleteFromContainer(containerId);
	}
	if (client)
		client->copyEntity(m_type, m_id, id, INVALID_ID);
}

IdTypeList RealObject::getChildElements()
{
	if (client) {
		IdTypeList children;
		QStringList list = client->getChildren(m_id).split("\t", QString::SkipEmptyParts);
		foreach (QString child, list)
			children << child;
	}
	return IdTypeList();
}

void RealObject::addChildElement(IdType const &id)
{
	if (client) {
		RealObject object = client->getObjectById(id);
		object.setContainerId(m_id);
	}
}

void RealObject::deleteChildElement(IdType const &id)
{
	if (client) {
		client->deleteObject(id, m_id);
	}
}

IdTypeList RealObject::getAllLinks( ) const
{
	return getIncomingLinks() + getOutcomingLinks();
}

IdTypeList RealObject::getIncomingLinks() const
{
	return m_incomingLinks;
}

IdTypeList RealObject::getOutcomingLinks() const
{
	return m_outcomingLinks;
}

/*void RealObject::addLink( IdType const &id )
{
	if( !m_links.contains(id) )
		m_links << id;
}
*/
void RealObject::addIncomingLink( IdType const &id )
{
	if( !m_incomingLinks.contains(id) )
		m_incomingLinks << id;
	if( client )
		client->addLink( m_id, id, INCOMING_LINK );
}

void RealObject::addOutcomingLink( IdType const &id )
{
	if( !m_outcomingLinks.contains(id) )
		m_outcomingLinks << id;
	if( client )
		client->addLink( m_id, id, OUTCOMING_LINK );
}

void RealObject::removeIncomingLink( IdType const &id )
{
	m_incomingLinks.removeAll(id);
	if( client )
		client->removeLink( m_id, id, INCOMING_LINK );
}

void RealObject::removeOutcomingLink( IdType const &id )
{
	m_outcomingLinks.removeAll(id);
	if( client )
		client->removeLink( m_id, id, OUTCOMING_LINK );

}

// ================================================== //

IdType RealLink::getFromId()
{
	IdType val = getProperty("from");
	if (val == "")
		return INVALID_ID;
	return val;
}

IdType RealLink::getToId()
{
	IdType val = getProperty("to");
	if (val == "")
		return INVALID_ID;
	return val;
}

void RealLink::setFromId( IdType const &arg )
{
	m_fromId = arg;
	setProperty("from", arg);
}

void RealLink::setToId( IdType const &arg )
{
	m_toId = arg;
	setProperty("to", arg);
}
