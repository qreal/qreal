#ifndef __repoclienticeI_h__
#define __repoclienticeI_h__

#include <repoclientice.h>

#include "../qrrepo/repoApi.h"
#include "../qrgui/kernel/definitions.h"

namespace RepoIce
{

class RepoApiI : virtual public RepoApi
{
public:

    virtual ::std::string name(const ::std::string&, const Ice::Current&);

    virtual void setName(const ::std::string&, const ::std::string&, const Ice::Current&);

    virtual ::RepoIce::IdList children(const ::std::string&, const Ice::Current&);

    virtual void addChild(const ::std::string&, const ::std::string&, const Ice::Current&);

    virtual void removeChild(const ::std::string&, const ::std::string&, const Ice::Current&);

    virtual void removeElement(const ::std::string&, const Ice::Current&);

    virtual ::RepoIce::IdList parents(const ::std::string&, const Ice::Current&);

    virtual void addParent(const ::std::string&, const ::std::string&, const Ice::Current&);

    virtual void removeParent(const ::std::string&, const ::std::string&, const Ice::Current&);

    virtual ::RepoIce::IdList outgoingLinks(const ::std::string&, const Ice::Current&);

    virtual ::RepoIce::IdList incomingLinks(const ::std::string&, const Ice::Current&);

    virtual ::RepoIce::IdList links(const ::std::string&, const Ice::Current&);

    virtual ::std::string typeName(const ::std::string&, const Ice::Current&);

    virtual ::std::string property(const ::std::string&, const ::std::string&, const Ice::Current&);

    virtual ::std::string stringProperty(const ::std::string&, const ::std::string&, const Ice::Current&);

    virtual void setProperty(const ::std::string&, const ::std::string&, const ::std::string&, const Ice::Current&);

    virtual void removeProperty(const ::std::string&, const ::std::string&, const Ice::Current&);

    virtual bool hasProperty(const ::std::string&, const ::std::string&, const Ice::Current&);

    virtual ::std::string from(const ::std::string&, const Ice::Current&);

    virtual void setFrom(const ::std::string&, const ::std::string&, const Ice::Current&);

    virtual ::std::string to(const ::std::string&, const Ice::Current&);

    virtual void setTo(const ::std::string&, const ::std::string&, const Ice::Current&);

    virtual ::Ice::Double fromPort(const ::std::string&, const Ice::Current&);

    virtual void setFromPort(const ::std::string&, ::Ice::Double, const Ice::Current&);

    virtual ::Ice::Double toPort(const ::std::string&, const Ice::Current&);

    virtual void setToPort(const ::std::string&, ::Ice::Double, const Ice::Current&);

    virtual ::std::string otherEntityFromLink(const ::std::string&, const ::std::string&, const Ice::Current&);

    virtual void exterminate(const Ice::Current&);

    virtual void save(const Ice::Current&);
	
		static Ice::ObjectAdapterPtr _adapter;

		RepoApiI (::qrRepo::RepoApi &_repoApi) : repoApi(_repoApi)		{}
			

	private:
		
		static ::qReal::Id toqRealId(const ::std::string& str);
		
		::qrRepo::RepoApi &repoApi;
};

}

#endif
