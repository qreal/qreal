#ifndef __repoclienticeI_h__
#define __repoclienticeI_h__
#include <repoclientice.h>
#include <Ice/Ice.h>

#include "../../common/realrepoapiclasses.h"
#include "../../common/classes.h"
#include "../../repo/realrepoclient.h"

using namespace std;
namespace RepoIce
{
class RealTypeIceI : virtual public RealTypeIce
{
public:
    virtual int getId(const Ice::Current&);
    virtual string getName(const Ice::Current&);
    virtual void setName(const string&, const Ice::Current&);
    virtual string getDescription(const Ice::Current&);
    virtual void setDescription(const string&, const Ice::Current&);
    virtual void setProperty(const string&, const string&, const Ice::Current&);
    virtual string getProperty(const string&, const Ice::Current&);
    virtual int getPropertiesCount(const Ice::Current&);
    virtual ::RepoIce::MetaTypeIce getMetaTypeIce(const Ice::Current&);
    virtual void setMetaTypeIce(::RepoIce::MetaTypeIce, const Ice::Current&);
    virtual ::RepoIce::QIntList getObjects(const Ice::Current&);
		RealTypeIceI (QRealTypes::RealType);
private:
	RealType realType;
};
class RealObjectIceI : virtual public RealObjectIce
{
public:
    virtual int getId(const Ice::Current&);
    virtual string getName(const Ice::Current&);
    virtual void setName(const string&, const Ice::Current&);
    virtual string getDescription(const Ice::Current&);
    virtual void setDescription(const string&, const Ice::Current&);
    virtual void setProperty(const string&, const string&, const Ice::Current&);
    virtual string getProperty(const string&, const Ice::Current&);
    virtual int getPropertiesCount(const Ice::Current&);
    virtual int getTypeId(const Ice::Current&);
    virtual void setTypeId(int, const Ice::Current&);
    virtual bool getVisibility(const Ice::Current&);
    virtual void setVisibility(bool, const Ice::Current&);
    virtual int getContainerId(const Ice::Current&);
    virtual void setContainerId(int, const Ice::Current&);
    virtual string getConfiguration(const Ice::Current&);
    virtual void setConfiguration(const string&, const Ice::Current&);
    virtual ::RepoIce::QIntList getChildElements(const Ice::Current&);
    virtual void addChildElement(int, const Ice::Current&);
    virtual void deleteChildElement(int, const Ice::Current&);
    virtual ::RepoIce::QIntList getAllLinks(const Ice::Current&);
    virtual ::RepoIce::QIntList getIncomingLinks(const Ice::Current&);
    virtual ::RepoIce::QIntList getOutcomingLinks(const Ice::Current&);
    virtual void addIncomingLink(int, const Ice::Current&);
    virtual void addOutcomingLink(int, const Ice::Current&);
		RealObjectIceI (QRealTypes::RealObject);
private:
	RealObject realObject;
};
class RealLinkIceI : virtual public RealLinkIce
{
public:
    virtual int getId(const Ice::Current&);
    virtual void setName(const string&, const Ice::Current&);
    virtual void setProperty(const string&, const string&, const Ice::Current&);
    virtual string getProperty(const string&, const Ice::Current&);
    virtual int getPropertiesCount(const Ice::Current&);
    virtual int getFromId(const Ice::Current&);
    virtual void setFromId(int, const Ice::Current&);
    virtual int getToId(const Ice::Current&);
    virtual void setToId(int, const Ice::Current&);
		RealLinkIceI (QRealTypes::RealLink);
private:
	RealLink realLink;
};
class RepoClientIceI : virtual public RepoClientIce
{
public:
    virtual ::RepoIce::QIntList getAllTypes(const Ice::Current&);
    virtual ::RepoIce::QIntList getTypesByMetaTypeIce(::RepoIce::MetaTypeIce, const Ice::Current&);
    virtual ::RepoIce::RealTypeIcePrx getTypeById(int, const Ice::Current&);
    virtual ::RepoIce::QIntList getObjectsListByType(int, const Ice::Current&);
    virtual int createType(const string&, const Ice::Current&);
    virtual void deleteType(int, const Ice::Current&);
    virtual ::RepoIce::RealObjectIcePrx getObjectById(int, const Ice::Current&);
    virtual int createEntity(int, const string&, const Ice::Current&);
    virtual int createEntityWithParent(int, const string&, int, const Ice::Current&);
    virtual void deleteObject(int, const Ice::Current&);
    virtual ::RepoIce::RealLinkIcePrx getLinkById(int, const Ice::Current&);
    virtual int createLink(const string&, const Ice::Current&);
    virtual void deleteLink(int, const Ice::Current&);
		
		static Ice::ObjectAdapterPtr _adapter;
protected:
	RealRepoClient repoClient;
};
}
#endif
