#include "realrepoapiclasses.h"

using namespace QRealTypes;

int RealNamedEntity::getId() 
{
	return m_id;
}

void RealNamedEntity::setId( const int id )
{
	m_id = id;
}

QString RealNamedEntity::getName() const
{
	return m_name;
}

void RealNamedEntity::setName( const QString& arg ) 
{
	m_name = arg;
}

void RealNamedEntity::setProperty( const QString& name, const QString& val )
{
	m_properties[name] = val;
}

QString RealNamedEntity::getProperty( const QString& name ) const
{
	return m_properties[name];
}

int RealNamedEntity::getPropertiesCount() const
{
	return m_properties.count();
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

QIntList RealType::getObjects() const 
{
	return m_objects;
}

// ================================================== //

int RealObject::getTypeId() const
{
	return m_typeId;
}

void RealObject::setTypeId( const int arg )
{
	m_typeId = arg;
}

bool RealObject::getVisibility() const 
{
	return m_visibility;
}

void RealObject::setVisibility( const bool arg )
{
	m_visibility = arg;
}

int RealObject::getContainerId() const
{
	return m_containerId;
}

void RealObject::setContainerId( const int arg )
{
	m_containerId = arg;
}

QString RealObject::getConfiguration() const 
{
	return m_configuration;
}

void RealObject::setConfiguration( const QString& arg )
{
	m_configuration = arg;
}

QIntList RealObject::getChildElements() const 
{
	return m_children;
}

void RealObject::addChildElement( const int arg )
{
	if( !m_children.contains(arg) )
		m_children << arg;
}

void RealObject::deleteChildElement( const int arg )
{
	if( m_children.contains(arg) )
		m_children.removeAll(arg);
}

QIntList RealObject::getAllLinks() const 
{
	return m_links;
}

void RealObject::addLink( const int id )
{
	if( !m_links.contains(id) )
		m_links << id;
}

// ================================================== //

int RealLink::getTypeId() const
{
	return m_type;
}

void RealLink::setTypeId( const int arg )
{
	m_type = arg;
}

int RealLink::getFromId() const 
{
	return m_fromId;
}

int RealLink::getToId() const 
{
	return m_toId;
}

void RealLink::setFromId( const int arg )
{
	m_fromId = arg;
}

void RealLink::setToId( const int arg )
{
	m_toId = arg;
}

