#include <repoclienticeI.h>

Ice::ObjectAdapterPtr RepoIce::RepoApiI::_adapter;      // static member


::qReal::Id
RepoIce::RepoApiI::toqRealId(const ::std::string& str)
{
	return qReal::Id::loadFromString(QString::fromStdString(str));
}

::std::string
RepoIce::RepoApiI::name(const ::std::string& id, const Ice::Current& )
{
	return repoApi.name( toqRealId(id) ).toStdString();
}

void
RepoIce::RepoApiI::setName(const ::std::string& id, const ::std::string& name, const Ice::Current& )
{
	repoApi.setName(qReal::Id::loadFromString(QString::fromStdString(id)),QString::fromStdString(name));
}

::RepoIce::IdList
RepoIce::RepoApiI::children(const ::std::string& id, const Ice::Current& )
{
	qReal::IdList list = repoApi.children(toqRealId(id));
	::RepoIce::IdList result;
	
	for (qReal::IdList::const_iterator i = list.begin(); i != list.end(); i++)
		result.push_back((*i).toString().toStdString());
	
	return result;
}

void
RepoIce::RepoApiI::addChild(const ::std::string& id, const ::std::string& child, const Ice::Current& )
{
	repoApi.addChild(toqRealId(id), toqRealId(child));
}

void
RepoIce::RepoApiI::removeChild(const ::std::string& id, const ::std::string& child, const Ice::Current& )
{
	repoApi.removeChild(toqRealId(id), toqRealId(child));
}

void
RepoIce::RepoApiI::removeElement(const ::std::string& id, const Ice::Current& )
{
	repoApi.removeElement(toqRealId(id));
}

::RepoIce::IdList
RepoIce::RepoApiI::parents(const ::std::string& id, const Ice::Current& )
{
	qReal::IdList list = repoApi.parents(toqRealId(id));
	::RepoIce::IdList result;
	
	for (qReal::IdList::const_iterator i = list.begin(); i != list.end(); i++)
		result.push_back((*i).toString().toStdString());
	
	return result;
}

void
RepoIce::RepoApiI::addParent(const ::std::string& id, const ::std::string& parent, const Ice::Current& )
{
	repoApi.addParent(toqRealId(id), toqRealId(parent));
}

void
RepoIce::RepoApiI::removeParent(const ::std::string& id, const ::std::string& parent, const Ice::Current& )
{
	repoApi.removeParent(toqRealId(id), toqRealId(parent));
}

::RepoIce::IdList
RepoIce::RepoApiI::outcomingLinks(const ::std::string& id, const Ice::Current& )
{
	qReal::IdList list = repoApi.outcomingLinks(toqRealId(id));
	::RepoIce::IdList result;
	
	for (qReal::IdList::const_iterator i = list.begin(); i != list.end(); i++)
		result.push_back((*i).toString().toStdString());
	
	return result;
}

::RepoIce::IdList
RepoIce::RepoApiI::incomingLinks(const ::std::string& id, const Ice::Current& )
{
	qReal::IdList list = repoApi.incomingLinks(toqRealId(id));
	::RepoIce::IdList result;
	
	for (qReal::IdList::const_iterator i = list.begin(); i != list.end(); i++)
		result.push_back((*i).toString().toStdString());
	
	return result;
}

::RepoIce::IdList
RepoIce::RepoApiI::links(const ::std::string& id, const Ice::Current& )
{
	qReal::IdList list = repoApi.links(toqRealId(id));
	::RepoIce::IdList result;
	
	for (qReal::IdList::const_iterator i = list.begin(); i != list.end(); i++)
		result.push_back((*i).toString().toStdString());
	
	return result;
}

::std::string
RepoIce::RepoApiI::typeName(const ::std::string& id, const Ice::Current& )
{
	return repoApi.typeName( toqRealId(id) ).toStdString();
}

::std::string
RepoIce::RepoApiI::property(const ::std::string& id, const ::std::string& propertyName, const Ice::Current& )
{
	return repoApi.property(toqRealId(id), QString::fromStdString(propertyName)).toString().toStdString();
}

::std::string
RepoIce::RepoApiI::stringProperty(const ::std::string& id, const ::std::string& propertyName, const Ice::Current& )
{
	return repoApi.stringProperty(toqRealId(id), QString::fromStdString(propertyName)).toStdString();
}

void
RepoIce::RepoApiI::setProperty(const ::std::string& id, const ::std::string& propertyName, const ::std::string& value, const Ice::Current& )
{
	repoApi.setProperty(toqRealId(id), QString::fromStdString(propertyName), QString::fromStdString(value));
}

void
RepoIce::RepoApiI::removeProperty(const ::std::string& id, const ::std::string& propertyName, const Ice::Current& )
{
	repoApi.removeProperty(toqRealId(id), QString::fromStdString(propertyName));
}

bool
RepoIce::RepoApiI::hasProperty(const ::std::string& id, const ::std::string& propertyName, const Ice::Current& )
{
	return repoApi.hasProperty(toqRealId(id), QString::fromStdString(propertyName));
}

::std::string
RepoIce::RepoApiI::from(const ::std::string& id, const Ice::Current& )
{
	return repoApi.from(toqRealId(id)).toString().toStdString();
}

void
RepoIce::RepoApiI::setFrom(const ::std::string& id, const ::std::string& from, const Ice::Current& )
{
	return repoApi.setFrom(toqRealId(id), toqRealId(from));
}

::std::string
RepoIce::RepoApiI::to(const ::std::string& id, const Ice::Current& )
{
	return repoApi.to(toqRealId(id)).toString().toStdString();
}

void
RepoIce::RepoApiI::setTo(const ::std::string& id, const ::std::string& to, const Ice::Current& )
{
	repoApi.setFrom(toqRealId(id), toqRealId(to));
}

::Ice::Double
RepoIce::RepoApiI::fromPort(const ::std::string& id, const Ice::Current& )
{
	return repoApi.fromPort(toqRealId(id));
}

void
RepoIce::RepoApiI::setFromPort(const ::std::string& id, ::Ice::Double fromPort, const Ice::Current& )
{
	repoApi.setFromPort(toqRealId(id), fromPort);
}

::Ice::Double
RepoIce::RepoApiI::toPort(const ::std::string& id, const Ice::Current& )
{
	return repoApi.toPort(toqRealId(id));
}

void
RepoIce::RepoApiI::setToPort(const ::std::string& id, ::Ice::Double toPort, const Ice::Current& )
{
	repoApi.setToPort(toqRealId(id), toPort);
}

::std::string
RepoIce::RepoApiI::otherEntityFromLink(const ::std::string& linkId, const ::std::string& firstNode, const Ice::Current& )
{
	return repoApi.otherEntityFromLink(toqRealId(linkId), toqRealId(firstNode)).toString().toStdString();
}

void
RepoIce::RepoApiI::exterminate(const Ice::Current& )
{
	repoApi.exterminate();
}

void
RepoIce::RepoApiI::save(const Ice::Current& )
{
	repoApi.save();
}
