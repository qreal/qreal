#include <repoclienticeI.h>

Ice::ObjectAdapterPtr RepoIce::RepoClientIceI::_adapter;      // static member

RepoIce::RealTypeIceI::RealTypeIceI (QRealTypes::RealType _realType) : realType(_realType)
{
}

int
RepoIce::RealTypeIceI::getId(const Ice::Current&)
{
    return realType.getId();
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

::RepoIce::QIntList
RepoIce::RealTypeIceI::getObjects(const Ice::Current&)
{
    return realType.getObjects().toVector().toStdVector();
}
		
RepoIce::RealObjectIceI::RealObjectIceI (QRealTypes::RealObject _realObject): realObject(_realObject)
{
}

int
RepoIce::RealObjectIceI::getId(const Ice::Current&)
{
    return realObject.getId();
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

int
RepoIce::RealObjectIceI::getTypeId(const Ice::Current&)
{
    return realObject.getTypeId();
}

void
RepoIce::RealObjectIceI::setTypeId(int id,
                                   const Ice::Current&)
{
	realObject.setTypeId(id);
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

int
RepoIce::RealObjectIceI::getContainerId(const Ice::Current&)
{
    return realObject.getContainerId();
}

void
RepoIce::RealObjectIceI::setContainerId(int id,
                                        const Ice::Current&)
{
	realObject.setContainerId(id);
}

string
RepoIce::RealObjectIceI::getConfiguration(const Ice::Current&)
{
    return realObject.getConfiguration().toStdString();
}

void
RepoIce::RealObjectIceI::setConfiguration(const string& configuration,
                                          const Ice::Current&)
{
	realObject.setConfiguration(QString(configuration.c_str()));
}

::RepoIce::QIntList
RepoIce::RealObjectIceI::getChildElements(const Ice::Current&)
{
    return realObject.getChildElements().toVector().toStdVector();
}

void
RepoIce::RealObjectIceI::addChildElement(int elementId,
                                         const Ice::Current&)
{
	realObject.addChildElement(elementId);
}

void
RepoIce::RealObjectIceI::deleteChildElement(int elementId,
                                            const Ice::Current&)
{
	realObject.deleteChildElement(elementId);
}

::RepoIce::QIntList
RepoIce::RealObjectIceI::getAllLinks(const Ice::Current&)
{
    return realObject.getAllLinks().toVector().toStdVector(); 
}

::RepoIce::QIntList
RepoIce::RealObjectIceI::getIncomingLinks(const Ice::Current&)
{
    return realObject.getIncomingLinks().toVector().toStdVector();
}

::RepoIce::QIntList
RepoIce::RealObjectIceI::getOutcomingLinks(const Ice::Current&)
{
    return realObject.getOutcomingLinks().toVector().toStdVector();
}

void
RepoIce::RealObjectIceI::addIncomingLink(int linkId,
                                         const Ice::Current&)
{
	realObject.addIncomingLink(linkId);
}

void
RepoIce::RealObjectIceI::addOutcomingLink(int linkId,
                                          const Ice::Current&)
{
	realObject.addOutcomingLink(linkId);
}

RepoIce::RealLinkIceI::RealLinkIceI (QRealTypes::RealLink _realLink) : realLink(_realLink)
{
}

int
RepoIce::RealLinkIceI::getId(const Ice::Current&)
{
    return realLink.getId();
}

void
RepoIce::RealLinkIceI::setName(const string& name,
                               const Ice::Current&)
{
	//realLink.setName(name);
		throw "Not Implemented";
}

void
RepoIce::RealLinkIceI::setProperty(const string& name,
                                   const string& val,
                                   const Ice::Current&)
{
	//realLink.setProperty(name,val);
		throw "Not Implemented";
}

string
RepoIce::RealLinkIceI::getProperty(const string& name,
                                   const Ice::Current&)
{
    //return realLink.getProperty(name);
		throw "Not Implemented";
		return 0;
}

int
RepoIce::RealLinkIceI::getPropertiesCount(const Ice::Current&)
{
    //return realLink.getPropertiesCount()
		throw "Not Implemented";
		return 0;
}

int
RepoIce::RealLinkIceI::getFromId(const Ice::Current&)
{
    return realLink.getFromId();
}

void
RepoIce::RealLinkIceI::setFromId(int id,
                                 const Ice::Current&)
{
	realLink.setFromId(id);
}

int
RepoIce::RealLinkIceI::getToId(const Ice::Current&)
{
    return realLink.getToId();
}

void
RepoIce::RealLinkIceI::setToId(int id,
                               const Ice::Current&)
{
	realLink.setToId(id);
}

::RepoIce::QIntList
RepoIce::RepoClientIceI::getAllTypes(const Ice::Current&)
{
    return repoClient.getAllTypes().toVector().toStdVector();
}

::RepoIce::QIntList
RepoIce::RepoClientIceI::getTypesByMetaTypeIce(::RepoIce::MetaTypeIce mType,
                                               const Ice::Current&)
{
    return repoClient.getTypesByMetaType((MetaType)mType).toVector().toStdVector();
}

::RepoIce::RealTypeIcePrx
RepoIce::RepoClientIceI::getTypeById(int id,
                                     const Ice::Current&)
{
	//Todo: не понятно когда будет освобождена память. Нужно проверить.
  RealTypeIcePtr servant = new RealTypeIceI(repoClient.getTypeById(id));
  RealTypeIcePrx proxy = RealTypeIcePrx::uncheckedCast(_adapter->addWithUUID(servant));

	return proxy;
}

::RepoIce::QIntList
RepoIce::RepoClientIceI::getObjectsListByType(int typeId,
                                    const Ice::Current&)
{
    return repoClient.getObjectsListByType(typeId).toVector().toStdVector();
}

int
RepoIce::RepoClientIceI::createType(const string& name,
                                    const Ice::Current&)
{
		//Seems it's deprecated
    //return repoClient.createType(QString(name.c_str()));
		throw "Deprecated";
		//return 0;
}

void
RepoIce::RepoClientIceI::deleteType(int id,
                                    const Ice::Current&)
{
	//Seems it's deprecated
	//repoClient.deleteType(id);
		throw "Deprecated";
}

::RepoIce::RealObjectIcePrx
RepoIce::RepoClientIceI::getObjectById(int id,
                                       const Ice::Current&)
{
	//Todo: не понятно когда будет освобождена память. Нужно проверить.
  RealObjectIcePtr servant = new RealObjectIceI(repoClient.getObjectById(id));
  RealObjectIcePrx proxy = RealObjectIcePrx::uncheckedCast(_adapter->addWithUUID(servant));

	return proxy;
}

int
RepoIce::RepoClientIceI::createEntity(int type, const string& name, const Ice::Current&)
{
	return repoClient.createEntity(type, QString(name.c_str()));
}

int
RepoIce::RepoClientIceI::createEntityWithParent(int type, const string& name, int parent, const Ice::Current&)
{
	return repoClient.createEntityWithParent(type, QString(name.c_str()), parent);
}

void
RepoIce::RepoClientIceI::deleteObject(int id,
                                      const Ice::Current&)
{
		throw "Not Implemented";
    //return repoClient.deleteObject(QString(name.c_str()));
}

::RepoIce::RealLinkIcePrx
RepoIce::RepoClientIceI::getLinkById(int id,
                                     const Ice::Current&)
{
	//Todo: не понятно когда будет освобождена память. Нужно проверить.
  RealLinkIcePtr servant = new RealLinkIceI(repoClient.getLinkById(id));
  RealLinkIcePrx proxy = RealLinkIcePrx::uncheckedCast(_adapter->addWithUUID(servant));

	return proxy;
}

int
RepoIce::RepoClientIceI::createLink(const string& name,
                                    const Ice::Current&)
{
		throw "not implemented";
    //return repoClient.createLink(QString(name.c_str()));
}

void
RepoIce::RepoClientIceI::deleteLink(int id,
                                    const Ice::Current&)
{
		throw "not implemented";
    //return repoClient.deleteLink(QString(name.c_str()));
}
