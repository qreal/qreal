#ifndef __repoclienticeI_h__
#define __repoclienticeI_h__
#include <../repoclientice.h>
#include <Ice/Ice.h>

#include "../../common/realrepoapiclasses.h"
#include "../../common/classes.h"
#include "../../repo/realrepoclient.h"

using namespace std;
using namespace qRealTypes;

namespace RepoIce
{
class RealTypeIceI : virtual public RealTypeIce
{
public:
    virtual string getId(const Ice::Current&);

    virtual string getName(const Ice::Current&);
    virtual void setName(const string&, const Ice::Current&);

    virtual string getDescription(const Ice::Current&);
    virtual void setDescription(const string&, const Ice::Current&);

    virtual void setProperty(const string&, const string&, const Ice::Current&);
    virtual string getProperty(const string&, const Ice::Current&);
    virtual int getPropertiesCount(const Ice::Current&);

    virtual ::RepoIce::MetaTypeIce getMetaTypeIce(const Ice::Current&);

    virtual void setMetaTypeIce(::RepoIce::MetaTypeIce, const Ice::Current&);
		virtual ::RepoIce::IdTypeList getObjects(const Ice::Current&);

		RealTypeIceI (RealType);

private:
	RealType realType;
};

class RealObjectIceI : virtual public RealObjectIce
{
public:
    virtual string getId(const Ice::Current&);

    virtual string getName(const Ice::Current&);
    virtual void setName(const string&, const Ice::Current&);

    virtual string getDescription(const Ice::Current&);
    virtual void setDescription(const string&, const Ice::Current&);

    virtual void setProperty(const string&, const string&, const Ice::Current&);
    virtual string getProperty(const string&, const Ice::Current&);
    virtual int getPropertiesCount(const Ice::Current&);

    virtual string getTypeId(const Ice::Current&);
    virtual void setTypeId(const string&, const Ice::Current&);

    virtual bool getVisibility(const Ice::Current&);
    virtual void setVisibility(bool, const Ice::Current&);

    virtual ::RepoIce::IdTypeList getContainerId(const Ice::Current&);
		virtual void addToContainer(const string&, const Ice::Current&);
		virtual void deleteFromContainer(const string&, const Ice::Current&);

    virtual string getConfiguration(const Ice::Current&);
    virtual void setConfiguration(const string&, const Ice::Current&);

    virtual ::RepoIce::IdTypeList getChildElements(const Ice::Current&);
		virtual void addChildElement(const string&, const Ice::Current&);
		virtual void deleteChildElement(const string&, const Ice::Current&);

    virtual ::RepoIce::IdTypeList getAllLinks(const Ice::Current&);
    virtual ::RepoIce::IdTypeList getIncomingLinks(const Ice::Current&);
    virtual ::RepoIce::IdTypeList getOutcomingLinks(const Ice::Current&);

		virtual void addIncomingLink(const string&, const Ice::Current&);
		virtual void addOutcomingLink(const string&, const Ice::Current&);

		virtual void removeIncomingLink(const string&, const Ice::Current&);
		virtual void removeOutcomingLink(const string&, const Ice::Current&);

		RealObjectIceI (RealObject);

private:
	RealObject realObject;
};

class RealLinkIceI : virtual public RealLinkIce
{
public:
    virtual string getId(const Ice::Current&);

    virtual string getName(const Ice::Current&);
    virtual void setName(const string&, const Ice::Current&);

    virtual string getTypeId(const Ice::Current&);
		virtual void setTypeId(const string&, const Ice::Current&);

    virtual void setProperty(const string&, const string&, const Ice::Current&);
    virtual string getProperty(const string&, const Ice::Current&);
    virtual int getPropertiesCount(const Ice::Current&);

    virtual string getFromId(const Ice::Current&);
		virtual void setFromId(const string&, const Ice::Current&);

    virtual string getToId(const Ice::Current&);
		virtual void setToId(const string&, const Ice::Current&);

		RealLinkIceI (RealLink);

private:
	RealLink realLink;
};

class RepoClientIceI : virtual public RepoClientIce
{
public:
		RepoClientIceI();
		virtual ~RepoClientIceI();
    virtual ::RepoIce::IdTypeList getAllTypes(const Ice::Current&);
    virtual ::RepoIce::IdTypeList getTypesByMetaTypeIce(::RepoIce::MetaTypeIce, const Ice::Current&);
    virtual ::RepoIce::RealTypeIcePrx getTypeById(const string&, const Ice::Current&);
		virtual ::RepoIce::RealTypeIcePrx getTypeByName(const string&, const Ice::Current&);
		virtual string getTypeIdByName(const string&, const Ice::Current& );

		virtual ::RepoIce::IdTypeList getObjectsListByType(const string&, const Ice::Current&);
		virtual ::RepoIce::IdTypeList getLinks(const Ice::Current&);


    virtual ::RepoIce::RealObjectIcePrx getObjectById(const string&, const Ice::Current&);
    virtual string createObject(const string&, const string&, const Ice::Current&);
		virtual string createObjectWithParent(const string&, const string&, const string&, const Ice::Current&);
    virtual void deleteObject(const string&, const string&, const Ice::Current&);

		virtual ::RepoIce::RealLinkIcePrx getLinkById(const string&, const Ice::Current&);
    virtual string createLink(const string&, const Ice::Current&);
		virtual string createLinkWithType(const string&, const string&, const Ice::Current&);
		virtual string createLinkWithParent(const string& type, const string& name, const string& parent, const Ice::Current&);
		virtual void deleteLink(const string&, const string&, const Ice::Current&);
		
		static Ice::ObjectAdapterPtr _adapter;

protected:
		RealRepoClient *repoClient;
};
}

#endif

