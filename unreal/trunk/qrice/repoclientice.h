// **********************************************************************
//
// Copyright (c) 2003-2008 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.0
// Generated from file `repoclientice.ice'

#ifndef __repoclientice_h__
#define __repoclientice_h__

#include <Ice/LocalObjectF.h>
#include <Ice/ProxyF.h>
#include <Ice/ObjectF.h>
#include <Ice/Exception.h>
#include <Ice/LocalObject.h>
#include <Ice/Proxy.h>
#include <Ice/Object.h>
#include <Ice/Outgoing.h>
#include <Ice/Incoming.h>
#include <Ice/Direct.h>
#include <Ice/StreamF.h>
#include <Ice/UndefSysMacros.h>

#ifndef ICE_IGNORE_VERSION
#   if ICE_INT_VERSION / 100 != 303
#       error Ice version mismatch!
#   endif
#   if ICE_INT_VERSION % 100 > 50
#       error Beta header file detected
#   endif
#   if ICE_INT_VERSION % 100 < 0
#       error Ice patch level mismatch!
#   endif
#endif

namespace IceProxy
{

namespace RepoIce
{

class RepoApi;

}

}

namespace RepoIce
{

class RepoApi;
bool operator==(const RepoApi&, const RepoApi&);
bool operator<(const RepoApi&, const RepoApi&);

}

namespace IceInternal
{

::Ice::Object* upCast(::RepoIce::RepoApi*);
::IceProxy::Ice::Object* upCast(::IceProxy::RepoIce::RepoApi*);

}

namespace RepoIce
{

typedef ::IceInternal::Handle< ::RepoIce::RepoApi> RepoApiPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::RepoIce::RepoApi> RepoApiPrx;

void __read(::IceInternal::BasicStream*, RepoApiPrx&);
void __patch__RepoApiPtr(void*, ::Ice::ObjectPtr&);

}

namespace RepoIce
{

typedef ::std::vector< ::std::string> IdList;

const ::std::string ROOTID = "qrm:/ROOT_ID/ROOT_ID/ROOT_ID/ROOT_ID";

}

namespace IceProxy
{

namespace RepoIce
{

class RepoApi : virtual public ::IceProxy::Ice::Object
{
public:

    ::std::string name(const ::std::string& id)
    {
        return name(id, 0);
    }
    ::std::string name(const ::std::string& id, const ::Ice::Context& __ctx)
    {
        return name(id, &__ctx);
    }
    
private:

    ::std::string name(const ::std::string&, const ::Ice::Context*);
    
public:

    void setName(const ::std::string& id, const ::std::string& name)
    {
        setName(id, name, 0);
    }
    void setName(const ::std::string& id, const ::std::string& name, const ::Ice::Context& __ctx)
    {
        setName(id, name, &__ctx);
    }
    
private:

    void setName(const ::std::string&, const ::std::string&, const ::Ice::Context*);
    
public:

    ::RepoIce::IdList children(const ::std::string& id)
    {
        return children(id, 0);
    }
    ::RepoIce::IdList children(const ::std::string& id, const ::Ice::Context& __ctx)
    {
        return children(id, &__ctx);
    }
    
private:

    ::RepoIce::IdList children(const ::std::string&, const ::Ice::Context*);
    
public:

    void addChild(const ::std::string& id, const ::std::string& child)
    {
        addChild(id, child, 0);
    }
    void addChild(const ::std::string& id, const ::std::string& child, const ::Ice::Context& __ctx)
    {
        addChild(id, child, &__ctx);
    }
    
private:

    void addChild(const ::std::string&, const ::std::string&, const ::Ice::Context*);
    
public:

    void removeChild(const ::std::string& id, const ::std::string& child)
    {
        removeChild(id, child, 0);
    }
    void removeChild(const ::std::string& id, const ::std::string& child, const ::Ice::Context& __ctx)
    {
        removeChild(id, child, &__ctx);
    }
    
private:

    void removeChild(const ::std::string&, const ::std::string&, const ::Ice::Context*);
    
public:

    void removeElement(const ::std::string& id)
    {
        removeElement(id, 0);
    }
    void removeElement(const ::std::string& id, const ::Ice::Context& __ctx)
    {
        removeElement(id, &__ctx);
    }
    
private:

    void removeElement(const ::std::string&, const ::Ice::Context*);
    
public:

    ::RepoIce::IdList parents(const ::std::string& id)
    {
        return parents(id, 0);
    }
    ::RepoIce::IdList parents(const ::std::string& id, const ::Ice::Context& __ctx)
    {
        return parents(id, &__ctx);
    }
    
private:

    ::RepoIce::IdList parents(const ::std::string&, const ::Ice::Context*);
    
public:

    void addParent(const ::std::string& id, const ::std::string& parent)
    {
        addParent(id, parent, 0);
    }
    void addParent(const ::std::string& id, const ::std::string& parent, const ::Ice::Context& __ctx)
    {
        addParent(id, parent, &__ctx);
    }
    
private:

    void addParent(const ::std::string&, const ::std::string&, const ::Ice::Context*);
    
public:

    void removeParent(const ::std::string& id, const ::std::string& parent)
    {
        removeParent(id, parent, 0);
    }
    void removeParent(const ::std::string& id, const ::std::string& parent, const ::Ice::Context& __ctx)
    {
        removeParent(id, parent, &__ctx);
    }
    
private:

    void removeParent(const ::std::string&, const ::std::string&, const ::Ice::Context*);
    
public:

    ::RepoIce::IdList outcomingLinks(const ::std::string& id)
    {
        return outcomingLinks(id, 0);
    }
    ::RepoIce::IdList outcomingLinks(const ::std::string& id, const ::Ice::Context& __ctx)
    {
        return outcomingLinks(id, &__ctx);
    }
    
private:

    ::RepoIce::IdList outcomingLinks(const ::std::string&, const ::Ice::Context*);
    
public:

    ::RepoIce::IdList incomingLinks(const ::std::string& id)
    {
        return incomingLinks(id, 0);
    }
    ::RepoIce::IdList incomingLinks(const ::std::string& id, const ::Ice::Context& __ctx)
    {
        return incomingLinks(id, &__ctx);
    }
    
private:

    ::RepoIce::IdList incomingLinks(const ::std::string&, const ::Ice::Context*);
    
public:

    ::RepoIce::IdList links(const ::std::string& id)
    {
        return links(id, 0);
    }
    ::RepoIce::IdList links(const ::std::string& id, const ::Ice::Context& __ctx)
    {
        return links(id, &__ctx);
    }
    
private:

    ::RepoIce::IdList links(const ::std::string&, const ::Ice::Context*);
    
public:

    ::std::string typeName(const ::std::string& id)
    {
        return typeName(id, 0);
    }
    ::std::string typeName(const ::std::string& id, const ::Ice::Context& __ctx)
    {
        return typeName(id, &__ctx);
    }
    
private:

    ::std::string typeName(const ::std::string&, const ::Ice::Context*);
    
public:

    ::std::string property(const ::std::string& id, const ::std::string& propertyName)
    {
        return property(id, propertyName, 0);
    }
    ::std::string property(const ::std::string& id, const ::std::string& propertyName, const ::Ice::Context& __ctx)
    {
        return property(id, propertyName, &__ctx);
    }
    
private:

    ::std::string property(const ::std::string&, const ::std::string&, const ::Ice::Context*);
    
public:

    ::std::string stringProperty(const ::std::string& id, const ::std::string& propertyName)
    {
        return stringProperty(id, propertyName, 0);
    }
    ::std::string stringProperty(const ::std::string& id, const ::std::string& propertyName, const ::Ice::Context& __ctx)
    {
        return stringProperty(id, propertyName, &__ctx);
    }
    
private:

    ::std::string stringProperty(const ::std::string&, const ::std::string&, const ::Ice::Context*);
    
public:

    void setProperty(const ::std::string& id, const ::std::string& propertyName, const ::std::string& value)
    {
        setProperty(id, propertyName, value, 0);
    }
    void setProperty(const ::std::string& id, const ::std::string& propertyName, const ::std::string& value, const ::Ice::Context& __ctx)
    {
        setProperty(id, propertyName, value, &__ctx);
    }
    
private:

    void setProperty(const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Context*);
    
public:

    void removeProperty(const ::std::string& id, const ::std::string& propertyName)
    {
        removeProperty(id, propertyName, 0);
    }
    void removeProperty(const ::std::string& id, const ::std::string& propertyName, const ::Ice::Context& __ctx)
    {
        removeProperty(id, propertyName, &__ctx);
    }
    
private:

    void removeProperty(const ::std::string&, const ::std::string&, const ::Ice::Context*);
    
public:

    bool hasProperty(const ::std::string& id, const ::std::string& propertyName)
    {
        return hasProperty(id, propertyName, 0);
    }
    bool hasProperty(const ::std::string& id, const ::std::string& propertyName, const ::Ice::Context& __ctx)
    {
        return hasProperty(id, propertyName, &__ctx);
    }
    
private:

    bool hasProperty(const ::std::string&, const ::std::string&, const ::Ice::Context*);
    
public:

    ::std::string from(const ::std::string& id)
    {
        return from(id, 0);
    }
    ::std::string from(const ::std::string& id, const ::Ice::Context& __ctx)
    {
        return from(id, &__ctx);
    }
    
private:

    ::std::string from(const ::std::string&, const ::Ice::Context*);
    
public:

    void setFrom(const ::std::string& id, const ::std::string& from)
    {
        setFrom(id, from, 0);
    }
    void setFrom(const ::std::string& id, const ::std::string& from, const ::Ice::Context& __ctx)
    {
        setFrom(id, from, &__ctx);
    }
    
private:

    void setFrom(const ::std::string&, const ::std::string&, const ::Ice::Context*);
    
public:

    ::std::string to(const ::std::string& id)
    {
        return to(id, 0);
    }
    ::std::string to(const ::std::string& id, const ::Ice::Context& __ctx)
    {
        return to(id, &__ctx);
    }
    
private:

    ::std::string to(const ::std::string&, const ::Ice::Context*);
    
public:

    void setTo(const ::std::string& id, const ::std::string& to)
    {
        setTo(id, to, 0);
    }
    void setTo(const ::std::string& id, const ::std::string& to, const ::Ice::Context& __ctx)
    {
        setTo(id, to, &__ctx);
    }
    
private:

    void setTo(const ::std::string&, const ::std::string&, const ::Ice::Context*);
    
public:

    ::Ice::Double fromPort(const ::std::string& id)
    {
        return fromPort(id, 0);
    }
    ::Ice::Double fromPort(const ::std::string& id, const ::Ice::Context& __ctx)
    {
        return fromPort(id, &__ctx);
    }
    
private:

    ::Ice::Double fromPort(const ::std::string&, const ::Ice::Context*);
    
public:

    void setFromPort(const ::std::string& id, ::Ice::Double fromPort)
    {
        setFromPort(id, fromPort, 0);
    }
    void setFromPort(const ::std::string& id, ::Ice::Double fromPort, const ::Ice::Context& __ctx)
    {
        setFromPort(id, fromPort, &__ctx);
    }
    
private:

    void setFromPort(const ::std::string&, ::Ice::Double, const ::Ice::Context*);
    
public:

    ::Ice::Double toPort(const ::std::string& id)
    {
        return toPort(id, 0);
    }
    ::Ice::Double toPort(const ::std::string& id, const ::Ice::Context& __ctx)
    {
        return toPort(id, &__ctx);
    }
    
private:

    ::Ice::Double toPort(const ::std::string&, const ::Ice::Context*);
    
public:

    void setToPort(const ::std::string& id, ::Ice::Double toPort)
    {
        setToPort(id, toPort, 0);
    }
    void setToPort(const ::std::string& id, ::Ice::Double toPort, const ::Ice::Context& __ctx)
    {
        setToPort(id, toPort, &__ctx);
    }
    
private:

    void setToPort(const ::std::string&, ::Ice::Double, const ::Ice::Context*);
    
public:

    ::std::string otherEntityFromLink(const ::std::string& linkId, const ::std::string& firstNode)
    {
        return otherEntityFromLink(linkId, firstNode, 0);
    }
    ::std::string otherEntityFromLink(const ::std::string& linkId, const ::std::string& firstNode, const ::Ice::Context& __ctx)
    {
        return otherEntityFromLink(linkId, firstNode, &__ctx);
    }
    
private:

    ::std::string otherEntityFromLink(const ::std::string&, const ::std::string&, const ::Ice::Context*);
    
public:

    void exterminate()
    {
        exterminate(0);
    }
    void exterminate(const ::Ice::Context& __ctx)
    {
        exterminate(&__ctx);
    }
    
private:

    void exterminate(const ::Ice::Context*);
    
public:

    void save()
    {
        save(0);
    }
    void save(const ::Ice::Context& __ctx)
    {
        save(&__ctx);
    }
    
private:

    void save(const ::Ice::Context*);
    
public:
    
    ::IceInternal::ProxyHandle<RepoApi> ice_context(const ::Ice::Context& __context) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<RepoApi*>(_Base::ice_context(__context).get());
    #else
        return dynamic_cast<RepoApi*>(::IceProxy::Ice::Object::ice_context(__context).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<RepoApi> ice_adapterId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<RepoApi*>(_Base::ice_adapterId(__id).get());
    #else
        return dynamic_cast<RepoApi*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<RepoApi> ice_endpoints(const ::Ice::EndpointSeq& __endpoints) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<RepoApi*>(_Base::ice_endpoints(__endpoints).get());
    #else
        return dynamic_cast<RepoApi*>(::IceProxy::Ice::Object::ice_endpoints(__endpoints).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<RepoApi> ice_locatorCacheTimeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<RepoApi*>(_Base::ice_locatorCacheTimeout(__timeout).get());
    #else
        return dynamic_cast<RepoApi*>(::IceProxy::Ice::Object::ice_locatorCacheTimeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<RepoApi> ice_connectionCached(bool __cached) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<RepoApi*>(_Base::ice_connectionCached(__cached).get());
    #else
        return dynamic_cast<RepoApi*>(::IceProxy::Ice::Object::ice_connectionCached(__cached).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<RepoApi> ice_endpointSelection(::Ice::EndpointSelectionType __est) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<RepoApi*>(_Base::ice_endpointSelection(__est).get());
    #else
        return dynamic_cast<RepoApi*>(::IceProxy::Ice::Object::ice_endpointSelection(__est).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<RepoApi> ice_secure(bool __secure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<RepoApi*>(_Base::ice_secure(__secure).get());
    #else
        return dynamic_cast<RepoApi*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<RepoApi> ice_preferSecure(bool __preferSecure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<RepoApi*>(_Base::ice_preferSecure(__preferSecure).get());
    #else
        return dynamic_cast<RepoApi*>(::IceProxy::Ice::Object::ice_preferSecure(__preferSecure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<RepoApi> ice_router(const ::Ice::RouterPrx& __router) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<RepoApi*>(_Base::ice_router(__router).get());
    #else
        return dynamic_cast<RepoApi*>(::IceProxy::Ice::Object::ice_router(__router).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<RepoApi> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<RepoApi*>(_Base::ice_locator(__locator).get());
    #else
        return dynamic_cast<RepoApi*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<RepoApi> ice_collocationOptimized(bool __co) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<RepoApi*>(_Base::ice_collocationOptimized(__co).get());
    #else
        return dynamic_cast<RepoApi*>(::IceProxy::Ice::Object::ice_collocationOptimized(__co).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<RepoApi> ice_twoway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<RepoApi*>(_Base::ice_twoway().get());
    #else
        return dynamic_cast<RepoApi*>(::IceProxy::Ice::Object::ice_twoway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<RepoApi> ice_oneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<RepoApi*>(_Base::ice_oneway().get());
    #else
        return dynamic_cast<RepoApi*>(::IceProxy::Ice::Object::ice_oneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<RepoApi> ice_batchOneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<RepoApi*>(_Base::ice_batchOneway().get());
    #else
        return dynamic_cast<RepoApi*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<RepoApi> ice_datagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<RepoApi*>(_Base::ice_datagram().get());
    #else
        return dynamic_cast<RepoApi*>(::IceProxy::Ice::Object::ice_datagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<RepoApi> ice_batchDatagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<RepoApi*>(_Base::ice_batchDatagram().get());
    #else
        return dynamic_cast<RepoApi*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<RepoApi> ice_compress(bool __compress) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<RepoApi*>(_Base::ice_compress(__compress).get());
    #else
        return dynamic_cast<RepoApi*>(::IceProxy::Ice::Object::ice_compress(__compress).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<RepoApi> ice_timeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<RepoApi*>(_Base::ice_timeout(__timeout).get());
    #else
        return dynamic_cast<RepoApi*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<RepoApi> ice_connectionId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<RepoApi*>(_Base::ice_connectionId(__id).get());
    #else
        return dynamic_cast<RepoApi*>(::IceProxy::Ice::Object::ice_connectionId(__id).get());
    #endif
    }
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
    virtual ::IceProxy::Ice::Object* __newInstance() const;
};

}

}

namespace IceDelegate
{

namespace RepoIce
{

class RepoApi : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual ::std::string name(const ::std::string&, const ::Ice::Context*) = 0;

    virtual void setName(const ::std::string&, const ::std::string&, const ::Ice::Context*) = 0;

    virtual ::RepoIce::IdList children(const ::std::string&, const ::Ice::Context*) = 0;

    virtual void addChild(const ::std::string&, const ::std::string&, const ::Ice::Context*) = 0;

    virtual void removeChild(const ::std::string&, const ::std::string&, const ::Ice::Context*) = 0;

    virtual void removeElement(const ::std::string&, const ::Ice::Context*) = 0;

    virtual ::RepoIce::IdList parents(const ::std::string&, const ::Ice::Context*) = 0;

    virtual void addParent(const ::std::string&, const ::std::string&, const ::Ice::Context*) = 0;

    virtual void removeParent(const ::std::string&, const ::std::string&, const ::Ice::Context*) = 0;

    virtual ::RepoIce::IdList outcomingLinks(const ::std::string&, const ::Ice::Context*) = 0;

    virtual ::RepoIce::IdList incomingLinks(const ::std::string&, const ::Ice::Context*) = 0;

    virtual ::RepoIce::IdList links(const ::std::string&, const ::Ice::Context*) = 0;

    virtual ::std::string typeName(const ::std::string&, const ::Ice::Context*) = 0;

    virtual ::std::string property(const ::std::string&, const ::std::string&, const ::Ice::Context*) = 0;

    virtual ::std::string stringProperty(const ::std::string&, const ::std::string&, const ::Ice::Context*) = 0;

    virtual void setProperty(const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Context*) = 0;

    virtual void removeProperty(const ::std::string&, const ::std::string&, const ::Ice::Context*) = 0;

    virtual bool hasProperty(const ::std::string&, const ::std::string&, const ::Ice::Context*) = 0;

    virtual ::std::string from(const ::std::string&, const ::Ice::Context*) = 0;

    virtual void setFrom(const ::std::string&, const ::std::string&, const ::Ice::Context*) = 0;

    virtual ::std::string to(const ::std::string&, const ::Ice::Context*) = 0;

    virtual void setTo(const ::std::string&, const ::std::string&, const ::Ice::Context*) = 0;

    virtual ::Ice::Double fromPort(const ::std::string&, const ::Ice::Context*) = 0;

    virtual void setFromPort(const ::std::string&, ::Ice::Double, const ::Ice::Context*) = 0;

    virtual ::Ice::Double toPort(const ::std::string&, const ::Ice::Context*) = 0;

    virtual void setToPort(const ::std::string&, ::Ice::Double, const ::Ice::Context*) = 0;

    virtual ::std::string otherEntityFromLink(const ::std::string&, const ::std::string&, const ::Ice::Context*) = 0;

    virtual void exterminate(const ::Ice::Context*) = 0;

    virtual void save(const ::Ice::Context*) = 0;
};

}

}

namespace IceDelegateM
{

namespace RepoIce
{

class RepoApi : virtual public ::IceDelegate::RepoIce::RepoApi,
                virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual ::std::string name(const ::std::string&, const ::Ice::Context*);

    virtual void setName(const ::std::string&, const ::std::string&, const ::Ice::Context*);

    virtual ::RepoIce::IdList children(const ::std::string&, const ::Ice::Context*);

    virtual void addChild(const ::std::string&, const ::std::string&, const ::Ice::Context*);

    virtual void removeChild(const ::std::string&, const ::std::string&, const ::Ice::Context*);

    virtual void removeElement(const ::std::string&, const ::Ice::Context*);

    virtual ::RepoIce::IdList parents(const ::std::string&, const ::Ice::Context*);

    virtual void addParent(const ::std::string&, const ::std::string&, const ::Ice::Context*);

    virtual void removeParent(const ::std::string&, const ::std::string&, const ::Ice::Context*);

    virtual ::RepoIce::IdList outcomingLinks(const ::std::string&, const ::Ice::Context*);

    virtual ::RepoIce::IdList incomingLinks(const ::std::string&, const ::Ice::Context*);

    virtual ::RepoIce::IdList links(const ::std::string&, const ::Ice::Context*);

    virtual ::std::string typeName(const ::std::string&, const ::Ice::Context*);

    virtual ::std::string property(const ::std::string&, const ::std::string&, const ::Ice::Context*);

    virtual ::std::string stringProperty(const ::std::string&, const ::std::string&, const ::Ice::Context*);

    virtual void setProperty(const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Context*);

    virtual void removeProperty(const ::std::string&, const ::std::string&, const ::Ice::Context*);

    virtual bool hasProperty(const ::std::string&, const ::std::string&, const ::Ice::Context*);

    virtual ::std::string from(const ::std::string&, const ::Ice::Context*);

    virtual void setFrom(const ::std::string&, const ::std::string&, const ::Ice::Context*);

    virtual ::std::string to(const ::std::string&, const ::Ice::Context*);

    virtual void setTo(const ::std::string&, const ::std::string&, const ::Ice::Context*);

    virtual ::Ice::Double fromPort(const ::std::string&, const ::Ice::Context*);

    virtual void setFromPort(const ::std::string&, ::Ice::Double, const ::Ice::Context*);

    virtual ::Ice::Double toPort(const ::std::string&, const ::Ice::Context*);

    virtual void setToPort(const ::std::string&, ::Ice::Double, const ::Ice::Context*);

    virtual ::std::string otherEntityFromLink(const ::std::string&, const ::std::string&, const ::Ice::Context*);

    virtual void exterminate(const ::Ice::Context*);

    virtual void save(const ::Ice::Context*);
};

}

}

namespace IceDelegateD
{

namespace RepoIce
{

class RepoApi : virtual public ::IceDelegate::RepoIce::RepoApi,
                virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual ::std::string name(const ::std::string&, const ::Ice::Context*);

    virtual void setName(const ::std::string&, const ::std::string&, const ::Ice::Context*);

    virtual ::RepoIce::IdList children(const ::std::string&, const ::Ice::Context*);

    virtual void addChild(const ::std::string&, const ::std::string&, const ::Ice::Context*);

    virtual void removeChild(const ::std::string&, const ::std::string&, const ::Ice::Context*);

    virtual void removeElement(const ::std::string&, const ::Ice::Context*);

    virtual ::RepoIce::IdList parents(const ::std::string&, const ::Ice::Context*);

    virtual void addParent(const ::std::string&, const ::std::string&, const ::Ice::Context*);

    virtual void removeParent(const ::std::string&, const ::std::string&, const ::Ice::Context*);

    virtual ::RepoIce::IdList outcomingLinks(const ::std::string&, const ::Ice::Context*);

    virtual ::RepoIce::IdList incomingLinks(const ::std::string&, const ::Ice::Context*);

    virtual ::RepoIce::IdList links(const ::std::string&, const ::Ice::Context*);

    virtual ::std::string typeName(const ::std::string&, const ::Ice::Context*);

    virtual ::std::string property(const ::std::string&, const ::std::string&, const ::Ice::Context*);

    virtual ::std::string stringProperty(const ::std::string&, const ::std::string&, const ::Ice::Context*);

    virtual void setProperty(const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Context*);

    virtual void removeProperty(const ::std::string&, const ::std::string&, const ::Ice::Context*);

    virtual bool hasProperty(const ::std::string&, const ::std::string&, const ::Ice::Context*);

    virtual ::std::string from(const ::std::string&, const ::Ice::Context*);

    virtual void setFrom(const ::std::string&, const ::std::string&, const ::Ice::Context*);

    virtual ::std::string to(const ::std::string&, const ::Ice::Context*);

    virtual void setTo(const ::std::string&, const ::std::string&, const ::Ice::Context*);

    virtual ::Ice::Double fromPort(const ::std::string&, const ::Ice::Context*);

    virtual void setFromPort(const ::std::string&, ::Ice::Double, const ::Ice::Context*);

    virtual ::Ice::Double toPort(const ::std::string&, const ::Ice::Context*);

    virtual void setToPort(const ::std::string&, ::Ice::Double, const ::Ice::Context*);

    virtual ::std::string otherEntityFromLink(const ::std::string&, const ::std::string&, const ::Ice::Context*);

    virtual void exterminate(const ::Ice::Context*);

    virtual void save(const ::Ice::Context*);
};

}

}

namespace RepoIce
{

class RepoApi : virtual public ::Ice::Object
{
public:

    typedef RepoApiPrx ProxyType;
    typedef RepoApiPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual ::std::string name(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___name(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void setName(const ::std::string&, const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___setName(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::RepoIce::IdList children(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___children(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void addChild(const ::std::string&, const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___addChild(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void removeChild(const ::std::string&, const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___removeChild(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void removeElement(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___removeElement(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::RepoIce::IdList parents(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___parents(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void addParent(const ::std::string&, const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___addParent(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void removeParent(const ::std::string&, const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___removeParent(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::RepoIce::IdList outcomingLinks(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___outcomingLinks(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::RepoIce::IdList incomingLinks(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___incomingLinks(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::RepoIce::IdList links(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___links(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::std::string typeName(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___typeName(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::std::string property(const ::std::string&, const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___property(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::std::string stringProperty(const ::std::string&, const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___stringProperty(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void setProperty(const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___setProperty(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void removeProperty(const ::std::string&, const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___removeProperty(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual bool hasProperty(const ::std::string&, const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___hasProperty(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::std::string from(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___from(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void setFrom(const ::std::string&, const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___setFrom(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::std::string to(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___to(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void setTo(const ::std::string&, const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___setTo(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::Ice::Double fromPort(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___fromPort(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void setFromPort(const ::std::string&, ::Ice::Double, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___setFromPort(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::Ice::Double toPort(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___toPort(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void setToPort(const ::std::string&, ::Ice::Double, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___setToPort(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::std::string otherEntityFromLink(const ::std::string&, const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___otherEntityFromLink(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void exterminate(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___exterminate(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void save(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___save(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::Ice::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

}

#endif
