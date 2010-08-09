#include <repoclienticeI.h>

Ice::ObjectAdapterPtr RepoIce::RepoClientIceI::_adapter;      // static member

/* ****************************************************************** *
 * RealTypeIceI
 * ****************************************************************** */

RepoIce::RealTypeIceI::RealTypeIceI (qRealTypes::RealType _realType) : realType(_realType)
{
}

string
RepoIce::RealTypeIceI::getId(const Ice::Current&)
{
    return realType.getId().toStdString();
}

string
RepoIce::RealTypeIceI::getName(const Ice::Current&)
{
    return realType.getName().toStdString();
}

void
RepoIce::RealTypeIceI::setName(const string& name,
                               const Ice::Current&)
{
	realType.setName(QString(name.c_str()));
}

string
RepoIce::RealTypeIceI::getDescription(const Ice::Current&)
{
    return realType.getDescription().toStdString();
}

void
RepoIce::RealTypeIceI::setDescription(const string& description,
                                      const Ice::Current&)
{
	realType.setDescription(QString(description.c_str()));
}

void
RepoIce::RealTypeIceI::setProperty(const string& name,
                                   const string& val,
                                   const Ice::Current&)
{
	realType.setProperty(QString(name.c_str()),QString(val.c_str()));
}

string
RepoIce::RealTypeIceI::getProperty(const string& name,
                                   const Ice::Current&)
{
    return realType.getProperty(QString(name.c_str())).toStdString();
}

int
RepoIce::RealTypeIceI::getPropertiesCount(const Ice::Current&)
{
    return realType.getPropertiesCount();
}

::RepoIce::MetaTypeIce
RepoIce::RealTypeIceI::getMetaTypeIce(const Ice::Current&)
{
    return (MetaTypeIce)realType.getMetaType();
}

void
RepoIce::RealTypeIceI::setMetaTypeIce(::RepoIce::MetaTypeIce mType,
                                      const Ice::Current&)
{
	realType.setMetaType((MetaType)mType);
}

	::RepoIce::IdTypeList
RepoIce::RealTypeIceI::getObjects(const Ice::Current&)
{
	qRealTypes::IdTypeList idTypeList = realType.getObjects();
	::RepoIce::IdTypeList result;
	for (qRealTypes::IdTypeList::const_iterator i = idTypeList.begin(); i != idTypeList.end(); i++)
		result.push_back((*i).toStdString());

	return result;
}

/* ****************************************************************** *
 * RealObjectIceI
 * ****************************************************************** */

RepoIce::RealObjectIceI::RealObjectIceI (qRealTypes::RealObject _realObject): realObject(_realObject)
{
}

string
RepoIce::RealObjectIceI::getId(const Ice::Current&)
{
	return realObject.getId().toStdString();
}

string
RepoIce::RealObjectIceI::getName(const Ice::Current&)
{
    return realObject.getName().toStdString();
}

void
RepoIce::RealObjectIceI::setName(const string& name,
                                 const Ice::Current&)
{
	realObject.setName(QString(name.c_str()));
}

string
RepoIce::RealObjectIceI::getDescription(const Ice::Current&)
{
    return realObject.getDescription().toStdString();
}

void
RepoIce::RealObjectIceI::setDescription(const string& description,
                                        const Ice::Current&)
{
	realObject.setDescription(QString(description.c_str()));
}

void
RepoIce::RealObjectIceI::setProperty(const string& name,
                                     const string& val,
                                     const Ice::Current&)
{
	realObject.setProperty(QString(name.c_str()),QString(val.c_str()));
}

string
RepoIce::RealObjectIceI::getProperty(const string& name,
                                     const Ice::Current&)
{
    return realObject.getProperty(QString(name.c_str())).toStdString();
}

int
RepoIce::RealObjectIceI::getPropertiesCount(const Ice::Current&)
{
    return realObject.getPropertiesCount();
}

string
RepoIce::RealObjectIceI::getTypeId(const Ice::Current&)
{
	return realObject.getTypeId().toStdString();
}

void
		RepoIce::RealObjectIceI::setTypeId(const string& id,
                                   const Ice::Current&)
{
	realObject.setTypeId(QString(id.c_str()));
}

bool
RepoIce::RealObjectIceI::getVisibility(const Ice::Current&)
{
    return realObject.getVisibility();
}

void
RepoIce::RealObjectIceI::setVisibility(bool is,
                                       const Ice::Current&)
{
	realObject.setVisibility(is);
}

::RepoIce::IdTypeList
RepoIce::RealObjectIceI::getContainerId(const Ice::Current&)
{
	qRealTypes::IdTypeList idTypeList = realObject.getContainerId();
	::RepoIce::IdTypeList result;
	for (qRealTypes::IdTypeList::const_iterator i = idTypeList.begin(); i != idTypeList.end(); i++)
		result.push_back((*i).toStdString());

	return result;
}


void 
		RepoIce::RealObjectIceI::addToContainer(const string& id, const Ice::Current&)
{
	realObject.addToContainer(QString(id.c_str()));
}

void 
		RepoIce::RealObjectIceI::deleteFromContainer(const string& id, const Ice::Current&)
{
	realObject.deleteFromContainer(QString(id.c_str()));
}

string
RepoIce::RealObjectIceI::getConfiguration(const Ice::Current&)
{
		//TODO
    //return realObject.getConfiguration().toStdString();
		return string();
}

void
RepoIce::RealObjectIceI::setConfiguration(const string&,
                                          const Ice::Current&)
{
	//TODO
	//realObject.setConfiguration(QString(configuration.c_str()));
}

::RepoIce::IdTypeList
RepoIce::RealObjectIceI::getChildElements(const Ice::Current&)
{
	qRealTypes::IdTypeList idTypeList = realObject.getChildElements();
	::RepoIce::IdTypeList result;
	for (qRealTypes::IdTypeList::const_iterator i = idTypeList.begin(); i != idTypeList.end(); i++)
		result.push_back((*i).toStdString());

	return result;
}

void
		RepoIce::RealObjectIceI::addChildElement(const string& id,
                                         const Ice::Current&)
{
	realObject.addChildElement(QString(id.c_str()));
}

void
		RepoIce::RealObjectIceI::deleteChildElement(const string& id,
                                            const Ice::Current&)
{
	realObject.deleteChildElement(QString(id.c_str()));
}

::RepoIce::IdTypeList
RepoIce::RealObjectIceI::getAllLinks(const Ice::Current&)
{
	qRealTypes::IdTypeList idTypeList = realObject.getAllLinks();
	::RepoIce::IdTypeList result;
	for (qRealTypes::IdTypeList::const_iterator i = idTypeList.begin(); i != idTypeList.end(); i++)
		result.push_back((*i).toStdString());
	return result;
}

::RepoIce::IdTypeList
RepoIce::RealObjectIceI::getIncomingLinks(const Ice::Current&)
{
		qRealTypes::IdTypeList idTypeList = realObject.getIncomingLinks();
	::RepoIce::IdTypeList result;
	for (qRealTypes::IdTypeList::const_iterator i = idTypeList.begin(); i != idTypeList.end(); i++)
		result.push_back((*i).toStdString());
	return result;
}

::RepoIce::IdTypeList
RepoIce::RealObjectIceI::getOutcomingLinks(const Ice::Current&)
{
	qRealTypes::IdTypeList idTypeList = realObject.getOutcomingLinks();
	::RepoIce::IdTypeList result;
	for (qRealTypes::IdTypeList::const_iterator i = idTypeList.begin(); i != idTypeList.end(); i++)
		result.push_back((*i).toStdString());
	return result;
}

void
		RepoIce::RealObjectIceI::addIncomingLink(const string& id,
                                         const Ice::Current&)
{
	realObject.addIncomingLink(QString(id.c_str()));
}

void
		RepoIce::RealObjectIceI::addOutcomingLink(const string& id,
                                          const Ice::Current&)
{
	realObject.addOutcomingLink(QString(id.c_str()));
}

void
		RepoIce::RealObjectIceI::removeIncomingLink(const string& id,
                                         const Ice::Current&)
{
	realObject.removeIncomingLink(QString(id.c_str()));
}

void
		RepoIce::RealObjectIceI::removeOutcomingLink(const string& id,
                                          const Ice::Current&)
{
	realObject.removeOutcomingLink(QString(id.c_str()));
}

/* ****************************************************************** *
 * RealLinkIceI
 * ****************************************************************** */

RepoIce::RealLinkIceI::RealLinkIceI (qRealTypes::RealLink _realLink) : realLink(_realLink)
{
}

string
RepoIce::RealLinkIceI::getId(const Ice::Current&)
{
	return realLink.getId().toStdString();
}

void
RepoIce::RealLinkIceI::setName(const string& name,
                               const Ice::Current&)
{
	realLink.setName(QString(name.c_str()));
}

string
RepoIce::RealLinkIceI::getName(const Ice::Current&)
{
	return realLink.getName().toStdString();
}

void
RepoIce::RealLinkIceI::setProperty(const string& name,
                                   const string& val,
                                   const Ice::Current&)
{
	realLink.setProperty(QString(name.c_str()),QString(val.c_str()));
}

string
RepoIce::RealLinkIceI::getProperty(const string& name,
                                   const Ice::Current&)
{
    return realLink.getProperty(QString(name.c_str())).toStdString();
}

int
RepoIce::RealLinkIceI::getPropertiesCount(const Ice::Current&)
{
    return realLink.getPropertiesCount();
}

string
RepoIce::RealLinkIceI::getFromId(const Ice::Current&)
{
	return realLink.getFromId().toStdString();
}

void
		RepoIce::RealLinkIceI::setFromId(const string& id,
                                 const Ice::Current&)
{
	realLink.setFromId(QString(id.c_str()));
}

string
RepoIce::RealLinkIceI::getToId(const Ice::Current&)
{
	return realLink.getToId().toStdString();
}

void
		RepoIce::RealLinkIceI::setToId(const string& id,
                               const Ice::Current&)
{
	realLink.setToId(QString(id.c_str()));
}
string
		RepoIce::RealLinkIceI::getTypeId(const Ice::Current&)
{
	return realLink.getTypeId().toStdString();
}

void
		RepoIce::RealLinkIceI::setTypeId(const string& id,
																			 const Ice::Current&)
{
	realLink.setTypeId(QString(id.c_str()));
}

/* ****************************************************************** *
 * RepoClientIceI
 * ****************************************************************** */

RepoIce::RepoClientIceI::RepoClientIceI()
{
	repoClient = new RealRepoClient("127.0.0.1", 6666);
}

RepoIce::RepoClientIceI::~RepoClientIceI()
{
	delete repoClient;
}

::RepoIce::IdTypeList
RepoIce::RepoClientIceI::getAllTypes(const Ice::Current&)
{
	qRealTypes::IdTypeList idTypeList = repoClient->getAllTypes();
	::RepoIce::IdTypeList result;
	for (qRealTypes::IdTypeList::const_iterator i = idTypeList.begin(); i != idTypeList.end(); i++)
		result.push_back((*i).toStdString());
	return result;
}

::RepoIce::IdTypeList
RepoIce::RepoClientIceI::getTypesByMetaTypeIce(::RepoIce::MetaTypeIce mType,
                                               const Ice::Current&)
{
	qRealTypes::IdTypeList idTypeList = repoClient->getTypesByMetaType((MetaType)mType);
	::RepoIce::IdTypeList result;
	for (qRealTypes::IdTypeList::const_iterator i = idTypeList.begin(); i != idTypeList.end(); i++)
		result.push_back((*i).toStdString());
	return result;
}

::RepoIce::RealTypeIcePrx
RepoIce::RepoClientIceI::getTypeById(const string& id,
                                     const Ice::Current&)
{
	//FIXME: утечка памяти
	RealTypeIcePtr servant = new RealTypeIceI(repoClient->getTypeById(QString(id.c_str())));
  RealTypeIcePrx proxy = RealTypeIcePrx::uncheckedCast(_adapter->addWithUUID(servant));

	return proxy;
}

::RepoIce::RealTypeIcePrx 
RepoIce::RepoClientIceI::getTypeByName(const string& name , const Ice::Current&)
{
	//FIXME: утечка памяти
  RealTypeIcePtr servant = new RealTypeIceI(repoClient->getTypeByName(QString(name.c_str())));
  RealTypeIcePrx proxy = RealTypeIcePrx::uncheckedCast(_adapter->addWithUUID(servant));

	return proxy;
}

string
RepoIce::RepoClientIceI::getTypeIdByName(const string& name, const Ice::Current& )
{
	return repoClient->getTypeIdByName(QString(name.c_str())).toStdString();
}

	::RepoIce::IdTypeList
RepoIce::RepoClientIceI::getObjectsListByType(const string& id,
                                    const Ice::Current&)
{
	qRealTypes::IdTypeList idTypeList = repoClient->getObjectsListByType(QString(id.c_str()));
	::RepoIce::IdTypeList result;
	for (qRealTypes::IdTypeList::const_iterator i = idTypeList.begin(); i != idTypeList.end(); i++)
		result.push_back((*i).toStdString());

	return result;
}

	::RepoIce::IdTypeList
RepoIce::RepoClientIceI::getLinks(const Ice::Current&)
{
	qRealTypes::IdTypeList idTypeList = repoClient->getLinks();
	::RepoIce::IdTypeList result;
	for (qRealTypes::IdTypeList::const_iterator i = idTypeList.begin(); i != idTypeList.end(); i++)
		result.push_back((*i).toStdString());
	return result;
}


::RepoIce::RealObjectIcePrx
RepoIce::RepoClientIceI::getObjectById(const string& id,
                                       const Ice::Current&)
{
	//FIXME: утечка памяти
	RealObjectIcePtr servant = new RealObjectIceI(repoClient->getObjectById(QString(id.c_str())));
  RealObjectIcePrx proxy = RealObjectIcePrx::uncheckedCast(_adapter->addWithUUID(servant));

	return proxy;
}

string
		RepoIce::RepoClientIceI::createObject(const string& type, const string& name, const Ice::Current&)
{
	return repoClient->createObject(QString(type.c_str()), QString(name.c_str())).toStdString();
}

string
		RepoIce::RepoClientIceI::createObjectWithParent(const string& type, const string& name, const string& parent, const Ice::Current&)
{
	return repoClient->createObjectWithParent(QString(type.c_str()), QString(name.c_str()), QString(parent.c_str())).toStdString();
}

void
		RepoIce::RepoClientIceI::deleteObject(const string& id, const string& parent,
                                      const Ice::Current&)
{
	return repoClient->deleteObject(QString(id.c_str()), QString(parent.c_str()));
}

::RepoIce::RealLinkIcePrx
		RepoIce::RepoClientIceI::getLinkById(const string& id,
                                     const Ice::Current&)
{
	//Todo: утечка памяти
	RealLinkIcePtr servant = new RealLinkIceI(repoClient->getLinkById(QString(id.c_str())));
  RealLinkIcePrx proxy = RealLinkIcePrx::uncheckedCast(_adapter->addWithUUID(servant));

	return proxy;
}

string
RepoIce::RepoClientIceI::createLink(const string& name,
                                    const Ice::Current&)
{
	return repoClient->createLink(QString(name.c_str())).toStdString();
}

string
RepoIce::RepoClientIceI::createLinkWithType(const string& name, const string& type,
																						const Ice::Current&)
{
	return repoClient->createLinkWithType(QString(name.c_str()), QString(type.c_str())).toStdString();
}

string
		RepoIce::RepoClientIceI::createLinkWithParent(const string& type, const string& name, const string& parent,
																						const Ice::Current&)
{
	return repoClient->createLinkWithParent(QString(type.c_str()), QString(name.c_str()), QString(parent.c_str())).toStdString();
}

void
		RepoIce::RepoClientIceI::deleteLink(const string& id, const string& parent,
                                    const Ice::Current&)
{
	return repoClient->deleteLink(QString(id.c_str()), QString(parent.c_str()));
}

