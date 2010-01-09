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

#include <repoclientice.h>
#include <Ice/LocalException.h>
#include <Ice/ObjectFactory.h>
#include <Ice/BasicStream.h>
#include <IceUtil/Iterator.h>
#include <IceUtil/ScopedArray.h>

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

static const ::std::string __RepoIce__RepoApi__name_name = "name";

static const ::std::string __RepoIce__RepoApi__setName_name = "setName";

static const ::std::string __RepoIce__RepoApi__children_name = "children";

static const ::std::string __RepoIce__RepoApi__addChild_name = "addChild";

static const ::std::string __RepoIce__RepoApi__removeChild_name = "removeChild";

static const ::std::string __RepoIce__RepoApi__removeElement_name = "removeElement";

static const ::std::string __RepoIce__RepoApi__parents_name = "parents";

static const ::std::string __RepoIce__RepoApi__addParent_name = "addParent";

static const ::std::string __RepoIce__RepoApi__removeParent_name = "removeParent";

static const ::std::string __RepoIce__RepoApi__outcomingLinks_name = "outcomingLinks";

static const ::std::string __RepoIce__RepoApi__incomingLinks_name = "incomingLinks";

static const ::std::string __RepoIce__RepoApi__links_name = "links";

static const ::std::string __RepoIce__RepoApi__typeName_name = "typeName";

static const ::std::string __RepoIce__RepoApi__property_name = "property";

static const ::std::string __RepoIce__RepoApi__stringProperty_name = "stringProperty";

static const ::std::string __RepoIce__RepoApi__setProperty_name = "setProperty";

static const ::std::string __RepoIce__RepoApi__removeProperty_name = "removeProperty";

static const ::std::string __RepoIce__RepoApi__hasProperty_name = "hasProperty";

static const ::std::string __RepoIce__RepoApi__from_name = "from";

static const ::std::string __RepoIce__RepoApi__setFrom_name = "setFrom";

static const ::std::string __RepoIce__RepoApi__to_name = "to";

static const ::std::string __RepoIce__RepoApi__setTo_name = "setTo";

static const ::std::string __RepoIce__RepoApi__fromPort_name = "fromPort";

static const ::std::string __RepoIce__RepoApi__setFromPort_name = "setFromPort";

static const ::std::string __RepoIce__RepoApi__toPort_name = "toPort";

static const ::std::string __RepoIce__RepoApi__setToPort_name = "setToPort";

static const ::std::string __RepoIce__RepoApi__otherEntityFromLink_name = "otherEntityFromLink";

static const ::std::string __RepoIce__RepoApi__exterminate_name = "exterminate";

static const ::std::string __RepoIce__RepoApi__save_name = "save";

::Ice::Object* IceInternal::upCast(::RepoIce::RepoApi* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::RepoIce::RepoApi* p) { return p; }

void
RepoIce::__read(::IceInternal::BasicStream* __is, ::RepoIce::RepoApiPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::RepoIce::RepoApi;
        v->__copyFrom(proxy);
    }
}

::std::string
IceProxy::RepoIce::RepoApi::name(const ::std::string& id, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__RepoIce__RepoApi__name_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::RepoIce::RepoApi* __del = dynamic_cast< ::IceDelegate::RepoIce::RepoApi*>(__delBase.get());
            return __del->name(id, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapperRelaxed(__delBase, __ex, 0, __cnt);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

void
IceProxy::RepoIce::RepoApi::setName(const ::std::string& id, const ::std::string& name, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __delBase = __getDelegate(false);
            ::IceDelegate::RepoIce::RepoApi* __del = dynamic_cast< ::IceDelegate::RepoIce::RepoApi*>(__delBase.get());
            __del->setName(id, name, __ctx);
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

::RepoIce::IdList
IceProxy::RepoIce::RepoApi::children(const ::std::string& id, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__RepoIce__RepoApi__children_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::RepoIce::RepoApi* __del = dynamic_cast< ::IceDelegate::RepoIce::RepoApi*>(__delBase.get());
            return __del->children(id, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

void
IceProxy::RepoIce::RepoApi::addChild(const ::std::string& id, const ::std::string& child, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __delBase = __getDelegate(false);
            ::IceDelegate::RepoIce::RepoApi* __del = dynamic_cast< ::IceDelegate::RepoIce::RepoApi*>(__delBase.get());
            __del->addChild(id, child, __ctx);
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

void
IceProxy::RepoIce::RepoApi::removeChild(const ::std::string& id, const ::std::string& child, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __delBase = __getDelegate(false);
            ::IceDelegate::RepoIce::RepoApi* __del = dynamic_cast< ::IceDelegate::RepoIce::RepoApi*>(__delBase.get());
            __del->removeChild(id, child, __ctx);
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

void
IceProxy::RepoIce::RepoApi::removeElement(const ::std::string& id, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __delBase = __getDelegate(false);
            ::IceDelegate::RepoIce::RepoApi* __del = dynamic_cast< ::IceDelegate::RepoIce::RepoApi*>(__delBase.get());
            __del->removeElement(id, __ctx);
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

::RepoIce::IdList
IceProxy::RepoIce::RepoApi::parents(const ::std::string& id, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__RepoIce__RepoApi__parents_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::RepoIce::RepoApi* __del = dynamic_cast< ::IceDelegate::RepoIce::RepoApi*>(__delBase.get());
            return __del->parents(id, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapperRelaxed(__delBase, __ex, 0, __cnt);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

void
IceProxy::RepoIce::RepoApi::addParent(const ::std::string& id, const ::std::string& parent, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __delBase = __getDelegate(false);
            ::IceDelegate::RepoIce::RepoApi* __del = dynamic_cast< ::IceDelegate::RepoIce::RepoApi*>(__delBase.get());
            __del->addParent(id, parent, __ctx);
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

void
IceProxy::RepoIce::RepoApi::removeParent(const ::std::string& id, const ::std::string& parent, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __delBase = __getDelegate(false);
            ::IceDelegate::RepoIce::RepoApi* __del = dynamic_cast< ::IceDelegate::RepoIce::RepoApi*>(__delBase.get());
            __del->removeParent(id, parent, __ctx);
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

::RepoIce::IdList
IceProxy::RepoIce::RepoApi::outcomingLinks(const ::std::string& id, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__RepoIce__RepoApi__outcomingLinks_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::RepoIce::RepoApi* __del = dynamic_cast< ::IceDelegate::RepoIce::RepoApi*>(__delBase.get());
            return __del->outcomingLinks(id, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapperRelaxed(__delBase, __ex, 0, __cnt);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

::RepoIce::IdList
IceProxy::RepoIce::RepoApi::incomingLinks(const ::std::string& id, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__RepoIce__RepoApi__incomingLinks_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::RepoIce::RepoApi* __del = dynamic_cast< ::IceDelegate::RepoIce::RepoApi*>(__delBase.get());
            return __del->incomingLinks(id, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapperRelaxed(__delBase, __ex, 0, __cnt);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

::RepoIce::IdList
IceProxy::RepoIce::RepoApi::links(const ::std::string& id, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__RepoIce__RepoApi__links_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::RepoIce::RepoApi* __del = dynamic_cast< ::IceDelegate::RepoIce::RepoApi*>(__delBase.get());
            return __del->links(id, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapperRelaxed(__delBase, __ex, 0, __cnt);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

::std::string
IceProxy::RepoIce::RepoApi::typeName(const ::std::string& id, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__RepoIce__RepoApi__typeName_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::RepoIce::RepoApi* __del = dynamic_cast< ::IceDelegate::RepoIce::RepoApi*>(__delBase.get());
            return __del->typeName(id, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapperRelaxed(__delBase, __ex, 0, __cnt);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

::std::string
IceProxy::RepoIce::RepoApi::property(const ::std::string& id, const ::std::string& propertyName, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__RepoIce__RepoApi__property_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::RepoIce::RepoApi* __del = dynamic_cast< ::IceDelegate::RepoIce::RepoApi*>(__delBase.get());
            return __del->property(id, propertyName, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapperRelaxed(__delBase, __ex, 0, __cnt);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

::std::string
IceProxy::RepoIce::RepoApi::stringProperty(const ::std::string& id, const ::std::string& propertyName, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__RepoIce__RepoApi__stringProperty_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::RepoIce::RepoApi* __del = dynamic_cast< ::IceDelegate::RepoIce::RepoApi*>(__delBase.get());
            return __del->stringProperty(id, propertyName, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapperRelaxed(__delBase, __ex, 0, __cnt);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

void
IceProxy::RepoIce::RepoApi::setProperty(const ::std::string& id, const ::std::string& propertyName, const ::std::string& value, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __delBase = __getDelegate(false);
            ::IceDelegate::RepoIce::RepoApi* __del = dynamic_cast< ::IceDelegate::RepoIce::RepoApi*>(__delBase.get());
            __del->setProperty(id, propertyName, value, __ctx);
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

void
IceProxy::RepoIce::RepoApi::removeProperty(const ::std::string& id, const ::std::string& propertyName, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __delBase = __getDelegate(false);
            ::IceDelegate::RepoIce::RepoApi* __del = dynamic_cast< ::IceDelegate::RepoIce::RepoApi*>(__delBase.get());
            __del->removeProperty(id, propertyName, __ctx);
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

bool
IceProxy::RepoIce::RepoApi::hasProperty(const ::std::string& id, const ::std::string& propertyName, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__RepoIce__RepoApi__hasProperty_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::RepoIce::RepoApi* __del = dynamic_cast< ::IceDelegate::RepoIce::RepoApi*>(__delBase.get());
            return __del->hasProperty(id, propertyName, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapperRelaxed(__delBase, __ex, 0, __cnt);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

::std::string
IceProxy::RepoIce::RepoApi::from(const ::std::string& id, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__RepoIce__RepoApi__from_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::RepoIce::RepoApi* __del = dynamic_cast< ::IceDelegate::RepoIce::RepoApi*>(__delBase.get());
            return __del->from(id, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapperRelaxed(__delBase, __ex, 0, __cnt);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

void
IceProxy::RepoIce::RepoApi::setFrom(const ::std::string& id, const ::std::string& from, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __delBase = __getDelegate(false);
            ::IceDelegate::RepoIce::RepoApi* __del = dynamic_cast< ::IceDelegate::RepoIce::RepoApi*>(__delBase.get());
            __del->setFrom(id, from, __ctx);
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

::std::string
IceProxy::RepoIce::RepoApi::to(const ::std::string& id, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__RepoIce__RepoApi__to_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::RepoIce::RepoApi* __del = dynamic_cast< ::IceDelegate::RepoIce::RepoApi*>(__delBase.get());
            return __del->to(id, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapperRelaxed(__delBase, __ex, 0, __cnt);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

void
IceProxy::RepoIce::RepoApi::setTo(const ::std::string& id, const ::std::string& to, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __delBase = __getDelegate(false);
            ::IceDelegate::RepoIce::RepoApi* __del = dynamic_cast< ::IceDelegate::RepoIce::RepoApi*>(__delBase.get());
            __del->setTo(id, to, __ctx);
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

::Ice::Double
IceProxy::RepoIce::RepoApi::fromPort(const ::std::string& id, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__RepoIce__RepoApi__fromPort_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::RepoIce::RepoApi* __del = dynamic_cast< ::IceDelegate::RepoIce::RepoApi*>(__delBase.get());
            return __del->fromPort(id, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapperRelaxed(__delBase, __ex, 0, __cnt);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

void
IceProxy::RepoIce::RepoApi::setFromPort(const ::std::string& id, ::Ice::Double fromPort, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __delBase = __getDelegate(false);
            ::IceDelegate::RepoIce::RepoApi* __del = dynamic_cast< ::IceDelegate::RepoIce::RepoApi*>(__delBase.get());
            __del->setFromPort(id, fromPort, __ctx);
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

::Ice::Double
IceProxy::RepoIce::RepoApi::toPort(const ::std::string& id, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__RepoIce__RepoApi__toPort_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::RepoIce::RepoApi* __del = dynamic_cast< ::IceDelegate::RepoIce::RepoApi*>(__delBase.get());
            return __del->toPort(id, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapperRelaxed(__delBase, __ex, 0, __cnt);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

void
IceProxy::RepoIce::RepoApi::setToPort(const ::std::string& id, ::Ice::Double toPort, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __delBase = __getDelegate(false);
            ::IceDelegate::RepoIce::RepoApi* __del = dynamic_cast< ::IceDelegate::RepoIce::RepoApi*>(__delBase.get());
            __del->setToPort(id, toPort, __ctx);
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

::std::string
IceProxy::RepoIce::RepoApi::otherEntityFromLink(const ::std::string& linkId, const ::std::string& firstNode, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__RepoIce__RepoApi__otherEntityFromLink_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::RepoIce::RepoApi* __del = dynamic_cast< ::IceDelegate::RepoIce::RepoApi*>(__delBase.get());
            return __del->otherEntityFromLink(linkId, firstNode, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapperRelaxed(__delBase, __ex, 0, __cnt);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

void
IceProxy::RepoIce::RepoApi::exterminate(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __delBase = __getDelegate(false);
            ::IceDelegate::RepoIce::RepoApi* __del = dynamic_cast< ::IceDelegate::RepoIce::RepoApi*>(__delBase.get());
            __del->exterminate(__ctx);
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

void
IceProxy::RepoIce::RepoApi::save(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __delBase = __getDelegate(false);
            ::IceDelegate::RepoIce::RepoApi* __del = dynamic_cast< ::IceDelegate::RepoIce::RepoApi*>(__delBase.get());
            __del->save(__ctx);
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

const ::std::string&
IceProxy::RepoIce::RepoApi::ice_staticId()
{
    return ::RepoIce::RepoApi::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::RepoIce::RepoApi::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::RepoIce::RepoApi);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::RepoIce::RepoApi::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::RepoIce::RepoApi);
}

::IceProxy::Ice::Object*
IceProxy::RepoIce::RepoApi::__newInstance() const
{
    return new RepoApi;
}

::std::string
IceDelegateM::RepoIce::RepoApi::name(const ::std::string& id, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __RepoIce__RepoApi__name_name, ::Ice::Idempotent, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(id);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::std::string __ret;
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->read(__ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateM::RepoIce::RepoApi::setName(const ::std::string& id, const ::std::string& name, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __RepoIce__RepoApi__setName_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(id);
        __os->write(name);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    if(!__og.is()->b.empty())
    {
        try
        {
            if(!__ok)
            {
                try
                {
                    __og.throwUserException();
                }
                catch(const ::Ice::UserException& __ex)
                {
                    ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                    throw __uue;
                }
            }
            __og.is()->skipEmptyEncaps();
        }
        catch(const ::Ice::LocalException& __ex)
        {
            throw ::IceInternal::LocalExceptionWrapper(__ex, false);
        }
    }
}

::RepoIce::IdList
IceDelegateM::RepoIce::RepoApi::children(const ::std::string& id, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __RepoIce__RepoApi__children_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(id);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::RepoIce::IdList __ret;
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->read(__ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateM::RepoIce::RepoApi::addChild(const ::std::string& id, const ::std::string& child, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __RepoIce__RepoApi__addChild_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(id);
        __os->write(child);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    if(!__og.is()->b.empty())
    {
        try
        {
            if(!__ok)
            {
                try
                {
                    __og.throwUserException();
                }
                catch(const ::Ice::UserException& __ex)
                {
                    ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                    throw __uue;
                }
            }
            __og.is()->skipEmptyEncaps();
        }
        catch(const ::Ice::LocalException& __ex)
        {
            throw ::IceInternal::LocalExceptionWrapper(__ex, false);
        }
    }
}

void
IceDelegateM::RepoIce::RepoApi::removeChild(const ::std::string& id, const ::std::string& child, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __RepoIce__RepoApi__removeChild_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(id);
        __os->write(child);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    if(!__og.is()->b.empty())
    {
        try
        {
            if(!__ok)
            {
                try
                {
                    __og.throwUserException();
                }
                catch(const ::Ice::UserException& __ex)
                {
                    ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                    throw __uue;
                }
            }
            __og.is()->skipEmptyEncaps();
        }
        catch(const ::Ice::LocalException& __ex)
        {
            throw ::IceInternal::LocalExceptionWrapper(__ex, false);
        }
    }
}

void
IceDelegateM::RepoIce::RepoApi::removeElement(const ::std::string& id, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __RepoIce__RepoApi__removeElement_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(id);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    if(!__og.is()->b.empty())
    {
        try
        {
            if(!__ok)
            {
                try
                {
                    __og.throwUserException();
                }
                catch(const ::Ice::UserException& __ex)
                {
                    ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                    throw __uue;
                }
            }
            __og.is()->skipEmptyEncaps();
        }
        catch(const ::Ice::LocalException& __ex)
        {
            throw ::IceInternal::LocalExceptionWrapper(__ex, false);
        }
    }
}

::RepoIce::IdList
IceDelegateM::RepoIce::RepoApi::parents(const ::std::string& id, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __RepoIce__RepoApi__parents_name, ::Ice::Idempotent, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(id);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::RepoIce::IdList __ret;
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->read(__ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateM::RepoIce::RepoApi::addParent(const ::std::string& id, const ::std::string& parent, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __RepoIce__RepoApi__addParent_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(id);
        __os->write(parent);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    if(!__og.is()->b.empty())
    {
        try
        {
            if(!__ok)
            {
                try
                {
                    __og.throwUserException();
                }
                catch(const ::Ice::UserException& __ex)
                {
                    ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                    throw __uue;
                }
            }
            __og.is()->skipEmptyEncaps();
        }
        catch(const ::Ice::LocalException& __ex)
        {
            throw ::IceInternal::LocalExceptionWrapper(__ex, false);
        }
    }
}

void
IceDelegateM::RepoIce::RepoApi::removeParent(const ::std::string& id, const ::std::string& parent, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __RepoIce__RepoApi__removeParent_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(id);
        __os->write(parent);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    if(!__og.is()->b.empty())
    {
        try
        {
            if(!__ok)
            {
                try
                {
                    __og.throwUserException();
                }
                catch(const ::Ice::UserException& __ex)
                {
                    ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                    throw __uue;
                }
            }
            __og.is()->skipEmptyEncaps();
        }
        catch(const ::Ice::LocalException& __ex)
        {
            throw ::IceInternal::LocalExceptionWrapper(__ex, false);
        }
    }
}

::RepoIce::IdList
IceDelegateM::RepoIce::RepoApi::outcomingLinks(const ::std::string& id, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __RepoIce__RepoApi__outcomingLinks_name, ::Ice::Idempotent, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(id);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::RepoIce::IdList __ret;
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->read(__ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::RepoIce::IdList
IceDelegateM::RepoIce::RepoApi::incomingLinks(const ::std::string& id, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __RepoIce__RepoApi__incomingLinks_name, ::Ice::Idempotent, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(id);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::RepoIce::IdList __ret;
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->read(__ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::RepoIce::IdList
IceDelegateM::RepoIce::RepoApi::links(const ::std::string& id, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __RepoIce__RepoApi__links_name, ::Ice::Idempotent, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(id);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::RepoIce::IdList __ret;
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->read(__ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::std::string
IceDelegateM::RepoIce::RepoApi::typeName(const ::std::string& id, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __RepoIce__RepoApi__typeName_name, ::Ice::Idempotent, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(id);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::std::string __ret;
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->read(__ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::std::string
IceDelegateM::RepoIce::RepoApi::property(const ::std::string& id, const ::std::string& propertyName, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __RepoIce__RepoApi__property_name, ::Ice::Idempotent, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(id);
        __os->write(propertyName);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::std::string __ret;
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->read(__ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::std::string
IceDelegateM::RepoIce::RepoApi::stringProperty(const ::std::string& id, const ::std::string& propertyName, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __RepoIce__RepoApi__stringProperty_name, ::Ice::Idempotent, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(id);
        __os->write(propertyName);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::std::string __ret;
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->read(__ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateM::RepoIce::RepoApi::setProperty(const ::std::string& id, const ::std::string& propertyName, const ::std::string& value, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __RepoIce__RepoApi__setProperty_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(id);
        __os->write(propertyName);
        __os->write(value);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    if(!__og.is()->b.empty())
    {
        try
        {
            if(!__ok)
            {
                try
                {
                    __og.throwUserException();
                }
                catch(const ::Ice::UserException& __ex)
                {
                    ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                    throw __uue;
                }
            }
            __og.is()->skipEmptyEncaps();
        }
        catch(const ::Ice::LocalException& __ex)
        {
            throw ::IceInternal::LocalExceptionWrapper(__ex, false);
        }
    }
}

void
IceDelegateM::RepoIce::RepoApi::removeProperty(const ::std::string& id, const ::std::string& propertyName, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __RepoIce__RepoApi__removeProperty_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(id);
        __os->write(propertyName);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    if(!__og.is()->b.empty())
    {
        try
        {
            if(!__ok)
            {
                try
                {
                    __og.throwUserException();
                }
                catch(const ::Ice::UserException& __ex)
                {
                    ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                    throw __uue;
                }
            }
            __og.is()->skipEmptyEncaps();
        }
        catch(const ::Ice::LocalException& __ex)
        {
            throw ::IceInternal::LocalExceptionWrapper(__ex, false);
        }
    }
}

bool
IceDelegateM::RepoIce::RepoApi::hasProperty(const ::std::string& id, const ::std::string& propertyName, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __RepoIce__RepoApi__hasProperty_name, ::Ice::Idempotent, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(id);
        __os->write(propertyName);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        bool __ret;
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->read(__ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::std::string
IceDelegateM::RepoIce::RepoApi::from(const ::std::string& id, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __RepoIce__RepoApi__from_name, ::Ice::Idempotent, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(id);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::std::string __ret;
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->read(__ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateM::RepoIce::RepoApi::setFrom(const ::std::string& id, const ::std::string& from, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __RepoIce__RepoApi__setFrom_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(id);
        __os->write(from);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    if(!__og.is()->b.empty())
    {
        try
        {
            if(!__ok)
            {
                try
                {
                    __og.throwUserException();
                }
                catch(const ::Ice::UserException& __ex)
                {
                    ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                    throw __uue;
                }
            }
            __og.is()->skipEmptyEncaps();
        }
        catch(const ::Ice::LocalException& __ex)
        {
            throw ::IceInternal::LocalExceptionWrapper(__ex, false);
        }
    }
}

::std::string
IceDelegateM::RepoIce::RepoApi::to(const ::std::string& id, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __RepoIce__RepoApi__to_name, ::Ice::Idempotent, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(id);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::std::string __ret;
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->read(__ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateM::RepoIce::RepoApi::setTo(const ::std::string& id, const ::std::string& to, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __RepoIce__RepoApi__setTo_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(id);
        __os->write(to);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    if(!__og.is()->b.empty())
    {
        try
        {
            if(!__ok)
            {
                try
                {
                    __og.throwUserException();
                }
                catch(const ::Ice::UserException& __ex)
                {
                    ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                    throw __uue;
                }
            }
            __og.is()->skipEmptyEncaps();
        }
        catch(const ::Ice::LocalException& __ex)
        {
            throw ::IceInternal::LocalExceptionWrapper(__ex, false);
        }
    }
}

::Ice::Double
IceDelegateM::RepoIce::RepoApi::fromPort(const ::std::string& id, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __RepoIce__RepoApi__fromPort_name, ::Ice::Idempotent, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(id);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::Ice::Double __ret;
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->read(__ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateM::RepoIce::RepoApi::setFromPort(const ::std::string& id, ::Ice::Double fromPort, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __RepoIce__RepoApi__setFromPort_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(id);
        __os->write(fromPort);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    if(!__og.is()->b.empty())
    {
        try
        {
            if(!__ok)
            {
                try
                {
                    __og.throwUserException();
                }
                catch(const ::Ice::UserException& __ex)
                {
                    ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                    throw __uue;
                }
            }
            __og.is()->skipEmptyEncaps();
        }
        catch(const ::Ice::LocalException& __ex)
        {
            throw ::IceInternal::LocalExceptionWrapper(__ex, false);
        }
    }
}

::Ice::Double
IceDelegateM::RepoIce::RepoApi::toPort(const ::std::string& id, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __RepoIce__RepoApi__toPort_name, ::Ice::Idempotent, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(id);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::Ice::Double __ret;
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->read(__ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateM::RepoIce::RepoApi::setToPort(const ::std::string& id, ::Ice::Double toPort, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __RepoIce__RepoApi__setToPort_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(id);
        __os->write(toPort);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    if(!__og.is()->b.empty())
    {
        try
        {
            if(!__ok)
            {
                try
                {
                    __og.throwUserException();
                }
                catch(const ::Ice::UserException& __ex)
                {
                    ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                    throw __uue;
                }
            }
            __og.is()->skipEmptyEncaps();
        }
        catch(const ::Ice::LocalException& __ex)
        {
            throw ::IceInternal::LocalExceptionWrapper(__ex, false);
        }
    }
}

::std::string
IceDelegateM::RepoIce::RepoApi::otherEntityFromLink(const ::std::string& linkId, const ::std::string& firstNode, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __RepoIce__RepoApi__otherEntityFromLink_name, ::Ice::Idempotent, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(linkId);
        __os->write(firstNode);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::std::string __ret;
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->read(__ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateM::RepoIce::RepoApi::exterminate(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __RepoIce__RepoApi__exterminate_name, ::Ice::Normal, __context);
    bool __ok = __og.invoke();
    if(!__og.is()->b.empty())
    {
        try
        {
            if(!__ok)
            {
                try
                {
                    __og.throwUserException();
                }
                catch(const ::Ice::UserException& __ex)
                {
                    ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                    throw __uue;
                }
            }
            __og.is()->skipEmptyEncaps();
        }
        catch(const ::Ice::LocalException& __ex)
        {
            throw ::IceInternal::LocalExceptionWrapper(__ex, false);
        }
    }
}

void
IceDelegateM::RepoIce::RepoApi::save(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __RepoIce__RepoApi__save_name, ::Ice::Normal, __context);
    bool __ok = __og.invoke();
    if(!__og.is()->b.empty())
    {
        try
        {
            if(!__ok)
            {
                try
                {
                    __og.throwUserException();
                }
                catch(const ::Ice::UserException& __ex)
                {
                    ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                    throw __uue;
                }
            }
            __og.is()->skipEmptyEncaps();
        }
        catch(const ::Ice::LocalException& __ex)
        {
            throw ::IceInternal::LocalExceptionWrapper(__ex, false);
        }
    }
}

::std::string
IceDelegateD::RepoIce::RepoApi::name(const ::std::string& id, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::std::string& __result, const ::std::string& id, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_id(id)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::RepoIce::RepoApi* servant = dynamic_cast< ::RepoIce::RepoApi*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->name(_m_id, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::std::string& _result;
        const ::std::string& _m_id;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __RepoIce__RepoApi__name_name, ::Ice::Idempotent, __context);
    ::std::string __result;
    try
    {
        _DirectI __direct(__result, id, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

void
IceDelegateD::RepoIce::RepoApi::setName(const ::std::string& id, const ::std::string& name, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::std::string& id, const ::std::string& name, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _m_id(id),
            _m_name(name)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::RepoIce::RepoApi* servant = dynamic_cast< ::RepoIce::RepoApi*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->setName(_m_id, _m_name, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        const ::std::string& _m_id;
        const ::std::string& _m_name;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __RepoIce__RepoApi__setName_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(id, name, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
}

::RepoIce::IdList
IceDelegateD::RepoIce::RepoApi::children(const ::std::string& id, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::RepoIce::IdList& __result, const ::std::string& id, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_id(id)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::RepoIce::RepoApi* servant = dynamic_cast< ::RepoIce::RepoApi*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->children(_m_id, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::RepoIce::IdList& _result;
        const ::std::string& _m_id;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __RepoIce__RepoApi__children_name, ::Ice::Normal, __context);
    ::RepoIce::IdList __result;
    try
    {
        _DirectI __direct(__result, id, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

void
IceDelegateD::RepoIce::RepoApi::addChild(const ::std::string& id, const ::std::string& child, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::std::string& id, const ::std::string& child, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _m_id(id),
            _m_child(child)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::RepoIce::RepoApi* servant = dynamic_cast< ::RepoIce::RepoApi*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->addChild(_m_id, _m_child, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        const ::std::string& _m_id;
        const ::std::string& _m_child;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __RepoIce__RepoApi__addChild_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(id, child, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
}

void
IceDelegateD::RepoIce::RepoApi::removeChild(const ::std::string& id, const ::std::string& child, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::std::string& id, const ::std::string& child, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _m_id(id),
            _m_child(child)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::RepoIce::RepoApi* servant = dynamic_cast< ::RepoIce::RepoApi*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->removeChild(_m_id, _m_child, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        const ::std::string& _m_id;
        const ::std::string& _m_child;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __RepoIce__RepoApi__removeChild_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(id, child, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
}

void
IceDelegateD::RepoIce::RepoApi::removeElement(const ::std::string& id, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::std::string& id, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _m_id(id)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::RepoIce::RepoApi* servant = dynamic_cast< ::RepoIce::RepoApi*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->removeElement(_m_id, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        const ::std::string& _m_id;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __RepoIce__RepoApi__removeElement_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(id, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
}

::RepoIce::IdList
IceDelegateD::RepoIce::RepoApi::parents(const ::std::string& id, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::RepoIce::IdList& __result, const ::std::string& id, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_id(id)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::RepoIce::RepoApi* servant = dynamic_cast< ::RepoIce::RepoApi*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->parents(_m_id, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::RepoIce::IdList& _result;
        const ::std::string& _m_id;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __RepoIce__RepoApi__parents_name, ::Ice::Idempotent, __context);
    ::RepoIce::IdList __result;
    try
    {
        _DirectI __direct(__result, id, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

void
IceDelegateD::RepoIce::RepoApi::addParent(const ::std::string& id, const ::std::string& parent, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::std::string& id, const ::std::string& parent, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _m_id(id),
            _m_parent(parent)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::RepoIce::RepoApi* servant = dynamic_cast< ::RepoIce::RepoApi*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->addParent(_m_id, _m_parent, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        const ::std::string& _m_id;
        const ::std::string& _m_parent;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __RepoIce__RepoApi__addParent_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(id, parent, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
}

void
IceDelegateD::RepoIce::RepoApi::removeParent(const ::std::string& id, const ::std::string& parent, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::std::string& id, const ::std::string& parent, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _m_id(id),
            _m_parent(parent)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::RepoIce::RepoApi* servant = dynamic_cast< ::RepoIce::RepoApi*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->removeParent(_m_id, _m_parent, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        const ::std::string& _m_id;
        const ::std::string& _m_parent;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __RepoIce__RepoApi__removeParent_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(id, parent, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
}

::RepoIce::IdList
IceDelegateD::RepoIce::RepoApi::outcomingLinks(const ::std::string& id, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::RepoIce::IdList& __result, const ::std::string& id, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_id(id)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::RepoIce::RepoApi* servant = dynamic_cast< ::RepoIce::RepoApi*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->outcomingLinks(_m_id, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::RepoIce::IdList& _result;
        const ::std::string& _m_id;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __RepoIce__RepoApi__outcomingLinks_name, ::Ice::Idempotent, __context);
    ::RepoIce::IdList __result;
    try
    {
        _DirectI __direct(__result, id, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

::RepoIce::IdList
IceDelegateD::RepoIce::RepoApi::incomingLinks(const ::std::string& id, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::RepoIce::IdList& __result, const ::std::string& id, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_id(id)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::RepoIce::RepoApi* servant = dynamic_cast< ::RepoIce::RepoApi*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->incomingLinks(_m_id, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::RepoIce::IdList& _result;
        const ::std::string& _m_id;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __RepoIce__RepoApi__incomingLinks_name, ::Ice::Idempotent, __context);
    ::RepoIce::IdList __result;
    try
    {
        _DirectI __direct(__result, id, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

::RepoIce::IdList
IceDelegateD::RepoIce::RepoApi::links(const ::std::string& id, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::RepoIce::IdList& __result, const ::std::string& id, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_id(id)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::RepoIce::RepoApi* servant = dynamic_cast< ::RepoIce::RepoApi*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->links(_m_id, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::RepoIce::IdList& _result;
        const ::std::string& _m_id;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __RepoIce__RepoApi__links_name, ::Ice::Idempotent, __context);
    ::RepoIce::IdList __result;
    try
    {
        _DirectI __direct(__result, id, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

::std::string
IceDelegateD::RepoIce::RepoApi::typeName(const ::std::string& id, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::std::string& __result, const ::std::string& id, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_id(id)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::RepoIce::RepoApi* servant = dynamic_cast< ::RepoIce::RepoApi*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->typeName(_m_id, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::std::string& _result;
        const ::std::string& _m_id;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __RepoIce__RepoApi__typeName_name, ::Ice::Idempotent, __context);
    ::std::string __result;
    try
    {
        _DirectI __direct(__result, id, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

::std::string
IceDelegateD::RepoIce::RepoApi::property(const ::std::string& id, const ::std::string& propertyName, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::std::string& __result, const ::std::string& id, const ::std::string& propertyName, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_id(id),
            _m_propertyName(propertyName)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::RepoIce::RepoApi* servant = dynamic_cast< ::RepoIce::RepoApi*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->property(_m_id, _m_propertyName, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::std::string& _result;
        const ::std::string& _m_id;
        const ::std::string& _m_propertyName;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __RepoIce__RepoApi__property_name, ::Ice::Idempotent, __context);
    ::std::string __result;
    try
    {
        _DirectI __direct(__result, id, propertyName, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

::std::string
IceDelegateD::RepoIce::RepoApi::stringProperty(const ::std::string& id, const ::std::string& propertyName, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::std::string& __result, const ::std::string& id, const ::std::string& propertyName, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_id(id),
            _m_propertyName(propertyName)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::RepoIce::RepoApi* servant = dynamic_cast< ::RepoIce::RepoApi*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->stringProperty(_m_id, _m_propertyName, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::std::string& _result;
        const ::std::string& _m_id;
        const ::std::string& _m_propertyName;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __RepoIce__RepoApi__stringProperty_name, ::Ice::Idempotent, __context);
    ::std::string __result;
    try
    {
        _DirectI __direct(__result, id, propertyName, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

void
IceDelegateD::RepoIce::RepoApi::setProperty(const ::std::string& id, const ::std::string& propertyName, const ::std::string& value, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::std::string& id, const ::std::string& propertyName, const ::std::string& value, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _m_id(id),
            _m_propertyName(propertyName),
            _m_value(value)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::RepoIce::RepoApi* servant = dynamic_cast< ::RepoIce::RepoApi*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->setProperty(_m_id, _m_propertyName, _m_value, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        const ::std::string& _m_id;
        const ::std::string& _m_propertyName;
        const ::std::string& _m_value;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __RepoIce__RepoApi__setProperty_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(id, propertyName, value, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
}

void
IceDelegateD::RepoIce::RepoApi::removeProperty(const ::std::string& id, const ::std::string& propertyName, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::std::string& id, const ::std::string& propertyName, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _m_id(id),
            _m_propertyName(propertyName)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::RepoIce::RepoApi* servant = dynamic_cast< ::RepoIce::RepoApi*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->removeProperty(_m_id, _m_propertyName, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        const ::std::string& _m_id;
        const ::std::string& _m_propertyName;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __RepoIce__RepoApi__removeProperty_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(id, propertyName, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
}

bool
IceDelegateD::RepoIce::RepoApi::hasProperty(const ::std::string& id, const ::std::string& propertyName, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(bool& __result, const ::std::string& id, const ::std::string& propertyName, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_id(id),
            _m_propertyName(propertyName)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::RepoIce::RepoApi* servant = dynamic_cast< ::RepoIce::RepoApi*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->hasProperty(_m_id, _m_propertyName, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        bool& _result;
        const ::std::string& _m_id;
        const ::std::string& _m_propertyName;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __RepoIce__RepoApi__hasProperty_name, ::Ice::Idempotent, __context);
    bool __result;
    try
    {
        _DirectI __direct(__result, id, propertyName, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

::std::string
IceDelegateD::RepoIce::RepoApi::from(const ::std::string& id, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::std::string& __result, const ::std::string& id, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_id(id)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::RepoIce::RepoApi* servant = dynamic_cast< ::RepoIce::RepoApi*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->from(_m_id, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::std::string& _result;
        const ::std::string& _m_id;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __RepoIce__RepoApi__from_name, ::Ice::Idempotent, __context);
    ::std::string __result;
    try
    {
        _DirectI __direct(__result, id, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

void
IceDelegateD::RepoIce::RepoApi::setFrom(const ::std::string& id, const ::std::string& from, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::std::string& id, const ::std::string& from, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _m_id(id),
            _m_from(from)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::RepoIce::RepoApi* servant = dynamic_cast< ::RepoIce::RepoApi*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->setFrom(_m_id, _m_from, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        const ::std::string& _m_id;
        const ::std::string& _m_from;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __RepoIce__RepoApi__setFrom_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(id, from, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
}

::std::string
IceDelegateD::RepoIce::RepoApi::to(const ::std::string& id, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::std::string& __result, const ::std::string& id, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_id(id)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::RepoIce::RepoApi* servant = dynamic_cast< ::RepoIce::RepoApi*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->to(_m_id, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::std::string& _result;
        const ::std::string& _m_id;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __RepoIce__RepoApi__to_name, ::Ice::Idempotent, __context);
    ::std::string __result;
    try
    {
        _DirectI __direct(__result, id, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

void
IceDelegateD::RepoIce::RepoApi::setTo(const ::std::string& id, const ::std::string& to, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::std::string& id, const ::std::string& to, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _m_id(id),
            _m_to(to)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::RepoIce::RepoApi* servant = dynamic_cast< ::RepoIce::RepoApi*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->setTo(_m_id, _m_to, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        const ::std::string& _m_id;
        const ::std::string& _m_to;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __RepoIce__RepoApi__setTo_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(id, to, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
}

::Ice::Double
IceDelegateD::RepoIce::RepoApi::fromPort(const ::std::string& id, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::Ice::Double& __result, const ::std::string& id, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_id(id)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::RepoIce::RepoApi* servant = dynamic_cast< ::RepoIce::RepoApi*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->fromPort(_m_id, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::Ice::Double& _result;
        const ::std::string& _m_id;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __RepoIce__RepoApi__fromPort_name, ::Ice::Idempotent, __context);
    ::Ice::Double __result;
    try
    {
        _DirectI __direct(__result, id, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

void
IceDelegateD::RepoIce::RepoApi::setFromPort(const ::std::string& id, ::Ice::Double fromPort, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::std::string& id, ::Ice::Double fromPort, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _m_id(id),
            _m_fromPort(fromPort)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::RepoIce::RepoApi* servant = dynamic_cast< ::RepoIce::RepoApi*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->setFromPort(_m_id, _m_fromPort, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        const ::std::string& _m_id;
        ::Ice::Double _m_fromPort;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __RepoIce__RepoApi__setFromPort_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(id, fromPort, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
}

::Ice::Double
IceDelegateD::RepoIce::RepoApi::toPort(const ::std::string& id, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::Ice::Double& __result, const ::std::string& id, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_id(id)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::RepoIce::RepoApi* servant = dynamic_cast< ::RepoIce::RepoApi*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->toPort(_m_id, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::Ice::Double& _result;
        const ::std::string& _m_id;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __RepoIce__RepoApi__toPort_name, ::Ice::Idempotent, __context);
    ::Ice::Double __result;
    try
    {
        _DirectI __direct(__result, id, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

void
IceDelegateD::RepoIce::RepoApi::setToPort(const ::std::string& id, ::Ice::Double toPort, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::std::string& id, ::Ice::Double toPort, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _m_id(id),
            _m_toPort(toPort)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::RepoIce::RepoApi* servant = dynamic_cast< ::RepoIce::RepoApi*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->setToPort(_m_id, _m_toPort, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        const ::std::string& _m_id;
        ::Ice::Double _m_toPort;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __RepoIce__RepoApi__setToPort_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(id, toPort, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
}

::std::string
IceDelegateD::RepoIce::RepoApi::otherEntityFromLink(const ::std::string& linkId, const ::std::string& firstNode, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::std::string& __result, const ::std::string& linkId, const ::std::string& firstNode, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_linkId(linkId),
            _m_firstNode(firstNode)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::RepoIce::RepoApi* servant = dynamic_cast< ::RepoIce::RepoApi*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->otherEntityFromLink(_m_linkId, _m_firstNode, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::std::string& _result;
        const ::std::string& _m_linkId;
        const ::std::string& _m_firstNode;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __RepoIce__RepoApi__otherEntityFromLink_name, ::Ice::Idempotent, __context);
    ::std::string __result;
    try
    {
        _DirectI __direct(__result, linkId, firstNode, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

void
IceDelegateD::RepoIce::RepoApi::exterminate(const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::RepoIce::RepoApi* servant = dynamic_cast< ::RepoIce::RepoApi*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->exterminate(_current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __RepoIce__RepoApi__exterminate_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(__current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
}

void
IceDelegateD::RepoIce::RepoApi::save(const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::RepoIce::RepoApi* servant = dynamic_cast< ::RepoIce::RepoApi*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->save(_current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __RepoIce__RepoApi__save_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(__current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
}

::Ice::ObjectPtr
RepoIce::RepoApi::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __RepoIce__RepoApi_ids[2] =
{
    "::Ice::Object",
    "::RepoIce::RepoApi"
};

bool
RepoIce::RepoApi::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__RepoIce__RepoApi_ids, __RepoIce__RepoApi_ids + 2, _s);
}

::std::vector< ::std::string>
RepoIce::RepoApi::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__RepoIce__RepoApi_ids[0], &__RepoIce__RepoApi_ids[2]);
}

const ::std::string&
RepoIce::RepoApi::ice_id(const ::Ice::Current&) const
{
    return __RepoIce__RepoApi_ids[1];
}

const ::std::string&
RepoIce::RepoApi::ice_staticId()
{
    return __RepoIce__RepoApi_ids[1];
}

::Ice::DispatchStatus
RepoIce::RepoApi::___name(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string id;
    __is->read(id);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string __ret = name(id, __current);
    __os->write(__ret);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
RepoIce::RepoApi::___setName(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string id;
    ::std::string name;
    __is->read(id);
    __is->read(name);
    __is->endReadEncaps();
    setName(id, name, __current);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
RepoIce::RepoApi::___children(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string id;
    __is->read(id);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::RepoIce::IdList __ret = children(id, __current);
    if(__ret.size() == 0)
    {
        __os->writeSize(0);
    }
    else
    {
        __os->write(&__ret[0], &__ret[0] + __ret.size());
    }
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
RepoIce::RepoApi::___addChild(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string id;
    ::std::string child;
    __is->read(id);
    __is->read(child);
    __is->endReadEncaps();
    addChild(id, child, __current);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
RepoIce::RepoApi::___removeChild(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string id;
    ::std::string child;
    __is->read(id);
    __is->read(child);
    __is->endReadEncaps();
    removeChild(id, child, __current);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
RepoIce::RepoApi::___removeElement(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string id;
    __is->read(id);
    __is->endReadEncaps();
    removeElement(id, __current);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
RepoIce::RepoApi::___parents(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string id;
    __is->read(id);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::RepoIce::IdList __ret = parents(id, __current);
    if(__ret.size() == 0)
    {
        __os->writeSize(0);
    }
    else
    {
        __os->write(&__ret[0], &__ret[0] + __ret.size());
    }
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
RepoIce::RepoApi::___addParent(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string id;
    ::std::string parent;
    __is->read(id);
    __is->read(parent);
    __is->endReadEncaps();
    addParent(id, parent, __current);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
RepoIce::RepoApi::___removeParent(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string id;
    ::std::string parent;
    __is->read(id);
    __is->read(parent);
    __is->endReadEncaps();
    removeParent(id, parent, __current);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
RepoIce::RepoApi::___outcomingLinks(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string id;
    __is->read(id);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::RepoIce::IdList __ret = outcomingLinks(id, __current);
    if(__ret.size() == 0)
    {
        __os->writeSize(0);
    }
    else
    {
        __os->write(&__ret[0], &__ret[0] + __ret.size());
    }
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
RepoIce::RepoApi::___incomingLinks(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string id;
    __is->read(id);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::RepoIce::IdList __ret = incomingLinks(id, __current);
    if(__ret.size() == 0)
    {
        __os->writeSize(0);
    }
    else
    {
        __os->write(&__ret[0], &__ret[0] + __ret.size());
    }
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
RepoIce::RepoApi::___links(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string id;
    __is->read(id);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::RepoIce::IdList __ret = links(id, __current);
    if(__ret.size() == 0)
    {
        __os->writeSize(0);
    }
    else
    {
        __os->write(&__ret[0], &__ret[0] + __ret.size());
    }
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
RepoIce::RepoApi::___typeName(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string id;
    __is->read(id);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string __ret = typeName(id, __current);
    __os->write(__ret);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
RepoIce::RepoApi::___property(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string id;
    ::std::string propertyName;
    __is->read(id);
    __is->read(propertyName);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string __ret = property(id, propertyName, __current);
    __os->write(__ret);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
RepoIce::RepoApi::___stringProperty(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string id;
    ::std::string propertyName;
    __is->read(id);
    __is->read(propertyName);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string __ret = stringProperty(id, propertyName, __current);
    __os->write(__ret);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
RepoIce::RepoApi::___setProperty(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string id;
    ::std::string propertyName;
    ::std::string value;
    __is->read(id);
    __is->read(propertyName);
    __is->read(value);
    __is->endReadEncaps();
    setProperty(id, propertyName, value, __current);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
RepoIce::RepoApi::___removeProperty(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string id;
    ::std::string propertyName;
    __is->read(id);
    __is->read(propertyName);
    __is->endReadEncaps();
    removeProperty(id, propertyName, __current);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
RepoIce::RepoApi::___hasProperty(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string id;
    ::std::string propertyName;
    __is->read(id);
    __is->read(propertyName);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    bool __ret = hasProperty(id, propertyName, __current);
    __os->write(__ret);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
RepoIce::RepoApi::___from(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string id;
    __is->read(id);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string __ret = from(id, __current);
    __os->write(__ret);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
RepoIce::RepoApi::___setFrom(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string id;
    ::std::string from;
    __is->read(id);
    __is->read(from);
    __is->endReadEncaps();
    setFrom(id, from, __current);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
RepoIce::RepoApi::___to(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string id;
    __is->read(id);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string __ret = to(id, __current);
    __os->write(__ret);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
RepoIce::RepoApi::___setTo(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string id;
    ::std::string to;
    __is->read(id);
    __is->read(to);
    __is->endReadEncaps();
    setTo(id, to, __current);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
RepoIce::RepoApi::___fromPort(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string id;
    __is->read(id);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::Ice::Double __ret = fromPort(id, __current);
    __os->write(__ret);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
RepoIce::RepoApi::___setFromPort(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string id;
    ::Ice::Double fromPort;
    __is->read(id);
    __is->read(fromPort);
    __is->endReadEncaps();
    setFromPort(id, fromPort, __current);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
RepoIce::RepoApi::___toPort(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string id;
    __is->read(id);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::Ice::Double __ret = toPort(id, __current);
    __os->write(__ret);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
RepoIce::RepoApi::___setToPort(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string id;
    ::Ice::Double toPort;
    __is->read(id);
    __is->read(toPort);
    __is->endReadEncaps();
    setToPort(id, toPort, __current);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
RepoIce::RepoApi::___otherEntityFromLink(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string linkId;
    ::std::string firstNode;
    __is->read(linkId);
    __is->read(firstNode);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string __ret = otherEntityFromLink(linkId, firstNode, __current);
    __os->write(__ret);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
RepoIce::RepoApi::___exterminate(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    __inS.is()->skipEmptyEncaps();
    exterminate(__current);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
RepoIce::RepoApi::___save(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    __inS.is()->skipEmptyEncaps();
    save(__current);
    return ::Ice::DispatchOK;
}

static ::std::string __RepoIce__RepoApi_all[] =
{
    "addChild",
    "addParent",
    "children",
    "exterminate",
    "from",
    "fromPort",
    "hasProperty",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping",
    "incomingLinks",
    "links",
    "name",
    "otherEntityFromLink",
    "outcomingLinks",
    "parents",
    "property",
    "removeChild",
    "removeElement",
    "removeParent",
    "removeProperty",
    "save",
    "setFrom",
    "setFromPort",
    "setName",
    "setProperty",
    "setTo",
    "setToPort",
    "stringProperty",
    "to",
    "toPort",
    "typeName"
};

::Ice::DispatchStatus
RepoIce::RepoApi::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__RepoIce__RepoApi_all, __RepoIce__RepoApi_all + 33, current.operation);
    if(r.first == r.second)
    {
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __RepoIce__RepoApi_all)
    {
        case 0:
        {
            return ___addChild(in, current);
        }
        case 1:
        {
            return ___addParent(in, current);
        }
        case 2:
        {
            return ___children(in, current);
        }
        case 3:
        {
            return ___exterminate(in, current);
        }
        case 4:
        {
            return ___from(in, current);
        }
        case 5:
        {
            return ___fromPort(in, current);
        }
        case 6:
        {
            return ___hasProperty(in, current);
        }
        case 7:
        {
            return ___ice_id(in, current);
        }
        case 8:
        {
            return ___ice_ids(in, current);
        }
        case 9:
        {
            return ___ice_isA(in, current);
        }
        case 10:
        {
            return ___ice_ping(in, current);
        }
        case 11:
        {
            return ___incomingLinks(in, current);
        }
        case 12:
        {
            return ___links(in, current);
        }
        case 13:
        {
            return ___name(in, current);
        }
        case 14:
        {
            return ___otherEntityFromLink(in, current);
        }
        case 15:
        {
            return ___outcomingLinks(in, current);
        }
        case 16:
        {
            return ___parents(in, current);
        }
        case 17:
        {
            return ___property(in, current);
        }
        case 18:
        {
            return ___removeChild(in, current);
        }
        case 19:
        {
            return ___removeElement(in, current);
        }
        case 20:
        {
            return ___removeParent(in, current);
        }
        case 21:
        {
            return ___removeProperty(in, current);
        }
        case 22:
        {
            return ___save(in, current);
        }
        case 23:
        {
            return ___setFrom(in, current);
        }
        case 24:
        {
            return ___setFromPort(in, current);
        }
        case 25:
        {
            return ___setName(in, current);
        }
        case 26:
        {
            return ___setProperty(in, current);
        }
        case 27:
        {
            return ___setTo(in, current);
        }
        case 28:
        {
            return ___setToPort(in, current);
        }
        case 29:
        {
            return ___stringProperty(in, current);
        }
        case 30:
        {
            return ___to(in, current);
        }
        case 31:
        {
            return ___toPort(in, current);
        }
        case 32:
        {
            return ___typeName(in, current);
        }
    }

    assert(false);
    throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
}

void
RepoIce::RepoApi::__write(::IceInternal::BasicStream* __os) const
{
    __os->writeTypeId(ice_staticId());
    __os->startWriteSlice();
    __os->endWriteSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__write(__os);
#else
    ::Ice::Object::__write(__os);
#endif
}

void
RepoIce::RepoApi::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->readTypeId(myId);
    }
    __is->startReadSlice();
    __is->endReadSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__read(__is, true);
#else
    ::Ice::Object::__read(__is, true);
#endif
}

void
RepoIce::RepoApi::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type RepoIce::RepoApi was not generated with stream support";
    throw ex;
}

void
RepoIce::RepoApi::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type RepoIce::RepoApi was not generated with stream support";
    throw ex;
}

void 
RepoIce::__patch__RepoApiPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::RepoIce::RepoApiPtr* p = static_cast< ::RepoIce::RepoApiPtr*>(__addr);
    assert(p);
    *p = ::RepoIce::RepoApiPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::RepoIce::RepoApi::ice_staticId(), v->ice_id());
    }
}

bool
RepoIce::operator==(const ::RepoIce::RepoApi& l, const ::RepoIce::RepoApi& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
RepoIce::operator<(const ::RepoIce::RepoApi& l, const ::RepoIce::RepoApi& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}
