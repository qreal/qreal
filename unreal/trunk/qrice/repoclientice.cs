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

#if __MonoCS__

using _System = System;
using _Microsoft = Microsoft;
#else

using _System = global::System;
using _Microsoft = global::Microsoft;
#endif

namespace RepoIce
{
    public abstract class ROOTID
    {
        public const string value = "qrm:/ROOT_ID/ROOT_ID/ROOT_ID/ROOT_ID";
    }

    public interface RepoApi : Ice.Object, RepoApiOperations_, RepoApiOperationsNC_
    {
    }
}

namespace RepoIce
{
    public interface RepoApiPrx : Ice.ObjectPrx
    {
        string name(string id);
        string name(string id, _System.Collections.Generic.Dictionary<string, string> context__);

        void setName(string id, string name);
        void setName(string id, string name, _System.Collections.Generic.Dictionary<string, string> context__);

        string[] children(string id);
        string[] children(string id, _System.Collections.Generic.Dictionary<string, string> context__);

        void addChild(string id, string child);
        void addChild(string id, string child, _System.Collections.Generic.Dictionary<string, string> context__);

        void removeChild(string id, string child);
        void removeChild(string id, string child, _System.Collections.Generic.Dictionary<string, string> context__);

        void removeElement(string id);
        void removeElement(string id, _System.Collections.Generic.Dictionary<string, string> context__);

        string[] parents(string id);
        string[] parents(string id, _System.Collections.Generic.Dictionary<string, string> context__);

        void addParent(string id, string parent);
        void addParent(string id, string parent, _System.Collections.Generic.Dictionary<string, string> context__);

        void removeParent(string id, string parent);
        void removeParent(string id, string parent, _System.Collections.Generic.Dictionary<string, string> context__);

        string[] outcomingLinks(string id);
        string[] outcomingLinks(string id, _System.Collections.Generic.Dictionary<string, string> context__);

        string[] incomingLinks(string id);
        string[] incomingLinks(string id, _System.Collections.Generic.Dictionary<string, string> context__);

        string[] links(string id);
        string[] links(string id, _System.Collections.Generic.Dictionary<string, string> context__);

        string typeName(string id);
        string typeName(string id, _System.Collections.Generic.Dictionary<string, string> context__);

        string property(string id, string propertyName);
        string property(string id, string propertyName, _System.Collections.Generic.Dictionary<string, string> context__);

        string stringProperty(string id, string propertyName);
        string stringProperty(string id, string propertyName, _System.Collections.Generic.Dictionary<string, string> context__);

        void setProperty(string id, string propertyName, string value);
        void setProperty(string id, string propertyName, string value, _System.Collections.Generic.Dictionary<string, string> context__);

        void removeProperty(string id, string propertyName);
        void removeProperty(string id, string propertyName, _System.Collections.Generic.Dictionary<string, string> context__);

        bool hasProperty(string id, string propertyName);
        bool hasProperty(string id, string propertyName, _System.Collections.Generic.Dictionary<string, string> context__);

        string from(string id);
        string from(string id, _System.Collections.Generic.Dictionary<string, string> context__);

        void setFrom(string id, string from);
        void setFrom(string id, string from, _System.Collections.Generic.Dictionary<string, string> context__);

        string to(string id);
        string to(string id, _System.Collections.Generic.Dictionary<string, string> context__);

        void setTo(string id, string to);
        void setTo(string id, string to, _System.Collections.Generic.Dictionary<string, string> context__);

        double fromPort(string id);
        double fromPort(string id, _System.Collections.Generic.Dictionary<string, string> context__);

        void setFromPort(string id, double fromPort);
        void setFromPort(string id, double fromPort, _System.Collections.Generic.Dictionary<string, string> context__);

        double toPort(string id);
        double toPort(string id, _System.Collections.Generic.Dictionary<string, string> context__);

        void setToPort(string id, double toPort);
        void setToPort(string id, double toPort, _System.Collections.Generic.Dictionary<string, string> context__);

        string otherEntityFromLink(string linkId, string firstNode);
        string otherEntityFromLink(string linkId, string firstNode, _System.Collections.Generic.Dictionary<string, string> context__);

        void exterminate();
        void exterminate(_System.Collections.Generic.Dictionary<string, string> context__);

        void save();
        void save(_System.Collections.Generic.Dictionary<string, string> context__);
    }
}

namespace RepoIce
{
    public interface RepoApiOperations_
    {
        string name(string id, Ice.Current current__);

        void setName(string id, string name, Ice.Current current__);

        string[] children(string id, Ice.Current current__);

        void addChild(string id, string child, Ice.Current current__);

        void removeChild(string id, string child, Ice.Current current__);

        void removeElement(string id, Ice.Current current__);

        string[] parents(string id, Ice.Current current__);

        void addParent(string id, string parent, Ice.Current current__);

        void removeParent(string id, string parent, Ice.Current current__);

        string[] outcomingLinks(string id, Ice.Current current__);

        string[] incomingLinks(string id, Ice.Current current__);

        string[] links(string id, Ice.Current current__);

        string typeName(string id, Ice.Current current__);

        string property(string id, string propertyName, Ice.Current current__);

        string stringProperty(string id, string propertyName, Ice.Current current__);

        void setProperty(string id, string propertyName, string value, Ice.Current current__);

        void removeProperty(string id, string propertyName, Ice.Current current__);

        bool hasProperty(string id, string propertyName, Ice.Current current__);

        string from(string id, Ice.Current current__);

        void setFrom(string id, string from, Ice.Current current__);

        string to(string id, Ice.Current current__);

        void setTo(string id, string to, Ice.Current current__);

        double fromPort(string id, Ice.Current current__);

        void setFromPort(string id, double fromPort, Ice.Current current__);

        double toPort(string id, Ice.Current current__);

        void setToPort(string id, double toPort, Ice.Current current__);

        string otherEntityFromLink(string linkId, string firstNode, Ice.Current current__);

        void exterminate(Ice.Current current__);

        void save(Ice.Current current__);
    }

    public interface RepoApiOperationsNC_
    {
        string name(string id);

        void setName(string id, string name);

        string[] children(string id);

        void addChild(string id, string child);

        void removeChild(string id, string child);

        void removeElement(string id);

        string[] parents(string id);

        void addParent(string id, string parent);

        void removeParent(string id, string parent);

        string[] outcomingLinks(string id);

        string[] incomingLinks(string id);

        string[] links(string id);

        string typeName(string id);

        string property(string id, string propertyName);

        string stringProperty(string id, string propertyName);

        void setProperty(string id, string propertyName, string value);

        void removeProperty(string id, string propertyName);

        bool hasProperty(string id, string propertyName);

        string from(string id);

        void setFrom(string id, string from);

        string to(string id);

        void setTo(string id, string to);

        double fromPort(string id);

        void setFromPort(string id, double fromPort);

        double toPort(string id);

        void setToPort(string id, double toPort);

        string otherEntityFromLink(string linkId, string firstNode);

        void exterminate();

        void save();
    }
}

namespace RepoIce
{
    public sealed class IdListHelper
    {
        public static void write(IceInternal.BasicStream os__, string[] v__)
        {
            os__.writeStringSeq(v__);
        }

        public static string[] read(IceInternal.BasicStream is__)
        {
            string[] v__;
            v__ = is__.readStringSeq();
            return v__;
        }
    }

    public sealed class RepoApiPrxHelper : Ice.ObjectPrxHelperBase, RepoApiPrx
    {
        #region Synchronous operations

        public void addChild(string id, string child)
        {
            addChild(id, child, null, false);
        }

        public void addChild(string id, string child, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            addChild(id, child, context__, true);
        }

        private void addChild(string id, string child, _System.Collections.Generic.Dictionary<string, string> context__, bool explicitContext__)
        {
            if(explicitContext__ && context__ == null)
            {
                context__ = emptyContext_;
            }
            int cnt__ = 0;
            while(true)
            {
                Ice.ObjectDel_ delBase__ = null;
                try
                {
                    delBase__ = getDelegate__(false);
                    RepoApiDel_ del__ = (RepoApiDel_)delBase__;
                    del__.addChild(id, child, context__);
                    return;
                }
                catch(IceInternal.LocalExceptionWrapper ex__)
                {
                    handleExceptionWrapper__(delBase__, ex__, null);
                }
                catch(Ice.LocalException ex__)
                {
                    handleException__(delBase__, ex__, null, ref cnt__);
                }
            }
        }

        public void addParent(string id, string parent)
        {
            addParent(id, parent, null, false);
        }

        public void addParent(string id, string parent, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            addParent(id, parent, context__, true);
        }

        private void addParent(string id, string parent, _System.Collections.Generic.Dictionary<string, string> context__, bool explicitContext__)
        {
            if(explicitContext__ && context__ == null)
            {
                context__ = emptyContext_;
            }
            int cnt__ = 0;
            while(true)
            {
                Ice.ObjectDel_ delBase__ = null;
                try
                {
                    delBase__ = getDelegate__(false);
                    RepoApiDel_ del__ = (RepoApiDel_)delBase__;
                    del__.addParent(id, parent, context__);
                    return;
                }
                catch(IceInternal.LocalExceptionWrapper ex__)
                {
                    handleExceptionWrapper__(delBase__, ex__, null);
                }
                catch(Ice.LocalException ex__)
                {
                    handleException__(delBase__, ex__, null, ref cnt__);
                }
            }
        }

        public string[] children(string id)
        {
            return children(id, null, false);
        }

        public string[] children(string id, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            return children(id, context__, true);
        }

        private string[] children(string id, _System.Collections.Generic.Dictionary<string, string> context__, bool explicitContext__)
        {
            if(explicitContext__ && context__ == null)
            {
                context__ = emptyContext_;
            }
            int cnt__ = 0;
            while(true)
            {
                Ice.ObjectDel_ delBase__ = null;
                try
                {
                    checkTwowayOnly__("children");
                    delBase__ = getDelegate__(false);
                    RepoApiDel_ del__ = (RepoApiDel_)delBase__;
                    return del__.children(id, context__);
                }
                catch(IceInternal.LocalExceptionWrapper ex__)
                {
                    handleExceptionWrapper__(delBase__, ex__, null);
                }
                catch(Ice.LocalException ex__)
                {
                    handleException__(delBase__, ex__, null, ref cnt__);
                }
            }
        }

        public void exterminate()
        {
            exterminate(null, false);
        }

        public void exterminate(_System.Collections.Generic.Dictionary<string, string> context__)
        {
            exterminate(context__, true);
        }

        private void exterminate(_System.Collections.Generic.Dictionary<string, string> context__, bool explicitContext__)
        {
            if(explicitContext__ && context__ == null)
            {
                context__ = emptyContext_;
            }
            int cnt__ = 0;
            while(true)
            {
                Ice.ObjectDel_ delBase__ = null;
                try
                {
                    delBase__ = getDelegate__(false);
                    RepoApiDel_ del__ = (RepoApiDel_)delBase__;
                    del__.exterminate(context__);
                    return;
                }
                catch(IceInternal.LocalExceptionWrapper ex__)
                {
                    handleExceptionWrapper__(delBase__, ex__, null);
                }
                catch(Ice.LocalException ex__)
                {
                    handleException__(delBase__, ex__, null, ref cnt__);
                }
            }
        }

        public string from(string id)
        {
            return from(id, null, false);
        }

        public string from(string id, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            return from(id, context__, true);
        }

        private string from(string id, _System.Collections.Generic.Dictionary<string, string> context__, bool explicitContext__)
        {
            if(explicitContext__ && context__ == null)
            {
                context__ = emptyContext_;
            }
            int cnt__ = 0;
            while(true)
            {
                Ice.ObjectDel_ delBase__ = null;
                try
                {
                    checkTwowayOnly__("from");
                    delBase__ = getDelegate__(false);
                    RepoApiDel_ del__ = (RepoApiDel_)delBase__;
                    return del__.from(id, context__);
                }
                catch(IceInternal.LocalExceptionWrapper ex__)
                {
                    handleExceptionWrapperRelaxed__(delBase__, ex__, null, ref cnt__);
                }
                catch(Ice.LocalException ex__)
                {
                    handleException__(delBase__, ex__, null, ref cnt__);
                }
            }
        }

        public double fromPort(string id)
        {
            return fromPort(id, null, false);
        }

        public double fromPort(string id, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            return fromPort(id, context__, true);
        }

        private double fromPort(string id, _System.Collections.Generic.Dictionary<string, string> context__, bool explicitContext__)
        {
            if(explicitContext__ && context__ == null)
            {
                context__ = emptyContext_;
            }
            int cnt__ = 0;
            while(true)
            {
                Ice.ObjectDel_ delBase__ = null;
                try
                {
                    checkTwowayOnly__("fromPort");
                    delBase__ = getDelegate__(false);
                    RepoApiDel_ del__ = (RepoApiDel_)delBase__;
                    return del__.fromPort(id, context__);
                }
                catch(IceInternal.LocalExceptionWrapper ex__)
                {
                    handleExceptionWrapperRelaxed__(delBase__, ex__, null, ref cnt__);
                }
                catch(Ice.LocalException ex__)
                {
                    handleException__(delBase__, ex__, null, ref cnt__);
                }
            }
        }

        public bool hasProperty(string id, string propertyName)
        {
            return hasProperty(id, propertyName, null, false);
        }

        public bool hasProperty(string id, string propertyName, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            return hasProperty(id, propertyName, context__, true);
        }

        private bool hasProperty(string id, string propertyName, _System.Collections.Generic.Dictionary<string, string> context__, bool explicitContext__)
        {
            if(explicitContext__ && context__ == null)
            {
                context__ = emptyContext_;
            }
            int cnt__ = 0;
            while(true)
            {
                Ice.ObjectDel_ delBase__ = null;
                try
                {
                    checkTwowayOnly__("hasProperty");
                    delBase__ = getDelegate__(false);
                    RepoApiDel_ del__ = (RepoApiDel_)delBase__;
                    return del__.hasProperty(id, propertyName, context__);
                }
                catch(IceInternal.LocalExceptionWrapper ex__)
                {
                    handleExceptionWrapperRelaxed__(delBase__, ex__, null, ref cnt__);
                }
                catch(Ice.LocalException ex__)
                {
                    handleException__(delBase__, ex__, null, ref cnt__);
                }
            }
        }

        public string[] incomingLinks(string id)
        {
            return incomingLinks(id, null, false);
        }

        public string[] incomingLinks(string id, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            return incomingLinks(id, context__, true);
        }

        private string[] incomingLinks(string id, _System.Collections.Generic.Dictionary<string, string> context__, bool explicitContext__)
        {
            if(explicitContext__ && context__ == null)
            {
                context__ = emptyContext_;
            }
            int cnt__ = 0;
            while(true)
            {
                Ice.ObjectDel_ delBase__ = null;
                try
                {
                    checkTwowayOnly__("incomingLinks");
                    delBase__ = getDelegate__(false);
                    RepoApiDel_ del__ = (RepoApiDel_)delBase__;
                    return del__.incomingLinks(id, context__);
                }
                catch(IceInternal.LocalExceptionWrapper ex__)
                {
                    handleExceptionWrapperRelaxed__(delBase__, ex__, null, ref cnt__);
                }
                catch(Ice.LocalException ex__)
                {
                    handleException__(delBase__, ex__, null, ref cnt__);
                }
            }
        }

        public string[] links(string id)
        {
            return links(id, null, false);
        }

        public string[] links(string id, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            return links(id, context__, true);
        }

        private string[] links(string id, _System.Collections.Generic.Dictionary<string, string> context__, bool explicitContext__)
        {
            if(explicitContext__ && context__ == null)
            {
                context__ = emptyContext_;
            }
            int cnt__ = 0;
            while(true)
            {
                Ice.ObjectDel_ delBase__ = null;
                try
                {
                    checkTwowayOnly__("links");
                    delBase__ = getDelegate__(false);
                    RepoApiDel_ del__ = (RepoApiDel_)delBase__;
                    return del__.links(id, context__);
                }
                catch(IceInternal.LocalExceptionWrapper ex__)
                {
                    handleExceptionWrapperRelaxed__(delBase__, ex__, null, ref cnt__);
                }
                catch(Ice.LocalException ex__)
                {
                    handleException__(delBase__, ex__, null, ref cnt__);
                }
            }
        }

        public string name(string id)
        {
            return name(id, null, false);
        }

        public string name(string id, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            return name(id, context__, true);
        }

        private string name(string id, _System.Collections.Generic.Dictionary<string, string> context__, bool explicitContext__)
        {
            if(explicitContext__ && context__ == null)
            {
                context__ = emptyContext_;
            }
            int cnt__ = 0;
            while(true)
            {
                Ice.ObjectDel_ delBase__ = null;
                try
                {
                    checkTwowayOnly__("name");
                    delBase__ = getDelegate__(false);
                    RepoApiDel_ del__ = (RepoApiDel_)delBase__;
                    return del__.name(id, context__);
                }
                catch(IceInternal.LocalExceptionWrapper ex__)
                {
                    handleExceptionWrapperRelaxed__(delBase__, ex__, null, ref cnt__);
                }
                catch(Ice.LocalException ex__)
                {
                    handleException__(delBase__, ex__, null, ref cnt__);
                }
            }
        }

        public string otherEntityFromLink(string linkId, string firstNode)
        {
            return otherEntityFromLink(linkId, firstNode, null, false);
        }

        public string otherEntityFromLink(string linkId, string firstNode, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            return otherEntityFromLink(linkId, firstNode, context__, true);
        }

        private string otherEntityFromLink(string linkId, string firstNode, _System.Collections.Generic.Dictionary<string, string> context__, bool explicitContext__)
        {
            if(explicitContext__ && context__ == null)
            {
                context__ = emptyContext_;
            }
            int cnt__ = 0;
            while(true)
            {
                Ice.ObjectDel_ delBase__ = null;
                try
                {
                    checkTwowayOnly__("otherEntityFromLink");
                    delBase__ = getDelegate__(false);
                    RepoApiDel_ del__ = (RepoApiDel_)delBase__;
                    return del__.otherEntityFromLink(linkId, firstNode, context__);
                }
                catch(IceInternal.LocalExceptionWrapper ex__)
                {
                    handleExceptionWrapperRelaxed__(delBase__, ex__, null, ref cnt__);
                }
                catch(Ice.LocalException ex__)
                {
                    handleException__(delBase__, ex__, null, ref cnt__);
                }
            }
        }

        public string[] outcomingLinks(string id)
        {
            return outcomingLinks(id, null, false);
        }

        public string[] outcomingLinks(string id, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            return outcomingLinks(id, context__, true);
        }

        private string[] outcomingLinks(string id, _System.Collections.Generic.Dictionary<string, string> context__, bool explicitContext__)
        {
            if(explicitContext__ && context__ == null)
            {
                context__ = emptyContext_;
            }
            int cnt__ = 0;
            while(true)
            {
                Ice.ObjectDel_ delBase__ = null;
                try
                {
                    checkTwowayOnly__("outcomingLinks");
                    delBase__ = getDelegate__(false);
                    RepoApiDel_ del__ = (RepoApiDel_)delBase__;
                    return del__.outcomingLinks(id, context__);
                }
                catch(IceInternal.LocalExceptionWrapper ex__)
                {
                    handleExceptionWrapperRelaxed__(delBase__, ex__, null, ref cnt__);
                }
                catch(Ice.LocalException ex__)
                {
                    handleException__(delBase__, ex__, null, ref cnt__);
                }
            }
        }

        public string[] parents(string id)
        {
            return parents(id, null, false);
        }

        public string[] parents(string id, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            return parents(id, context__, true);
        }

        private string[] parents(string id, _System.Collections.Generic.Dictionary<string, string> context__, bool explicitContext__)
        {
            if(explicitContext__ && context__ == null)
            {
                context__ = emptyContext_;
            }
            int cnt__ = 0;
            while(true)
            {
                Ice.ObjectDel_ delBase__ = null;
                try
                {
                    checkTwowayOnly__("parents");
                    delBase__ = getDelegate__(false);
                    RepoApiDel_ del__ = (RepoApiDel_)delBase__;
                    return del__.parents(id, context__);
                }
                catch(IceInternal.LocalExceptionWrapper ex__)
                {
                    handleExceptionWrapperRelaxed__(delBase__, ex__, null, ref cnt__);
                }
                catch(Ice.LocalException ex__)
                {
                    handleException__(delBase__, ex__, null, ref cnt__);
                }
            }
        }

        public string property(string id, string propertyName)
        {
            return property(id, propertyName, null, false);
        }

        public string property(string id, string propertyName, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            return property(id, propertyName, context__, true);
        }

        private string property(string id, string propertyName, _System.Collections.Generic.Dictionary<string, string> context__, bool explicitContext__)
        {
            if(explicitContext__ && context__ == null)
            {
                context__ = emptyContext_;
            }
            int cnt__ = 0;
            while(true)
            {
                Ice.ObjectDel_ delBase__ = null;
                try
                {
                    checkTwowayOnly__("property");
                    delBase__ = getDelegate__(false);
                    RepoApiDel_ del__ = (RepoApiDel_)delBase__;
                    return del__.property(id, propertyName, context__);
                }
                catch(IceInternal.LocalExceptionWrapper ex__)
                {
                    handleExceptionWrapperRelaxed__(delBase__, ex__, null, ref cnt__);
                }
                catch(Ice.LocalException ex__)
                {
                    handleException__(delBase__, ex__, null, ref cnt__);
                }
            }
        }

        public void removeChild(string id, string child)
        {
            removeChild(id, child, null, false);
        }

        public void removeChild(string id, string child, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            removeChild(id, child, context__, true);
        }

        private void removeChild(string id, string child, _System.Collections.Generic.Dictionary<string, string> context__, bool explicitContext__)
        {
            if(explicitContext__ && context__ == null)
            {
                context__ = emptyContext_;
            }
            int cnt__ = 0;
            while(true)
            {
                Ice.ObjectDel_ delBase__ = null;
                try
                {
                    delBase__ = getDelegate__(false);
                    RepoApiDel_ del__ = (RepoApiDel_)delBase__;
                    del__.removeChild(id, child, context__);
                    return;
                }
                catch(IceInternal.LocalExceptionWrapper ex__)
                {
                    handleExceptionWrapper__(delBase__, ex__, null);
                }
                catch(Ice.LocalException ex__)
                {
                    handleException__(delBase__, ex__, null, ref cnt__);
                }
            }
        }

        public void removeElement(string id)
        {
            removeElement(id, null, false);
        }

        public void removeElement(string id, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            removeElement(id, context__, true);
        }

        private void removeElement(string id, _System.Collections.Generic.Dictionary<string, string> context__, bool explicitContext__)
        {
            if(explicitContext__ && context__ == null)
            {
                context__ = emptyContext_;
            }
            int cnt__ = 0;
            while(true)
            {
                Ice.ObjectDel_ delBase__ = null;
                try
                {
                    delBase__ = getDelegate__(false);
                    RepoApiDel_ del__ = (RepoApiDel_)delBase__;
                    del__.removeElement(id, context__);
                    return;
                }
                catch(IceInternal.LocalExceptionWrapper ex__)
                {
                    handleExceptionWrapper__(delBase__, ex__, null);
                }
                catch(Ice.LocalException ex__)
                {
                    handleException__(delBase__, ex__, null, ref cnt__);
                }
            }
        }

        public void removeParent(string id, string parent)
        {
            removeParent(id, parent, null, false);
        }

        public void removeParent(string id, string parent, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            removeParent(id, parent, context__, true);
        }

        private void removeParent(string id, string parent, _System.Collections.Generic.Dictionary<string, string> context__, bool explicitContext__)
        {
            if(explicitContext__ && context__ == null)
            {
                context__ = emptyContext_;
            }
            int cnt__ = 0;
            while(true)
            {
                Ice.ObjectDel_ delBase__ = null;
                try
                {
                    delBase__ = getDelegate__(false);
                    RepoApiDel_ del__ = (RepoApiDel_)delBase__;
                    del__.removeParent(id, parent, context__);
                    return;
                }
                catch(IceInternal.LocalExceptionWrapper ex__)
                {
                    handleExceptionWrapper__(delBase__, ex__, null);
                }
                catch(Ice.LocalException ex__)
                {
                    handleException__(delBase__, ex__, null, ref cnt__);
                }
            }
        }

        public void removeProperty(string id, string propertyName)
        {
            removeProperty(id, propertyName, null, false);
        }

        public void removeProperty(string id, string propertyName, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            removeProperty(id, propertyName, context__, true);
        }

        private void removeProperty(string id, string propertyName, _System.Collections.Generic.Dictionary<string, string> context__, bool explicitContext__)
        {
            if(explicitContext__ && context__ == null)
            {
                context__ = emptyContext_;
            }
            int cnt__ = 0;
            while(true)
            {
                Ice.ObjectDel_ delBase__ = null;
                try
                {
                    delBase__ = getDelegate__(false);
                    RepoApiDel_ del__ = (RepoApiDel_)delBase__;
                    del__.removeProperty(id, propertyName, context__);
                    return;
                }
                catch(IceInternal.LocalExceptionWrapper ex__)
                {
                    handleExceptionWrapper__(delBase__, ex__, null);
                }
                catch(Ice.LocalException ex__)
                {
                    handleException__(delBase__, ex__, null, ref cnt__);
                }
            }
        }

        public void save()
        {
            save(null, false);
        }

        public void save(_System.Collections.Generic.Dictionary<string, string> context__)
        {
            save(context__, true);
        }

        private void save(_System.Collections.Generic.Dictionary<string, string> context__, bool explicitContext__)
        {
            if(explicitContext__ && context__ == null)
            {
                context__ = emptyContext_;
            }
            int cnt__ = 0;
            while(true)
            {
                Ice.ObjectDel_ delBase__ = null;
                try
                {
                    delBase__ = getDelegate__(false);
                    RepoApiDel_ del__ = (RepoApiDel_)delBase__;
                    del__.save(context__);
                    return;
                }
                catch(IceInternal.LocalExceptionWrapper ex__)
                {
                    handleExceptionWrapper__(delBase__, ex__, null);
                }
                catch(Ice.LocalException ex__)
                {
                    handleException__(delBase__, ex__, null, ref cnt__);
                }
            }
        }

        public void setFrom(string id, string from)
        {
            setFrom(id, from, null, false);
        }

        public void setFrom(string id, string from, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            setFrom(id, from, context__, true);
        }

        private void setFrom(string id, string from, _System.Collections.Generic.Dictionary<string, string> context__, bool explicitContext__)
        {
            if(explicitContext__ && context__ == null)
            {
                context__ = emptyContext_;
            }
            int cnt__ = 0;
            while(true)
            {
                Ice.ObjectDel_ delBase__ = null;
                try
                {
                    delBase__ = getDelegate__(false);
                    RepoApiDel_ del__ = (RepoApiDel_)delBase__;
                    del__.setFrom(id, from, context__);
                    return;
                }
                catch(IceInternal.LocalExceptionWrapper ex__)
                {
                    handleExceptionWrapper__(delBase__, ex__, null);
                }
                catch(Ice.LocalException ex__)
                {
                    handleException__(delBase__, ex__, null, ref cnt__);
                }
            }
        }

        public void setFromPort(string id, double fromPort)
        {
            setFromPort(id, fromPort, null, false);
        }

        public void setFromPort(string id, double fromPort, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            setFromPort(id, fromPort, context__, true);
        }

        private void setFromPort(string id, double fromPort, _System.Collections.Generic.Dictionary<string, string> context__, bool explicitContext__)
        {
            if(explicitContext__ && context__ == null)
            {
                context__ = emptyContext_;
            }
            int cnt__ = 0;
            while(true)
            {
                Ice.ObjectDel_ delBase__ = null;
                try
                {
                    delBase__ = getDelegate__(false);
                    RepoApiDel_ del__ = (RepoApiDel_)delBase__;
                    del__.setFromPort(id, fromPort, context__);
                    return;
                }
                catch(IceInternal.LocalExceptionWrapper ex__)
                {
                    handleExceptionWrapper__(delBase__, ex__, null);
                }
                catch(Ice.LocalException ex__)
                {
                    handleException__(delBase__, ex__, null, ref cnt__);
                }
            }
        }

        public void setName(string id, string name)
        {
            setName(id, name, null, false);
        }

        public void setName(string id, string name, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            setName(id, name, context__, true);
        }

        private void setName(string id, string name, _System.Collections.Generic.Dictionary<string, string> context__, bool explicitContext__)
        {
            if(explicitContext__ && context__ == null)
            {
                context__ = emptyContext_;
            }
            int cnt__ = 0;
            while(true)
            {
                Ice.ObjectDel_ delBase__ = null;
                try
                {
                    delBase__ = getDelegate__(false);
                    RepoApiDel_ del__ = (RepoApiDel_)delBase__;
                    del__.setName(id, name, context__);
                    return;
                }
                catch(IceInternal.LocalExceptionWrapper ex__)
                {
                    handleExceptionWrapper__(delBase__, ex__, null);
                }
                catch(Ice.LocalException ex__)
                {
                    handleException__(delBase__, ex__, null, ref cnt__);
                }
            }
        }

        public void setProperty(string id, string propertyName, string value)
        {
            setProperty(id, propertyName, value, null, false);
        }

        public void setProperty(string id, string propertyName, string value, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            setProperty(id, propertyName, value, context__, true);
        }

        private void setProperty(string id, string propertyName, string value, _System.Collections.Generic.Dictionary<string, string> context__, bool explicitContext__)
        {
            if(explicitContext__ && context__ == null)
            {
                context__ = emptyContext_;
            }
            int cnt__ = 0;
            while(true)
            {
                Ice.ObjectDel_ delBase__ = null;
                try
                {
                    delBase__ = getDelegate__(false);
                    RepoApiDel_ del__ = (RepoApiDel_)delBase__;
                    del__.setProperty(id, propertyName, value, context__);
                    return;
                }
                catch(IceInternal.LocalExceptionWrapper ex__)
                {
                    handleExceptionWrapper__(delBase__, ex__, null);
                }
                catch(Ice.LocalException ex__)
                {
                    handleException__(delBase__, ex__, null, ref cnt__);
                }
            }
        }

        public void setTo(string id, string to)
        {
            setTo(id, to, null, false);
        }

        public void setTo(string id, string to, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            setTo(id, to, context__, true);
        }

        private void setTo(string id, string to, _System.Collections.Generic.Dictionary<string, string> context__, bool explicitContext__)
        {
            if(explicitContext__ && context__ == null)
            {
                context__ = emptyContext_;
            }
            int cnt__ = 0;
            while(true)
            {
                Ice.ObjectDel_ delBase__ = null;
                try
                {
                    delBase__ = getDelegate__(false);
                    RepoApiDel_ del__ = (RepoApiDel_)delBase__;
                    del__.setTo(id, to, context__);
                    return;
                }
                catch(IceInternal.LocalExceptionWrapper ex__)
                {
                    handleExceptionWrapper__(delBase__, ex__, null);
                }
                catch(Ice.LocalException ex__)
                {
                    handleException__(delBase__, ex__, null, ref cnt__);
                }
            }
        }

        public void setToPort(string id, double toPort)
        {
            setToPort(id, toPort, null, false);
        }

        public void setToPort(string id, double toPort, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            setToPort(id, toPort, context__, true);
        }

        private void setToPort(string id, double toPort, _System.Collections.Generic.Dictionary<string, string> context__, bool explicitContext__)
        {
            if(explicitContext__ && context__ == null)
            {
                context__ = emptyContext_;
            }
            int cnt__ = 0;
            while(true)
            {
                Ice.ObjectDel_ delBase__ = null;
                try
                {
                    delBase__ = getDelegate__(false);
                    RepoApiDel_ del__ = (RepoApiDel_)delBase__;
                    del__.setToPort(id, toPort, context__);
                    return;
                }
                catch(IceInternal.LocalExceptionWrapper ex__)
                {
                    handleExceptionWrapper__(delBase__, ex__, null);
                }
                catch(Ice.LocalException ex__)
                {
                    handleException__(delBase__, ex__, null, ref cnt__);
                }
            }
        }

        public string stringProperty(string id, string propertyName)
        {
            return stringProperty(id, propertyName, null, false);
        }

        public string stringProperty(string id, string propertyName, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            return stringProperty(id, propertyName, context__, true);
        }

        private string stringProperty(string id, string propertyName, _System.Collections.Generic.Dictionary<string, string> context__, bool explicitContext__)
        {
            if(explicitContext__ && context__ == null)
            {
                context__ = emptyContext_;
            }
            int cnt__ = 0;
            while(true)
            {
                Ice.ObjectDel_ delBase__ = null;
                try
                {
                    checkTwowayOnly__("stringProperty");
                    delBase__ = getDelegate__(false);
                    RepoApiDel_ del__ = (RepoApiDel_)delBase__;
                    return del__.stringProperty(id, propertyName, context__);
                }
                catch(IceInternal.LocalExceptionWrapper ex__)
                {
                    handleExceptionWrapperRelaxed__(delBase__, ex__, null, ref cnt__);
                }
                catch(Ice.LocalException ex__)
                {
                    handleException__(delBase__, ex__, null, ref cnt__);
                }
            }
        }

        public string to(string id)
        {
            return to(id, null, false);
        }

        public string to(string id, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            return to(id, context__, true);
        }

        private string to(string id, _System.Collections.Generic.Dictionary<string, string> context__, bool explicitContext__)
        {
            if(explicitContext__ && context__ == null)
            {
                context__ = emptyContext_;
            }
            int cnt__ = 0;
            while(true)
            {
                Ice.ObjectDel_ delBase__ = null;
                try
                {
                    checkTwowayOnly__("to");
                    delBase__ = getDelegate__(false);
                    RepoApiDel_ del__ = (RepoApiDel_)delBase__;
                    return del__.to(id, context__);
                }
                catch(IceInternal.LocalExceptionWrapper ex__)
                {
                    handleExceptionWrapperRelaxed__(delBase__, ex__, null, ref cnt__);
                }
                catch(Ice.LocalException ex__)
                {
                    handleException__(delBase__, ex__, null, ref cnt__);
                }
            }
        }

        public double toPort(string id)
        {
            return toPort(id, null, false);
        }

        public double toPort(string id, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            return toPort(id, context__, true);
        }

        private double toPort(string id, _System.Collections.Generic.Dictionary<string, string> context__, bool explicitContext__)
        {
            if(explicitContext__ && context__ == null)
            {
                context__ = emptyContext_;
            }
            int cnt__ = 0;
            while(true)
            {
                Ice.ObjectDel_ delBase__ = null;
                try
                {
                    checkTwowayOnly__("toPort");
                    delBase__ = getDelegate__(false);
                    RepoApiDel_ del__ = (RepoApiDel_)delBase__;
                    return del__.toPort(id, context__);
                }
                catch(IceInternal.LocalExceptionWrapper ex__)
                {
                    handleExceptionWrapperRelaxed__(delBase__, ex__, null, ref cnt__);
                }
                catch(Ice.LocalException ex__)
                {
                    handleException__(delBase__, ex__, null, ref cnt__);
                }
            }
        }

        public string typeName(string id)
        {
            return typeName(id, null, false);
        }

        public string typeName(string id, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            return typeName(id, context__, true);
        }

        private string typeName(string id, _System.Collections.Generic.Dictionary<string, string> context__, bool explicitContext__)
        {
            if(explicitContext__ && context__ == null)
            {
                context__ = emptyContext_;
            }
            int cnt__ = 0;
            while(true)
            {
                Ice.ObjectDel_ delBase__ = null;
                try
                {
                    checkTwowayOnly__("typeName");
                    delBase__ = getDelegate__(false);
                    RepoApiDel_ del__ = (RepoApiDel_)delBase__;
                    return del__.typeName(id, context__);
                }
                catch(IceInternal.LocalExceptionWrapper ex__)
                {
                    handleExceptionWrapperRelaxed__(delBase__, ex__, null, ref cnt__);
                }
                catch(Ice.LocalException ex__)
                {
                    handleException__(delBase__, ex__, null, ref cnt__);
                }
            }
        }

        #endregion

        #region Checked and unchecked cast operations

        public static RepoApiPrx checkedCast(Ice.ObjectPrx b)
        {
            if(b == null)
            {
                return null;
            }
            RepoApiPrx r = b as RepoApiPrx;
            if((r == null) && b.ice_isA("::RepoIce::RepoApi"))
            {
                RepoApiPrxHelper h = new RepoApiPrxHelper();
                h.copyFrom__(b);
                r = h;
            }
            return r;
        }

        public static RepoApiPrx checkedCast(Ice.ObjectPrx b, _System.Collections.Generic.Dictionary<string, string> ctx)
        {
            if(b == null)
            {
                return null;
            }
            RepoApiPrx r = b as RepoApiPrx;
            if((r == null) && b.ice_isA("::RepoIce::RepoApi", ctx))
            {
                RepoApiPrxHelper h = new RepoApiPrxHelper();
                h.copyFrom__(b);
                r = h;
            }
            return r;
        }

        public static RepoApiPrx checkedCast(Ice.ObjectPrx b, string f)
        {
            if(b == null)
            {
                return null;
            }
            Ice.ObjectPrx bb = b.ice_facet(f);
            try
            {
                if(bb.ice_isA("::RepoIce::RepoApi"))
                {
                    RepoApiPrxHelper h = new RepoApiPrxHelper();
                    h.copyFrom__(bb);
                    return h;
                }
            }
            catch(Ice.FacetNotExistException)
            {
            }
            return null;
        }

        public static RepoApiPrx checkedCast(Ice.ObjectPrx b, string f, _System.Collections.Generic.Dictionary<string, string> ctx)
        {
            if(b == null)
            {
                return null;
            }
            Ice.ObjectPrx bb = b.ice_facet(f);
            try
            {
                if(bb.ice_isA("::RepoIce::RepoApi", ctx))
                {
                    RepoApiPrxHelper h = new RepoApiPrxHelper();
                    h.copyFrom__(bb);
                    return h;
                }
            }
            catch(Ice.FacetNotExistException)
            {
            }
            return null;
        }

        public static RepoApiPrx uncheckedCast(Ice.ObjectPrx b)
        {
            if(b == null)
            {
                return null;
            }
            RepoApiPrx r = b as RepoApiPrx;
            if(r == null)
            {
                RepoApiPrxHelper h = new RepoApiPrxHelper();
                h.copyFrom__(b);
                r = h;
            }
            return r;
        }

        public static RepoApiPrx uncheckedCast(Ice.ObjectPrx b, string f)
        {
            if(b == null)
            {
                return null;
            }
            Ice.ObjectPrx bb = b.ice_facet(f);
            RepoApiPrxHelper h = new RepoApiPrxHelper();
            h.copyFrom__(bb);
            return h;
        }

        #endregion

        #region Marshaling support

        protected override Ice.ObjectDelM_ createDelegateM__()
        {
            return new RepoApiDelM_();
        }

        protected override Ice.ObjectDelD_ createDelegateD__()
        {
            return new RepoApiDelD_();
        }

        public static void write__(IceInternal.BasicStream os__, RepoApiPrx v__)
        {
            os__.writeProxy(v__);
        }

        public static RepoApiPrx read__(IceInternal.BasicStream is__)
        {
            Ice.ObjectPrx proxy = is__.readProxy();
            if(proxy != null)
            {
                RepoApiPrxHelper result = new RepoApiPrxHelper();
                result.copyFrom__(proxy);
                return result;
            }
            return null;
        }

        #endregion
    }
}

namespace RepoIce
{
    public interface RepoApiDel_ : Ice.ObjectDel_
    {
        string name(string id, _System.Collections.Generic.Dictionary<string, string> context__);

        void setName(string id, string name, _System.Collections.Generic.Dictionary<string, string> context__);

        string[] children(string id, _System.Collections.Generic.Dictionary<string, string> context__);

        void addChild(string id, string child, _System.Collections.Generic.Dictionary<string, string> context__);

        void removeChild(string id, string child, _System.Collections.Generic.Dictionary<string, string> context__);

        void removeElement(string id, _System.Collections.Generic.Dictionary<string, string> context__);

        string[] parents(string id, _System.Collections.Generic.Dictionary<string, string> context__);

        void addParent(string id, string parent, _System.Collections.Generic.Dictionary<string, string> context__);

        void removeParent(string id, string parent, _System.Collections.Generic.Dictionary<string, string> context__);

        string[] outcomingLinks(string id, _System.Collections.Generic.Dictionary<string, string> context__);

        string[] incomingLinks(string id, _System.Collections.Generic.Dictionary<string, string> context__);

        string[] links(string id, _System.Collections.Generic.Dictionary<string, string> context__);

        string typeName(string id, _System.Collections.Generic.Dictionary<string, string> context__);

        string property(string id, string propertyName, _System.Collections.Generic.Dictionary<string, string> context__);

        string stringProperty(string id, string propertyName, _System.Collections.Generic.Dictionary<string, string> context__);

        void setProperty(string id, string propertyName, string value, _System.Collections.Generic.Dictionary<string, string> context__);

        void removeProperty(string id, string propertyName, _System.Collections.Generic.Dictionary<string, string> context__);

        bool hasProperty(string id, string propertyName, _System.Collections.Generic.Dictionary<string, string> context__);

        string from(string id, _System.Collections.Generic.Dictionary<string, string> context__);

        void setFrom(string id, string from, _System.Collections.Generic.Dictionary<string, string> context__);

        string to(string id, _System.Collections.Generic.Dictionary<string, string> context__);

        void setTo(string id, string to, _System.Collections.Generic.Dictionary<string, string> context__);

        double fromPort(string id, _System.Collections.Generic.Dictionary<string, string> context__);

        void setFromPort(string id, double fromPort, _System.Collections.Generic.Dictionary<string, string> context__);

        double toPort(string id, _System.Collections.Generic.Dictionary<string, string> context__);

        void setToPort(string id, double toPort, _System.Collections.Generic.Dictionary<string, string> context__);

        string otherEntityFromLink(string linkId, string firstNode, _System.Collections.Generic.Dictionary<string, string> context__);

        void exterminate(_System.Collections.Generic.Dictionary<string, string> context__);

        void save(_System.Collections.Generic.Dictionary<string, string> context__);
    }
}

namespace RepoIce
{
    public sealed class RepoApiDelM_ : Ice.ObjectDelM_, RepoApiDel_
    {
        public void addChild(string id, string child, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            IceInternal.Outgoing og__ = handler__.getOutgoing("addChild", Ice.OperationMode.Normal, context__);
            try
            {
                try
                {
                    IceInternal.BasicStream os__ = og__.ostr();
                    os__.writeString(id);
                    os__.writeString(child);
                }
                catch(Ice.LocalException ex__)
                {
                    og__.abort(ex__);
                }
                bool ok__ = og__.invoke();
                if(!og__.istr().isEmpty())
                {
                    try
                    {
                        if(!ok__)
                        {
                            try
                            {
                                og__.throwUserException();
                            }
                            catch(Ice.UserException ex)
                            {
                                throw new Ice.UnknownUserException(ex.ice_name(), ex);
                            }
                        }
                        og__.istr().skipEmptyEncaps();
                    }
                    catch(Ice.LocalException ex__)
                    {
                        throw new IceInternal.LocalExceptionWrapper(ex__, false);
                    }
                }
            }
            finally
            {
                handler__.reclaimOutgoing(og__);
            }
        }

        public void addParent(string id, string parent, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            IceInternal.Outgoing og__ = handler__.getOutgoing("addParent", Ice.OperationMode.Normal, context__);
            try
            {
                try
                {
                    IceInternal.BasicStream os__ = og__.ostr();
                    os__.writeString(id);
                    os__.writeString(parent);
                }
                catch(Ice.LocalException ex__)
                {
                    og__.abort(ex__);
                }
                bool ok__ = og__.invoke();
                if(!og__.istr().isEmpty())
                {
                    try
                    {
                        if(!ok__)
                        {
                            try
                            {
                                og__.throwUserException();
                            }
                            catch(Ice.UserException ex)
                            {
                                throw new Ice.UnknownUserException(ex.ice_name(), ex);
                            }
                        }
                        og__.istr().skipEmptyEncaps();
                    }
                    catch(Ice.LocalException ex__)
                    {
                        throw new IceInternal.LocalExceptionWrapper(ex__, false);
                    }
                }
            }
            finally
            {
                handler__.reclaimOutgoing(og__);
            }
        }

        public string[] children(string id, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            IceInternal.Outgoing og__ = handler__.getOutgoing("children", Ice.OperationMode.Normal, context__);
            try
            {
                try
                {
                    IceInternal.BasicStream os__ = og__.ostr();
                    os__.writeString(id);
                }
                catch(Ice.LocalException ex__)
                {
                    og__.abort(ex__);
                }
                bool ok__ = og__.invoke();
                try
                {
                    if(!ok__)
                    {
                        try
                        {
                            og__.throwUserException();
                        }
                        catch(Ice.UserException ex)
                        {
                            throw new Ice.UnknownUserException(ex.ice_name(), ex);
                        }
                    }
                    IceInternal.BasicStream is__ = og__.istr();
                    is__.startReadEncaps();
                    string[] ret__;
                    ret__ = is__.readStringSeq();
                    is__.endReadEncaps();
                    return ret__;
                }
                catch(Ice.LocalException ex__)
                {
                    throw new IceInternal.LocalExceptionWrapper(ex__, false);
                }
            }
            finally
            {
                handler__.reclaimOutgoing(og__);
            }
        }

        public void exterminate(_System.Collections.Generic.Dictionary<string, string> context__)
        {
            IceInternal.Outgoing og__ = handler__.getOutgoing("exterminate", Ice.OperationMode.Normal, context__);
            try
            {
                bool ok__ = og__.invoke();
                if(!og__.istr().isEmpty())
                {
                    try
                    {
                        if(!ok__)
                        {
                            try
                            {
                                og__.throwUserException();
                            }
                            catch(Ice.UserException ex)
                            {
                                throw new Ice.UnknownUserException(ex.ice_name(), ex);
                            }
                        }
                        og__.istr().skipEmptyEncaps();
                    }
                    catch(Ice.LocalException ex__)
                    {
                        throw new IceInternal.LocalExceptionWrapper(ex__, false);
                    }
                }
            }
            finally
            {
                handler__.reclaimOutgoing(og__);
            }
        }

        public string from(string id, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            IceInternal.Outgoing og__ = handler__.getOutgoing("from", Ice.OperationMode.Idempotent, context__);
            try
            {
                try
                {
                    IceInternal.BasicStream os__ = og__.ostr();
                    os__.writeString(id);
                }
                catch(Ice.LocalException ex__)
                {
                    og__.abort(ex__);
                }
                bool ok__ = og__.invoke();
                try
                {
                    if(!ok__)
                    {
                        try
                        {
                            og__.throwUserException();
                        }
                        catch(Ice.UserException ex)
                        {
                            throw new Ice.UnknownUserException(ex.ice_name(), ex);
                        }
                    }
                    IceInternal.BasicStream is__ = og__.istr();
                    is__.startReadEncaps();
                    string ret__;
                    ret__ = is__.readString();
                    is__.endReadEncaps();
                    return ret__;
                }
                catch(Ice.LocalException ex__)
                {
                    throw new IceInternal.LocalExceptionWrapper(ex__, false);
                }
            }
            finally
            {
                handler__.reclaimOutgoing(og__);
            }
        }

        public double fromPort(string id, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            IceInternal.Outgoing og__ = handler__.getOutgoing("fromPort", Ice.OperationMode.Idempotent, context__);
            try
            {
                try
                {
                    IceInternal.BasicStream os__ = og__.ostr();
                    os__.writeString(id);
                }
                catch(Ice.LocalException ex__)
                {
                    og__.abort(ex__);
                }
                bool ok__ = og__.invoke();
                try
                {
                    if(!ok__)
                    {
                        try
                        {
                            og__.throwUserException();
                        }
                        catch(Ice.UserException ex)
                        {
                            throw new Ice.UnknownUserException(ex.ice_name(), ex);
                        }
                    }
                    IceInternal.BasicStream is__ = og__.istr();
                    is__.startReadEncaps();
                    double ret__;
                    ret__ = is__.readDouble();
                    is__.endReadEncaps();
                    return ret__;
                }
                catch(Ice.LocalException ex__)
                {
                    throw new IceInternal.LocalExceptionWrapper(ex__, false);
                }
            }
            finally
            {
                handler__.reclaimOutgoing(og__);
            }
        }

        public bool hasProperty(string id, string propertyName, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            IceInternal.Outgoing og__ = handler__.getOutgoing("hasProperty", Ice.OperationMode.Idempotent, context__);
            try
            {
                try
                {
                    IceInternal.BasicStream os__ = og__.ostr();
                    os__.writeString(id);
                    os__.writeString(propertyName);
                }
                catch(Ice.LocalException ex__)
                {
                    og__.abort(ex__);
                }
                bool ok__ = og__.invoke();
                try
                {
                    if(!ok__)
                    {
                        try
                        {
                            og__.throwUserException();
                        }
                        catch(Ice.UserException ex)
                        {
                            throw new Ice.UnknownUserException(ex.ice_name(), ex);
                        }
                    }
                    IceInternal.BasicStream is__ = og__.istr();
                    is__.startReadEncaps();
                    bool ret__;
                    ret__ = is__.readBool();
                    is__.endReadEncaps();
                    return ret__;
                }
                catch(Ice.LocalException ex__)
                {
                    throw new IceInternal.LocalExceptionWrapper(ex__, false);
                }
            }
            finally
            {
                handler__.reclaimOutgoing(og__);
            }
        }

        public string[] incomingLinks(string id, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            IceInternal.Outgoing og__ = handler__.getOutgoing("incomingLinks", Ice.OperationMode.Idempotent, context__);
            try
            {
                try
                {
                    IceInternal.BasicStream os__ = og__.ostr();
                    os__.writeString(id);
                }
                catch(Ice.LocalException ex__)
                {
                    og__.abort(ex__);
                }
                bool ok__ = og__.invoke();
                try
                {
                    if(!ok__)
                    {
                        try
                        {
                            og__.throwUserException();
                        }
                        catch(Ice.UserException ex)
                        {
                            throw new Ice.UnknownUserException(ex.ice_name(), ex);
                        }
                    }
                    IceInternal.BasicStream is__ = og__.istr();
                    is__.startReadEncaps();
                    string[] ret__;
                    ret__ = is__.readStringSeq();
                    is__.endReadEncaps();
                    return ret__;
                }
                catch(Ice.LocalException ex__)
                {
                    throw new IceInternal.LocalExceptionWrapper(ex__, false);
                }
            }
            finally
            {
                handler__.reclaimOutgoing(og__);
            }
        }

        public string[] links(string id, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            IceInternal.Outgoing og__ = handler__.getOutgoing("links", Ice.OperationMode.Idempotent, context__);
            try
            {
                try
                {
                    IceInternal.BasicStream os__ = og__.ostr();
                    os__.writeString(id);
                }
                catch(Ice.LocalException ex__)
                {
                    og__.abort(ex__);
                }
                bool ok__ = og__.invoke();
                try
                {
                    if(!ok__)
                    {
                        try
                        {
                            og__.throwUserException();
                        }
                        catch(Ice.UserException ex)
                        {
                            throw new Ice.UnknownUserException(ex.ice_name(), ex);
                        }
                    }
                    IceInternal.BasicStream is__ = og__.istr();
                    is__.startReadEncaps();
                    string[] ret__;
                    ret__ = is__.readStringSeq();
                    is__.endReadEncaps();
                    return ret__;
                }
                catch(Ice.LocalException ex__)
                {
                    throw new IceInternal.LocalExceptionWrapper(ex__, false);
                }
            }
            finally
            {
                handler__.reclaimOutgoing(og__);
            }
        }

        public string name(string id, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            IceInternal.Outgoing og__ = handler__.getOutgoing("name", Ice.OperationMode.Idempotent, context__);
            try
            {
                try
                {
                    IceInternal.BasicStream os__ = og__.ostr();
                    os__.writeString(id);
                }
                catch(Ice.LocalException ex__)
                {
                    og__.abort(ex__);
                }
                bool ok__ = og__.invoke();
                try
                {
                    if(!ok__)
                    {
                        try
                        {
                            og__.throwUserException();
                        }
                        catch(Ice.UserException ex)
                        {
                            throw new Ice.UnknownUserException(ex.ice_name(), ex);
                        }
                    }
                    IceInternal.BasicStream is__ = og__.istr();
                    is__.startReadEncaps();
                    string ret__;
                    ret__ = is__.readString();
                    is__.endReadEncaps();
                    return ret__;
                }
                catch(Ice.LocalException ex__)
                {
                    throw new IceInternal.LocalExceptionWrapper(ex__, false);
                }
            }
            finally
            {
                handler__.reclaimOutgoing(og__);
            }
        }

        public string otherEntityFromLink(string linkId, string firstNode, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            IceInternal.Outgoing og__ = handler__.getOutgoing("otherEntityFromLink", Ice.OperationMode.Idempotent, context__);
            try
            {
                try
                {
                    IceInternal.BasicStream os__ = og__.ostr();
                    os__.writeString(linkId);
                    os__.writeString(firstNode);
                }
                catch(Ice.LocalException ex__)
                {
                    og__.abort(ex__);
                }
                bool ok__ = og__.invoke();
                try
                {
                    if(!ok__)
                    {
                        try
                        {
                            og__.throwUserException();
                        }
                        catch(Ice.UserException ex)
                        {
                            throw new Ice.UnknownUserException(ex.ice_name(), ex);
                        }
                    }
                    IceInternal.BasicStream is__ = og__.istr();
                    is__.startReadEncaps();
                    string ret__;
                    ret__ = is__.readString();
                    is__.endReadEncaps();
                    return ret__;
                }
                catch(Ice.LocalException ex__)
                {
                    throw new IceInternal.LocalExceptionWrapper(ex__, false);
                }
            }
            finally
            {
                handler__.reclaimOutgoing(og__);
            }
        }

        public string[] outcomingLinks(string id, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            IceInternal.Outgoing og__ = handler__.getOutgoing("outcomingLinks", Ice.OperationMode.Idempotent, context__);
            try
            {
                try
                {
                    IceInternal.BasicStream os__ = og__.ostr();
                    os__.writeString(id);
                }
                catch(Ice.LocalException ex__)
                {
                    og__.abort(ex__);
                }
                bool ok__ = og__.invoke();
                try
                {
                    if(!ok__)
                    {
                        try
                        {
                            og__.throwUserException();
                        }
                        catch(Ice.UserException ex)
                        {
                            throw new Ice.UnknownUserException(ex.ice_name(), ex);
                        }
                    }
                    IceInternal.BasicStream is__ = og__.istr();
                    is__.startReadEncaps();
                    string[] ret__;
                    ret__ = is__.readStringSeq();
                    is__.endReadEncaps();
                    return ret__;
                }
                catch(Ice.LocalException ex__)
                {
                    throw new IceInternal.LocalExceptionWrapper(ex__, false);
                }
            }
            finally
            {
                handler__.reclaimOutgoing(og__);
            }
        }

        public string[] parents(string id, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            IceInternal.Outgoing og__ = handler__.getOutgoing("parents", Ice.OperationMode.Idempotent, context__);
            try
            {
                try
                {
                    IceInternal.BasicStream os__ = og__.ostr();
                    os__.writeString(id);
                }
                catch(Ice.LocalException ex__)
                {
                    og__.abort(ex__);
                }
                bool ok__ = og__.invoke();
                try
                {
                    if(!ok__)
                    {
                        try
                        {
                            og__.throwUserException();
                        }
                        catch(Ice.UserException ex)
                        {
                            throw new Ice.UnknownUserException(ex.ice_name(), ex);
                        }
                    }
                    IceInternal.BasicStream is__ = og__.istr();
                    is__.startReadEncaps();
                    string[] ret__;
                    ret__ = is__.readStringSeq();
                    is__.endReadEncaps();
                    return ret__;
                }
                catch(Ice.LocalException ex__)
                {
                    throw new IceInternal.LocalExceptionWrapper(ex__, false);
                }
            }
            finally
            {
                handler__.reclaimOutgoing(og__);
            }
        }

        public string property(string id, string propertyName, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            IceInternal.Outgoing og__ = handler__.getOutgoing("property", Ice.OperationMode.Idempotent, context__);
            try
            {
                try
                {
                    IceInternal.BasicStream os__ = og__.ostr();
                    os__.writeString(id);
                    os__.writeString(propertyName);
                }
                catch(Ice.LocalException ex__)
                {
                    og__.abort(ex__);
                }
                bool ok__ = og__.invoke();
                try
                {
                    if(!ok__)
                    {
                        try
                        {
                            og__.throwUserException();
                        }
                        catch(Ice.UserException ex)
                        {
                            throw new Ice.UnknownUserException(ex.ice_name(), ex);
                        }
                    }
                    IceInternal.BasicStream is__ = og__.istr();
                    is__.startReadEncaps();
                    string ret__;
                    ret__ = is__.readString();
                    is__.endReadEncaps();
                    return ret__;
                }
                catch(Ice.LocalException ex__)
                {
                    throw new IceInternal.LocalExceptionWrapper(ex__, false);
                }
            }
            finally
            {
                handler__.reclaimOutgoing(og__);
            }
        }

        public void removeChild(string id, string child, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            IceInternal.Outgoing og__ = handler__.getOutgoing("removeChild", Ice.OperationMode.Normal, context__);
            try
            {
                try
                {
                    IceInternal.BasicStream os__ = og__.ostr();
                    os__.writeString(id);
                    os__.writeString(child);
                }
                catch(Ice.LocalException ex__)
                {
                    og__.abort(ex__);
                }
                bool ok__ = og__.invoke();
                if(!og__.istr().isEmpty())
                {
                    try
                    {
                        if(!ok__)
                        {
                            try
                            {
                                og__.throwUserException();
                            }
                            catch(Ice.UserException ex)
                            {
                                throw new Ice.UnknownUserException(ex.ice_name(), ex);
                            }
                        }
                        og__.istr().skipEmptyEncaps();
                    }
                    catch(Ice.LocalException ex__)
                    {
                        throw new IceInternal.LocalExceptionWrapper(ex__, false);
                    }
                }
            }
            finally
            {
                handler__.reclaimOutgoing(og__);
            }
        }

        public void removeElement(string id, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            IceInternal.Outgoing og__ = handler__.getOutgoing("removeElement", Ice.OperationMode.Normal, context__);
            try
            {
                try
                {
                    IceInternal.BasicStream os__ = og__.ostr();
                    os__.writeString(id);
                }
                catch(Ice.LocalException ex__)
                {
                    og__.abort(ex__);
                }
                bool ok__ = og__.invoke();
                if(!og__.istr().isEmpty())
                {
                    try
                    {
                        if(!ok__)
                        {
                            try
                            {
                                og__.throwUserException();
                            }
                            catch(Ice.UserException ex)
                            {
                                throw new Ice.UnknownUserException(ex.ice_name(), ex);
                            }
                        }
                        og__.istr().skipEmptyEncaps();
                    }
                    catch(Ice.LocalException ex__)
                    {
                        throw new IceInternal.LocalExceptionWrapper(ex__, false);
                    }
                }
            }
            finally
            {
                handler__.reclaimOutgoing(og__);
            }
        }

        public void removeParent(string id, string parent, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            IceInternal.Outgoing og__ = handler__.getOutgoing("removeParent", Ice.OperationMode.Normal, context__);
            try
            {
                try
                {
                    IceInternal.BasicStream os__ = og__.ostr();
                    os__.writeString(id);
                    os__.writeString(parent);
                }
                catch(Ice.LocalException ex__)
                {
                    og__.abort(ex__);
                }
                bool ok__ = og__.invoke();
                if(!og__.istr().isEmpty())
                {
                    try
                    {
                        if(!ok__)
                        {
                            try
                            {
                                og__.throwUserException();
                            }
                            catch(Ice.UserException ex)
                            {
                                throw new Ice.UnknownUserException(ex.ice_name(), ex);
                            }
                        }
                        og__.istr().skipEmptyEncaps();
                    }
                    catch(Ice.LocalException ex__)
                    {
                        throw new IceInternal.LocalExceptionWrapper(ex__, false);
                    }
                }
            }
            finally
            {
                handler__.reclaimOutgoing(og__);
            }
        }

        public void removeProperty(string id, string propertyName, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            IceInternal.Outgoing og__ = handler__.getOutgoing("removeProperty", Ice.OperationMode.Normal, context__);
            try
            {
                try
                {
                    IceInternal.BasicStream os__ = og__.ostr();
                    os__.writeString(id);
                    os__.writeString(propertyName);
                }
                catch(Ice.LocalException ex__)
                {
                    og__.abort(ex__);
                }
                bool ok__ = og__.invoke();
                if(!og__.istr().isEmpty())
                {
                    try
                    {
                        if(!ok__)
                        {
                            try
                            {
                                og__.throwUserException();
                            }
                            catch(Ice.UserException ex)
                            {
                                throw new Ice.UnknownUserException(ex.ice_name(), ex);
                            }
                        }
                        og__.istr().skipEmptyEncaps();
                    }
                    catch(Ice.LocalException ex__)
                    {
                        throw new IceInternal.LocalExceptionWrapper(ex__, false);
                    }
                }
            }
            finally
            {
                handler__.reclaimOutgoing(og__);
            }
        }

        public void save(_System.Collections.Generic.Dictionary<string, string> context__)
        {
            IceInternal.Outgoing og__ = handler__.getOutgoing("save", Ice.OperationMode.Normal, context__);
            try
            {
                bool ok__ = og__.invoke();
                if(!og__.istr().isEmpty())
                {
                    try
                    {
                        if(!ok__)
                        {
                            try
                            {
                                og__.throwUserException();
                            }
                            catch(Ice.UserException ex)
                            {
                                throw new Ice.UnknownUserException(ex.ice_name(), ex);
                            }
                        }
                        og__.istr().skipEmptyEncaps();
                    }
                    catch(Ice.LocalException ex__)
                    {
                        throw new IceInternal.LocalExceptionWrapper(ex__, false);
                    }
                }
            }
            finally
            {
                handler__.reclaimOutgoing(og__);
            }
        }

        public void setFrom(string id, string from, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            IceInternal.Outgoing og__ = handler__.getOutgoing("setFrom", Ice.OperationMode.Normal, context__);
            try
            {
                try
                {
                    IceInternal.BasicStream os__ = og__.ostr();
                    os__.writeString(id);
                    os__.writeString(from);
                }
                catch(Ice.LocalException ex__)
                {
                    og__.abort(ex__);
                }
                bool ok__ = og__.invoke();
                if(!og__.istr().isEmpty())
                {
                    try
                    {
                        if(!ok__)
                        {
                            try
                            {
                                og__.throwUserException();
                            }
                            catch(Ice.UserException ex)
                            {
                                throw new Ice.UnknownUserException(ex.ice_name(), ex);
                            }
                        }
                        og__.istr().skipEmptyEncaps();
                    }
                    catch(Ice.LocalException ex__)
                    {
                        throw new IceInternal.LocalExceptionWrapper(ex__, false);
                    }
                }
            }
            finally
            {
                handler__.reclaimOutgoing(og__);
            }
        }

        public void setFromPort(string id, double fromPort, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            IceInternal.Outgoing og__ = handler__.getOutgoing("setFromPort", Ice.OperationMode.Normal, context__);
            try
            {
                try
                {
                    IceInternal.BasicStream os__ = og__.ostr();
                    os__.writeString(id);
                    os__.writeDouble(fromPort);
                }
                catch(Ice.LocalException ex__)
                {
                    og__.abort(ex__);
                }
                bool ok__ = og__.invoke();
                if(!og__.istr().isEmpty())
                {
                    try
                    {
                        if(!ok__)
                        {
                            try
                            {
                                og__.throwUserException();
                            }
                            catch(Ice.UserException ex)
                            {
                                throw new Ice.UnknownUserException(ex.ice_name(), ex);
                            }
                        }
                        og__.istr().skipEmptyEncaps();
                    }
                    catch(Ice.LocalException ex__)
                    {
                        throw new IceInternal.LocalExceptionWrapper(ex__, false);
                    }
                }
            }
            finally
            {
                handler__.reclaimOutgoing(og__);
            }
        }

        public void setName(string id, string name, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            IceInternal.Outgoing og__ = handler__.getOutgoing("setName", Ice.OperationMode.Normal, context__);
            try
            {
                try
                {
                    IceInternal.BasicStream os__ = og__.ostr();
                    os__.writeString(id);
                    os__.writeString(name);
                }
                catch(Ice.LocalException ex__)
                {
                    og__.abort(ex__);
                }
                bool ok__ = og__.invoke();
                if(!og__.istr().isEmpty())
                {
                    try
                    {
                        if(!ok__)
                        {
                            try
                            {
                                og__.throwUserException();
                            }
                            catch(Ice.UserException ex)
                            {
                                throw new Ice.UnknownUserException(ex.ice_name(), ex);
                            }
                        }
                        og__.istr().skipEmptyEncaps();
                    }
                    catch(Ice.LocalException ex__)
                    {
                        throw new IceInternal.LocalExceptionWrapper(ex__, false);
                    }
                }
            }
            finally
            {
                handler__.reclaimOutgoing(og__);
            }
        }

        public void setProperty(string id, string propertyName, string value, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            IceInternal.Outgoing og__ = handler__.getOutgoing("setProperty", Ice.OperationMode.Normal, context__);
            try
            {
                try
                {
                    IceInternal.BasicStream os__ = og__.ostr();
                    os__.writeString(id);
                    os__.writeString(propertyName);
                    os__.writeString(value);
                }
                catch(Ice.LocalException ex__)
                {
                    og__.abort(ex__);
                }
                bool ok__ = og__.invoke();
                if(!og__.istr().isEmpty())
                {
                    try
                    {
                        if(!ok__)
                        {
                            try
                            {
                                og__.throwUserException();
                            }
                            catch(Ice.UserException ex)
                            {
                                throw new Ice.UnknownUserException(ex.ice_name(), ex);
                            }
                        }
                        og__.istr().skipEmptyEncaps();
                    }
                    catch(Ice.LocalException ex__)
                    {
                        throw new IceInternal.LocalExceptionWrapper(ex__, false);
                    }
                }
            }
            finally
            {
                handler__.reclaimOutgoing(og__);
            }
        }

        public void setTo(string id, string to, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            IceInternal.Outgoing og__ = handler__.getOutgoing("setTo", Ice.OperationMode.Normal, context__);
            try
            {
                try
                {
                    IceInternal.BasicStream os__ = og__.ostr();
                    os__.writeString(id);
                    os__.writeString(to);
                }
                catch(Ice.LocalException ex__)
                {
                    og__.abort(ex__);
                }
                bool ok__ = og__.invoke();
                if(!og__.istr().isEmpty())
                {
                    try
                    {
                        if(!ok__)
                        {
                            try
                            {
                                og__.throwUserException();
                            }
                            catch(Ice.UserException ex)
                            {
                                throw new Ice.UnknownUserException(ex.ice_name(), ex);
                            }
                        }
                        og__.istr().skipEmptyEncaps();
                    }
                    catch(Ice.LocalException ex__)
                    {
                        throw new IceInternal.LocalExceptionWrapper(ex__, false);
                    }
                }
            }
            finally
            {
                handler__.reclaimOutgoing(og__);
            }
        }

        public void setToPort(string id, double toPort, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            IceInternal.Outgoing og__ = handler__.getOutgoing("setToPort", Ice.OperationMode.Normal, context__);
            try
            {
                try
                {
                    IceInternal.BasicStream os__ = og__.ostr();
                    os__.writeString(id);
                    os__.writeDouble(toPort);
                }
                catch(Ice.LocalException ex__)
                {
                    og__.abort(ex__);
                }
                bool ok__ = og__.invoke();
                if(!og__.istr().isEmpty())
                {
                    try
                    {
                        if(!ok__)
                        {
                            try
                            {
                                og__.throwUserException();
                            }
                            catch(Ice.UserException ex)
                            {
                                throw new Ice.UnknownUserException(ex.ice_name(), ex);
                            }
                        }
                        og__.istr().skipEmptyEncaps();
                    }
                    catch(Ice.LocalException ex__)
                    {
                        throw new IceInternal.LocalExceptionWrapper(ex__, false);
                    }
                }
            }
            finally
            {
                handler__.reclaimOutgoing(og__);
            }
        }

        public string stringProperty(string id, string propertyName, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            IceInternal.Outgoing og__ = handler__.getOutgoing("stringProperty", Ice.OperationMode.Idempotent, context__);
            try
            {
                try
                {
                    IceInternal.BasicStream os__ = og__.ostr();
                    os__.writeString(id);
                    os__.writeString(propertyName);
                }
                catch(Ice.LocalException ex__)
                {
                    og__.abort(ex__);
                }
                bool ok__ = og__.invoke();
                try
                {
                    if(!ok__)
                    {
                        try
                        {
                            og__.throwUserException();
                        }
                        catch(Ice.UserException ex)
                        {
                            throw new Ice.UnknownUserException(ex.ice_name(), ex);
                        }
                    }
                    IceInternal.BasicStream is__ = og__.istr();
                    is__.startReadEncaps();
                    string ret__;
                    ret__ = is__.readString();
                    is__.endReadEncaps();
                    return ret__;
                }
                catch(Ice.LocalException ex__)
                {
                    throw new IceInternal.LocalExceptionWrapper(ex__, false);
                }
            }
            finally
            {
                handler__.reclaimOutgoing(og__);
            }
        }

        public string to(string id, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            IceInternal.Outgoing og__ = handler__.getOutgoing("to", Ice.OperationMode.Idempotent, context__);
            try
            {
                try
                {
                    IceInternal.BasicStream os__ = og__.ostr();
                    os__.writeString(id);
                }
                catch(Ice.LocalException ex__)
                {
                    og__.abort(ex__);
                }
                bool ok__ = og__.invoke();
                try
                {
                    if(!ok__)
                    {
                        try
                        {
                            og__.throwUserException();
                        }
                        catch(Ice.UserException ex)
                        {
                            throw new Ice.UnknownUserException(ex.ice_name(), ex);
                        }
                    }
                    IceInternal.BasicStream is__ = og__.istr();
                    is__.startReadEncaps();
                    string ret__;
                    ret__ = is__.readString();
                    is__.endReadEncaps();
                    return ret__;
                }
                catch(Ice.LocalException ex__)
                {
                    throw new IceInternal.LocalExceptionWrapper(ex__, false);
                }
            }
            finally
            {
                handler__.reclaimOutgoing(og__);
            }
        }

        public double toPort(string id, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            IceInternal.Outgoing og__ = handler__.getOutgoing("toPort", Ice.OperationMode.Idempotent, context__);
            try
            {
                try
                {
                    IceInternal.BasicStream os__ = og__.ostr();
                    os__.writeString(id);
                }
                catch(Ice.LocalException ex__)
                {
                    og__.abort(ex__);
                }
                bool ok__ = og__.invoke();
                try
                {
                    if(!ok__)
                    {
                        try
                        {
                            og__.throwUserException();
                        }
                        catch(Ice.UserException ex)
                        {
                            throw new Ice.UnknownUserException(ex.ice_name(), ex);
                        }
                    }
                    IceInternal.BasicStream is__ = og__.istr();
                    is__.startReadEncaps();
                    double ret__;
                    ret__ = is__.readDouble();
                    is__.endReadEncaps();
                    return ret__;
                }
                catch(Ice.LocalException ex__)
                {
                    throw new IceInternal.LocalExceptionWrapper(ex__, false);
                }
            }
            finally
            {
                handler__.reclaimOutgoing(og__);
            }
        }

        public string typeName(string id, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            IceInternal.Outgoing og__ = handler__.getOutgoing("typeName", Ice.OperationMode.Idempotent, context__);
            try
            {
                try
                {
                    IceInternal.BasicStream os__ = og__.ostr();
                    os__.writeString(id);
                }
                catch(Ice.LocalException ex__)
                {
                    og__.abort(ex__);
                }
                bool ok__ = og__.invoke();
                try
                {
                    if(!ok__)
                    {
                        try
                        {
                            og__.throwUserException();
                        }
                        catch(Ice.UserException ex)
                        {
                            throw new Ice.UnknownUserException(ex.ice_name(), ex);
                        }
                    }
                    IceInternal.BasicStream is__ = og__.istr();
                    is__.startReadEncaps();
                    string ret__;
                    ret__ = is__.readString();
                    is__.endReadEncaps();
                    return ret__;
                }
                catch(Ice.LocalException ex__)
                {
                    throw new IceInternal.LocalExceptionWrapper(ex__, false);
                }
            }
            finally
            {
                handler__.reclaimOutgoing(og__);
            }
        }
    }
}

namespace RepoIce
{
    public sealed class RepoApiDelD_ : Ice.ObjectDelD_, RepoApiDel_
    {
        public void addChild(string id, string child, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            Ice.Current current__ = new Ice.Current();
            initCurrent__(ref current__, "addChild", Ice.OperationMode.Normal, context__);
            IceInternal.Direct.RunDelegate run__ = delegate(Ice.Object obj__)
            {
                RepoApi servant__ = null;
                try
                {
                    servant__ = (RepoApi)obj__;
                }
                catch(_System.InvalidCastException)
                {
                    throw new Ice.OperationNotExistException(current__.id, current__.facet, current__.operation);
                }
                servant__.addChild(id, child, current__);
                return Ice.DispatchStatus.DispatchOK;
            };
            IceInternal.Direct direct__ = null;
            try
            {
                direct__ = new IceInternal.Direct(current__, run__);
                try
                {
                    Ice.DispatchStatus status__ = direct__.servant().collocDispatch__(direct__);
                    _System.Diagnostics.Debug.Assert(status__ == Ice.DispatchStatus.DispatchOK);
                }
                finally
                {
                    direct__.destroy();
                }
            }
            catch(Ice.SystemException)
            {
                throw;
            }
            catch(System.Exception ex__)
            {
                IceInternal.LocalExceptionWrapper.throwWrapper(ex__);
            }
        }

        public void addParent(string id, string parent, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            Ice.Current current__ = new Ice.Current();
            initCurrent__(ref current__, "addParent", Ice.OperationMode.Normal, context__);
            IceInternal.Direct.RunDelegate run__ = delegate(Ice.Object obj__)
            {
                RepoApi servant__ = null;
                try
                {
                    servant__ = (RepoApi)obj__;
                }
                catch(_System.InvalidCastException)
                {
                    throw new Ice.OperationNotExistException(current__.id, current__.facet, current__.operation);
                }
                servant__.addParent(id, parent, current__);
                return Ice.DispatchStatus.DispatchOK;
            };
            IceInternal.Direct direct__ = null;
            try
            {
                direct__ = new IceInternal.Direct(current__, run__);
                try
                {
                    Ice.DispatchStatus status__ = direct__.servant().collocDispatch__(direct__);
                    _System.Diagnostics.Debug.Assert(status__ == Ice.DispatchStatus.DispatchOK);
                }
                finally
                {
                    direct__.destroy();
                }
            }
            catch(Ice.SystemException)
            {
                throw;
            }
            catch(System.Exception ex__)
            {
                IceInternal.LocalExceptionWrapper.throwWrapper(ex__);
            }
        }

        public string[] children(string id, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            Ice.Current current__ = new Ice.Current();
            initCurrent__(ref current__, "children", Ice.OperationMode.Normal, context__);
            string[] result__ = null;
            IceInternal.Direct.RunDelegate run__ = delegate(Ice.Object obj__)
            {
                RepoApi servant__ = null;
                try
                {
                    servant__ = (RepoApi)obj__;
                }
                catch(_System.InvalidCastException)
                {
                    throw new Ice.OperationNotExistException(current__.id, current__.facet, current__.operation);
                }
                result__ = servant__.children(id, current__);
                return Ice.DispatchStatus.DispatchOK;
            };
            IceInternal.Direct direct__ = null;
            try
            {
                direct__ = new IceInternal.Direct(current__, run__);
                try
                {
                    Ice.DispatchStatus status__ = direct__.servant().collocDispatch__(direct__);
                    _System.Diagnostics.Debug.Assert(status__ == Ice.DispatchStatus.DispatchOK);
                }
                finally
                {
                    direct__.destroy();
                }
            }
            catch(Ice.SystemException)
            {
                throw;
            }
            catch(System.Exception ex__)
            {
                IceInternal.LocalExceptionWrapper.throwWrapper(ex__);
            }
            return result__;
        }

        public void exterminate(_System.Collections.Generic.Dictionary<string, string> context__)
        {
            Ice.Current current__ = new Ice.Current();
            initCurrent__(ref current__, "exterminate", Ice.OperationMode.Normal, context__);
            IceInternal.Direct.RunDelegate run__ = delegate(Ice.Object obj__)
            {
                RepoApi servant__ = null;
                try
                {
                    servant__ = (RepoApi)obj__;
                }
                catch(_System.InvalidCastException)
                {
                    throw new Ice.OperationNotExistException(current__.id, current__.facet, current__.operation);
                }
                servant__.exterminate(current__);
                return Ice.DispatchStatus.DispatchOK;
            };
            IceInternal.Direct direct__ = null;
            try
            {
                direct__ = new IceInternal.Direct(current__, run__);
                try
                {
                    Ice.DispatchStatus status__ = direct__.servant().collocDispatch__(direct__);
                    _System.Diagnostics.Debug.Assert(status__ == Ice.DispatchStatus.DispatchOK);
                }
                finally
                {
                    direct__.destroy();
                }
            }
            catch(Ice.SystemException)
            {
                throw;
            }
            catch(System.Exception ex__)
            {
                IceInternal.LocalExceptionWrapper.throwWrapper(ex__);
            }
        }

        public string from(string id, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            Ice.Current current__ = new Ice.Current();
            initCurrent__(ref current__, "from", Ice.OperationMode.Idempotent, context__);
            string result__ = null;
            IceInternal.Direct.RunDelegate run__ = delegate(Ice.Object obj__)
            {
                RepoApi servant__ = null;
                try
                {
                    servant__ = (RepoApi)obj__;
                }
                catch(_System.InvalidCastException)
                {
                    throw new Ice.OperationNotExistException(current__.id, current__.facet, current__.operation);
                }
                result__ = servant__.from(id, current__);
                return Ice.DispatchStatus.DispatchOK;
            };
            IceInternal.Direct direct__ = null;
            try
            {
                direct__ = new IceInternal.Direct(current__, run__);
                try
                {
                    Ice.DispatchStatus status__ = direct__.servant().collocDispatch__(direct__);
                    _System.Diagnostics.Debug.Assert(status__ == Ice.DispatchStatus.DispatchOK);
                }
                finally
                {
                    direct__.destroy();
                }
            }
            catch(Ice.SystemException)
            {
                throw;
            }
            catch(System.Exception ex__)
            {
                IceInternal.LocalExceptionWrapper.throwWrapper(ex__);
            }
            return result__;
        }

        public double fromPort(string id, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            Ice.Current current__ = new Ice.Current();
            initCurrent__(ref current__, "fromPort", Ice.OperationMode.Idempotent, context__);
            double result__ = 0.0;
            IceInternal.Direct.RunDelegate run__ = delegate(Ice.Object obj__)
            {
                RepoApi servant__ = null;
                try
                {
                    servant__ = (RepoApi)obj__;
                }
                catch(_System.InvalidCastException)
                {
                    throw new Ice.OperationNotExistException(current__.id, current__.facet, current__.operation);
                }
                result__ = servant__.fromPort(id, current__);
                return Ice.DispatchStatus.DispatchOK;
            };
            IceInternal.Direct direct__ = null;
            try
            {
                direct__ = new IceInternal.Direct(current__, run__);
                try
                {
                    Ice.DispatchStatus status__ = direct__.servant().collocDispatch__(direct__);
                    _System.Diagnostics.Debug.Assert(status__ == Ice.DispatchStatus.DispatchOK);
                }
                finally
                {
                    direct__.destroy();
                }
            }
            catch(Ice.SystemException)
            {
                throw;
            }
            catch(System.Exception ex__)
            {
                IceInternal.LocalExceptionWrapper.throwWrapper(ex__);
            }
            return result__;
        }

        public bool hasProperty(string id, string propertyName, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            Ice.Current current__ = new Ice.Current();
            initCurrent__(ref current__, "hasProperty", Ice.OperationMode.Idempotent, context__);
            bool result__ = false;
            IceInternal.Direct.RunDelegate run__ = delegate(Ice.Object obj__)
            {
                RepoApi servant__ = null;
                try
                {
                    servant__ = (RepoApi)obj__;
                }
                catch(_System.InvalidCastException)
                {
                    throw new Ice.OperationNotExistException(current__.id, current__.facet, current__.operation);
                }
                result__ = servant__.hasProperty(id, propertyName, current__);
                return Ice.DispatchStatus.DispatchOK;
            };
            IceInternal.Direct direct__ = null;
            try
            {
                direct__ = new IceInternal.Direct(current__, run__);
                try
                {
                    Ice.DispatchStatus status__ = direct__.servant().collocDispatch__(direct__);
                    _System.Diagnostics.Debug.Assert(status__ == Ice.DispatchStatus.DispatchOK);
                }
                finally
                {
                    direct__.destroy();
                }
            }
            catch(Ice.SystemException)
            {
                throw;
            }
            catch(System.Exception ex__)
            {
                IceInternal.LocalExceptionWrapper.throwWrapper(ex__);
            }
            return result__;
        }

        public string[] incomingLinks(string id, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            Ice.Current current__ = new Ice.Current();
            initCurrent__(ref current__, "incomingLinks", Ice.OperationMode.Idempotent, context__);
            string[] result__ = null;
            IceInternal.Direct.RunDelegate run__ = delegate(Ice.Object obj__)
            {
                RepoApi servant__ = null;
                try
                {
                    servant__ = (RepoApi)obj__;
                }
                catch(_System.InvalidCastException)
                {
                    throw new Ice.OperationNotExistException(current__.id, current__.facet, current__.operation);
                }
                result__ = servant__.incomingLinks(id, current__);
                return Ice.DispatchStatus.DispatchOK;
            };
            IceInternal.Direct direct__ = null;
            try
            {
                direct__ = new IceInternal.Direct(current__, run__);
                try
                {
                    Ice.DispatchStatus status__ = direct__.servant().collocDispatch__(direct__);
                    _System.Diagnostics.Debug.Assert(status__ == Ice.DispatchStatus.DispatchOK);
                }
                finally
                {
                    direct__.destroy();
                }
            }
            catch(Ice.SystemException)
            {
                throw;
            }
            catch(System.Exception ex__)
            {
                IceInternal.LocalExceptionWrapper.throwWrapper(ex__);
            }
            return result__;
        }

        public string[] links(string id, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            Ice.Current current__ = new Ice.Current();
            initCurrent__(ref current__, "links", Ice.OperationMode.Idempotent, context__);
            string[] result__ = null;
            IceInternal.Direct.RunDelegate run__ = delegate(Ice.Object obj__)
            {
                RepoApi servant__ = null;
                try
                {
                    servant__ = (RepoApi)obj__;
                }
                catch(_System.InvalidCastException)
                {
                    throw new Ice.OperationNotExistException(current__.id, current__.facet, current__.operation);
                }
                result__ = servant__.links(id, current__);
                return Ice.DispatchStatus.DispatchOK;
            };
            IceInternal.Direct direct__ = null;
            try
            {
                direct__ = new IceInternal.Direct(current__, run__);
                try
                {
                    Ice.DispatchStatus status__ = direct__.servant().collocDispatch__(direct__);
                    _System.Diagnostics.Debug.Assert(status__ == Ice.DispatchStatus.DispatchOK);
                }
                finally
                {
                    direct__.destroy();
                }
            }
            catch(Ice.SystemException)
            {
                throw;
            }
            catch(System.Exception ex__)
            {
                IceInternal.LocalExceptionWrapper.throwWrapper(ex__);
            }
            return result__;
        }

        public string name(string id, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            Ice.Current current__ = new Ice.Current();
            initCurrent__(ref current__, "name", Ice.OperationMode.Idempotent, context__);
            string result__ = null;
            IceInternal.Direct.RunDelegate run__ = delegate(Ice.Object obj__)
            {
                RepoApi servant__ = null;
                try
                {
                    servant__ = (RepoApi)obj__;
                }
                catch(_System.InvalidCastException)
                {
                    throw new Ice.OperationNotExistException(current__.id, current__.facet, current__.operation);
                }
                result__ = servant__.name(id, current__);
                return Ice.DispatchStatus.DispatchOK;
            };
            IceInternal.Direct direct__ = null;
            try
            {
                direct__ = new IceInternal.Direct(current__, run__);
                try
                {
                    Ice.DispatchStatus status__ = direct__.servant().collocDispatch__(direct__);
                    _System.Diagnostics.Debug.Assert(status__ == Ice.DispatchStatus.DispatchOK);
                }
                finally
                {
                    direct__.destroy();
                }
            }
            catch(Ice.SystemException)
            {
                throw;
            }
            catch(System.Exception ex__)
            {
                IceInternal.LocalExceptionWrapper.throwWrapper(ex__);
            }
            return result__;
        }

        public string otherEntityFromLink(string linkId, string firstNode, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            Ice.Current current__ = new Ice.Current();
            initCurrent__(ref current__, "otherEntityFromLink", Ice.OperationMode.Idempotent, context__);
            string result__ = null;
            IceInternal.Direct.RunDelegate run__ = delegate(Ice.Object obj__)
            {
                RepoApi servant__ = null;
                try
                {
                    servant__ = (RepoApi)obj__;
                }
                catch(_System.InvalidCastException)
                {
                    throw new Ice.OperationNotExistException(current__.id, current__.facet, current__.operation);
                }
                result__ = servant__.otherEntityFromLink(linkId, firstNode, current__);
                return Ice.DispatchStatus.DispatchOK;
            };
            IceInternal.Direct direct__ = null;
            try
            {
                direct__ = new IceInternal.Direct(current__, run__);
                try
                {
                    Ice.DispatchStatus status__ = direct__.servant().collocDispatch__(direct__);
                    _System.Diagnostics.Debug.Assert(status__ == Ice.DispatchStatus.DispatchOK);
                }
                finally
                {
                    direct__.destroy();
                }
            }
            catch(Ice.SystemException)
            {
                throw;
            }
            catch(System.Exception ex__)
            {
                IceInternal.LocalExceptionWrapper.throwWrapper(ex__);
            }
            return result__;
        }

        public string[] outcomingLinks(string id, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            Ice.Current current__ = new Ice.Current();
            initCurrent__(ref current__, "outcomingLinks", Ice.OperationMode.Idempotent, context__);
            string[] result__ = null;
            IceInternal.Direct.RunDelegate run__ = delegate(Ice.Object obj__)
            {
                RepoApi servant__ = null;
                try
                {
                    servant__ = (RepoApi)obj__;
                }
                catch(_System.InvalidCastException)
                {
                    throw new Ice.OperationNotExistException(current__.id, current__.facet, current__.operation);
                }
                result__ = servant__.outcomingLinks(id, current__);
                return Ice.DispatchStatus.DispatchOK;
            };
            IceInternal.Direct direct__ = null;
            try
            {
                direct__ = new IceInternal.Direct(current__, run__);
                try
                {
                    Ice.DispatchStatus status__ = direct__.servant().collocDispatch__(direct__);
                    _System.Diagnostics.Debug.Assert(status__ == Ice.DispatchStatus.DispatchOK);
                }
                finally
                {
                    direct__.destroy();
                }
            }
            catch(Ice.SystemException)
            {
                throw;
            }
            catch(System.Exception ex__)
            {
                IceInternal.LocalExceptionWrapper.throwWrapper(ex__);
            }
            return result__;
        }

        public string[] parents(string id, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            Ice.Current current__ = new Ice.Current();
            initCurrent__(ref current__, "parents", Ice.OperationMode.Idempotent, context__);
            string[] result__ = null;
            IceInternal.Direct.RunDelegate run__ = delegate(Ice.Object obj__)
            {
                RepoApi servant__ = null;
                try
                {
                    servant__ = (RepoApi)obj__;
                }
                catch(_System.InvalidCastException)
                {
                    throw new Ice.OperationNotExistException(current__.id, current__.facet, current__.operation);
                }
                result__ = servant__.parents(id, current__);
                return Ice.DispatchStatus.DispatchOK;
            };
            IceInternal.Direct direct__ = null;
            try
            {
                direct__ = new IceInternal.Direct(current__, run__);
                try
                {
                    Ice.DispatchStatus status__ = direct__.servant().collocDispatch__(direct__);
                    _System.Diagnostics.Debug.Assert(status__ == Ice.DispatchStatus.DispatchOK);
                }
                finally
                {
                    direct__.destroy();
                }
            }
            catch(Ice.SystemException)
            {
                throw;
            }
            catch(System.Exception ex__)
            {
                IceInternal.LocalExceptionWrapper.throwWrapper(ex__);
            }
            return result__;
        }

        public string property(string id, string propertyName, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            Ice.Current current__ = new Ice.Current();
            initCurrent__(ref current__, "property", Ice.OperationMode.Idempotent, context__);
            string result__ = null;
            IceInternal.Direct.RunDelegate run__ = delegate(Ice.Object obj__)
            {
                RepoApi servant__ = null;
                try
                {
                    servant__ = (RepoApi)obj__;
                }
                catch(_System.InvalidCastException)
                {
                    throw new Ice.OperationNotExistException(current__.id, current__.facet, current__.operation);
                }
                result__ = servant__.property(id, propertyName, current__);
                return Ice.DispatchStatus.DispatchOK;
            };
            IceInternal.Direct direct__ = null;
            try
            {
                direct__ = new IceInternal.Direct(current__, run__);
                try
                {
                    Ice.DispatchStatus status__ = direct__.servant().collocDispatch__(direct__);
                    _System.Diagnostics.Debug.Assert(status__ == Ice.DispatchStatus.DispatchOK);
                }
                finally
                {
                    direct__.destroy();
                }
            }
            catch(Ice.SystemException)
            {
                throw;
            }
            catch(System.Exception ex__)
            {
                IceInternal.LocalExceptionWrapper.throwWrapper(ex__);
            }
            return result__;
        }

        public void removeChild(string id, string child, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            Ice.Current current__ = new Ice.Current();
            initCurrent__(ref current__, "removeChild", Ice.OperationMode.Normal, context__);
            IceInternal.Direct.RunDelegate run__ = delegate(Ice.Object obj__)
            {
                RepoApi servant__ = null;
                try
                {
                    servant__ = (RepoApi)obj__;
                }
                catch(_System.InvalidCastException)
                {
                    throw new Ice.OperationNotExistException(current__.id, current__.facet, current__.operation);
                }
                servant__.removeChild(id, child, current__);
                return Ice.DispatchStatus.DispatchOK;
            };
            IceInternal.Direct direct__ = null;
            try
            {
                direct__ = new IceInternal.Direct(current__, run__);
                try
                {
                    Ice.DispatchStatus status__ = direct__.servant().collocDispatch__(direct__);
                    _System.Diagnostics.Debug.Assert(status__ == Ice.DispatchStatus.DispatchOK);
                }
                finally
                {
                    direct__.destroy();
                }
            }
            catch(Ice.SystemException)
            {
                throw;
            }
            catch(System.Exception ex__)
            {
                IceInternal.LocalExceptionWrapper.throwWrapper(ex__);
            }
        }

        public void removeElement(string id, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            Ice.Current current__ = new Ice.Current();
            initCurrent__(ref current__, "removeElement", Ice.OperationMode.Normal, context__);
            IceInternal.Direct.RunDelegate run__ = delegate(Ice.Object obj__)
            {
                RepoApi servant__ = null;
                try
                {
                    servant__ = (RepoApi)obj__;
                }
                catch(_System.InvalidCastException)
                {
                    throw new Ice.OperationNotExistException(current__.id, current__.facet, current__.operation);
                }
                servant__.removeElement(id, current__);
                return Ice.DispatchStatus.DispatchOK;
            };
            IceInternal.Direct direct__ = null;
            try
            {
                direct__ = new IceInternal.Direct(current__, run__);
                try
                {
                    Ice.DispatchStatus status__ = direct__.servant().collocDispatch__(direct__);
                    _System.Diagnostics.Debug.Assert(status__ == Ice.DispatchStatus.DispatchOK);
                }
                finally
                {
                    direct__.destroy();
                }
            }
            catch(Ice.SystemException)
            {
                throw;
            }
            catch(System.Exception ex__)
            {
                IceInternal.LocalExceptionWrapper.throwWrapper(ex__);
            }
        }

        public void removeParent(string id, string parent, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            Ice.Current current__ = new Ice.Current();
            initCurrent__(ref current__, "removeParent", Ice.OperationMode.Normal, context__);
            IceInternal.Direct.RunDelegate run__ = delegate(Ice.Object obj__)
            {
                RepoApi servant__ = null;
                try
                {
                    servant__ = (RepoApi)obj__;
                }
                catch(_System.InvalidCastException)
                {
                    throw new Ice.OperationNotExistException(current__.id, current__.facet, current__.operation);
                }
                servant__.removeParent(id, parent, current__);
                return Ice.DispatchStatus.DispatchOK;
            };
            IceInternal.Direct direct__ = null;
            try
            {
                direct__ = new IceInternal.Direct(current__, run__);
                try
                {
                    Ice.DispatchStatus status__ = direct__.servant().collocDispatch__(direct__);
                    _System.Diagnostics.Debug.Assert(status__ == Ice.DispatchStatus.DispatchOK);
                }
                finally
                {
                    direct__.destroy();
                }
            }
            catch(Ice.SystemException)
            {
                throw;
            }
            catch(System.Exception ex__)
            {
                IceInternal.LocalExceptionWrapper.throwWrapper(ex__);
            }
        }

        public void removeProperty(string id, string propertyName, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            Ice.Current current__ = new Ice.Current();
            initCurrent__(ref current__, "removeProperty", Ice.OperationMode.Normal, context__);
            IceInternal.Direct.RunDelegate run__ = delegate(Ice.Object obj__)
            {
                RepoApi servant__ = null;
                try
                {
                    servant__ = (RepoApi)obj__;
                }
                catch(_System.InvalidCastException)
                {
                    throw new Ice.OperationNotExistException(current__.id, current__.facet, current__.operation);
                }
                servant__.removeProperty(id, propertyName, current__);
                return Ice.DispatchStatus.DispatchOK;
            };
            IceInternal.Direct direct__ = null;
            try
            {
                direct__ = new IceInternal.Direct(current__, run__);
                try
                {
                    Ice.DispatchStatus status__ = direct__.servant().collocDispatch__(direct__);
                    _System.Diagnostics.Debug.Assert(status__ == Ice.DispatchStatus.DispatchOK);
                }
                finally
                {
                    direct__.destroy();
                }
            }
            catch(Ice.SystemException)
            {
                throw;
            }
            catch(System.Exception ex__)
            {
                IceInternal.LocalExceptionWrapper.throwWrapper(ex__);
            }
        }

        public void save(_System.Collections.Generic.Dictionary<string, string> context__)
        {
            Ice.Current current__ = new Ice.Current();
            initCurrent__(ref current__, "save", Ice.OperationMode.Normal, context__);
            IceInternal.Direct.RunDelegate run__ = delegate(Ice.Object obj__)
            {
                RepoApi servant__ = null;
                try
                {
                    servant__ = (RepoApi)obj__;
                }
                catch(_System.InvalidCastException)
                {
                    throw new Ice.OperationNotExistException(current__.id, current__.facet, current__.operation);
                }
                servant__.save(current__);
                return Ice.DispatchStatus.DispatchOK;
            };
            IceInternal.Direct direct__ = null;
            try
            {
                direct__ = new IceInternal.Direct(current__, run__);
                try
                {
                    Ice.DispatchStatus status__ = direct__.servant().collocDispatch__(direct__);
                    _System.Diagnostics.Debug.Assert(status__ == Ice.DispatchStatus.DispatchOK);
                }
                finally
                {
                    direct__.destroy();
                }
            }
            catch(Ice.SystemException)
            {
                throw;
            }
            catch(System.Exception ex__)
            {
                IceInternal.LocalExceptionWrapper.throwWrapper(ex__);
            }
        }

        public void setFrom(string id, string from, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            Ice.Current current__ = new Ice.Current();
            initCurrent__(ref current__, "setFrom", Ice.OperationMode.Normal, context__);
            IceInternal.Direct.RunDelegate run__ = delegate(Ice.Object obj__)
            {
                RepoApi servant__ = null;
                try
                {
                    servant__ = (RepoApi)obj__;
                }
                catch(_System.InvalidCastException)
                {
                    throw new Ice.OperationNotExistException(current__.id, current__.facet, current__.operation);
                }
                servant__.setFrom(id, from, current__);
                return Ice.DispatchStatus.DispatchOK;
            };
            IceInternal.Direct direct__ = null;
            try
            {
                direct__ = new IceInternal.Direct(current__, run__);
                try
                {
                    Ice.DispatchStatus status__ = direct__.servant().collocDispatch__(direct__);
                    _System.Diagnostics.Debug.Assert(status__ == Ice.DispatchStatus.DispatchOK);
                }
                finally
                {
                    direct__.destroy();
                }
            }
            catch(Ice.SystemException)
            {
                throw;
            }
            catch(System.Exception ex__)
            {
                IceInternal.LocalExceptionWrapper.throwWrapper(ex__);
            }
        }

        public void setFromPort(string id, double fromPort, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            Ice.Current current__ = new Ice.Current();
            initCurrent__(ref current__, "setFromPort", Ice.OperationMode.Normal, context__);
            IceInternal.Direct.RunDelegate run__ = delegate(Ice.Object obj__)
            {
                RepoApi servant__ = null;
                try
                {
                    servant__ = (RepoApi)obj__;
                }
                catch(_System.InvalidCastException)
                {
                    throw new Ice.OperationNotExistException(current__.id, current__.facet, current__.operation);
                }
                servant__.setFromPort(id, fromPort, current__);
                return Ice.DispatchStatus.DispatchOK;
            };
            IceInternal.Direct direct__ = null;
            try
            {
                direct__ = new IceInternal.Direct(current__, run__);
                try
                {
                    Ice.DispatchStatus status__ = direct__.servant().collocDispatch__(direct__);
                    _System.Diagnostics.Debug.Assert(status__ == Ice.DispatchStatus.DispatchOK);
                }
                finally
                {
                    direct__.destroy();
                }
            }
            catch(Ice.SystemException)
            {
                throw;
            }
            catch(System.Exception ex__)
            {
                IceInternal.LocalExceptionWrapper.throwWrapper(ex__);
            }
        }

        public void setName(string id, string name, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            Ice.Current current__ = new Ice.Current();
            initCurrent__(ref current__, "setName", Ice.OperationMode.Normal, context__);
            IceInternal.Direct.RunDelegate run__ = delegate(Ice.Object obj__)
            {
                RepoApi servant__ = null;
                try
                {
                    servant__ = (RepoApi)obj__;
                }
                catch(_System.InvalidCastException)
                {
                    throw new Ice.OperationNotExistException(current__.id, current__.facet, current__.operation);
                }
                servant__.setName(id, name, current__);
                return Ice.DispatchStatus.DispatchOK;
            };
            IceInternal.Direct direct__ = null;
            try
            {
                direct__ = new IceInternal.Direct(current__, run__);
                try
                {
                    Ice.DispatchStatus status__ = direct__.servant().collocDispatch__(direct__);
                    _System.Diagnostics.Debug.Assert(status__ == Ice.DispatchStatus.DispatchOK);
                }
                finally
                {
                    direct__.destroy();
                }
            }
            catch(Ice.SystemException)
            {
                throw;
            }
            catch(System.Exception ex__)
            {
                IceInternal.LocalExceptionWrapper.throwWrapper(ex__);
            }
        }

        public void setProperty(string id, string propertyName, string value, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            Ice.Current current__ = new Ice.Current();
            initCurrent__(ref current__, "setProperty", Ice.OperationMode.Normal, context__);
            IceInternal.Direct.RunDelegate run__ = delegate(Ice.Object obj__)
            {
                RepoApi servant__ = null;
                try
                {
                    servant__ = (RepoApi)obj__;
                }
                catch(_System.InvalidCastException)
                {
                    throw new Ice.OperationNotExistException(current__.id, current__.facet, current__.operation);
                }
                servant__.setProperty(id, propertyName, value, current__);
                return Ice.DispatchStatus.DispatchOK;
            };
            IceInternal.Direct direct__ = null;
            try
            {
                direct__ = new IceInternal.Direct(current__, run__);
                try
                {
                    Ice.DispatchStatus status__ = direct__.servant().collocDispatch__(direct__);
                    _System.Diagnostics.Debug.Assert(status__ == Ice.DispatchStatus.DispatchOK);
                }
                finally
                {
                    direct__.destroy();
                }
            }
            catch(Ice.SystemException)
            {
                throw;
            }
            catch(System.Exception ex__)
            {
                IceInternal.LocalExceptionWrapper.throwWrapper(ex__);
            }
        }

        public void setTo(string id, string to, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            Ice.Current current__ = new Ice.Current();
            initCurrent__(ref current__, "setTo", Ice.OperationMode.Normal, context__);
            IceInternal.Direct.RunDelegate run__ = delegate(Ice.Object obj__)
            {
                RepoApi servant__ = null;
                try
                {
                    servant__ = (RepoApi)obj__;
                }
                catch(_System.InvalidCastException)
                {
                    throw new Ice.OperationNotExistException(current__.id, current__.facet, current__.operation);
                }
                servant__.setTo(id, to, current__);
                return Ice.DispatchStatus.DispatchOK;
            };
            IceInternal.Direct direct__ = null;
            try
            {
                direct__ = new IceInternal.Direct(current__, run__);
                try
                {
                    Ice.DispatchStatus status__ = direct__.servant().collocDispatch__(direct__);
                    _System.Diagnostics.Debug.Assert(status__ == Ice.DispatchStatus.DispatchOK);
                }
                finally
                {
                    direct__.destroy();
                }
            }
            catch(Ice.SystemException)
            {
                throw;
            }
            catch(System.Exception ex__)
            {
                IceInternal.LocalExceptionWrapper.throwWrapper(ex__);
            }
        }

        public void setToPort(string id, double toPort, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            Ice.Current current__ = new Ice.Current();
            initCurrent__(ref current__, "setToPort", Ice.OperationMode.Normal, context__);
            IceInternal.Direct.RunDelegate run__ = delegate(Ice.Object obj__)
            {
                RepoApi servant__ = null;
                try
                {
                    servant__ = (RepoApi)obj__;
                }
                catch(_System.InvalidCastException)
                {
                    throw new Ice.OperationNotExistException(current__.id, current__.facet, current__.operation);
                }
                servant__.setToPort(id, toPort, current__);
                return Ice.DispatchStatus.DispatchOK;
            };
            IceInternal.Direct direct__ = null;
            try
            {
                direct__ = new IceInternal.Direct(current__, run__);
                try
                {
                    Ice.DispatchStatus status__ = direct__.servant().collocDispatch__(direct__);
                    _System.Diagnostics.Debug.Assert(status__ == Ice.DispatchStatus.DispatchOK);
                }
                finally
                {
                    direct__.destroy();
                }
            }
            catch(Ice.SystemException)
            {
                throw;
            }
            catch(System.Exception ex__)
            {
                IceInternal.LocalExceptionWrapper.throwWrapper(ex__);
            }
        }

        public string stringProperty(string id, string propertyName, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            Ice.Current current__ = new Ice.Current();
            initCurrent__(ref current__, "stringProperty", Ice.OperationMode.Idempotent, context__);
            string result__ = null;
            IceInternal.Direct.RunDelegate run__ = delegate(Ice.Object obj__)
            {
                RepoApi servant__ = null;
                try
                {
                    servant__ = (RepoApi)obj__;
                }
                catch(_System.InvalidCastException)
                {
                    throw new Ice.OperationNotExistException(current__.id, current__.facet, current__.operation);
                }
                result__ = servant__.stringProperty(id, propertyName, current__);
                return Ice.DispatchStatus.DispatchOK;
            };
            IceInternal.Direct direct__ = null;
            try
            {
                direct__ = new IceInternal.Direct(current__, run__);
                try
                {
                    Ice.DispatchStatus status__ = direct__.servant().collocDispatch__(direct__);
                    _System.Diagnostics.Debug.Assert(status__ == Ice.DispatchStatus.DispatchOK);
                }
                finally
                {
                    direct__.destroy();
                }
            }
            catch(Ice.SystemException)
            {
                throw;
            }
            catch(System.Exception ex__)
            {
                IceInternal.LocalExceptionWrapper.throwWrapper(ex__);
            }
            return result__;
        }

        public string to(string id, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            Ice.Current current__ = new Ice.Current();
            initCurrent__(ref current__, "to", Ice.OperationMode.Idempotent, context__);
            string result__ = null;
            IceInternal.Direct.RunDelegate run__ = delegate(Ice.Object obj__)
            {
                RepoApi servant__ = null;
                try
                {
                    servant__ = (RepoApi)obj__;
                }
                catch(_System.InvalidCastException)
                {
                    throw new Ice.OperationNotExistException(current__.id, current__.facet, current__.operation);
                }
                result__ = servant__.to(id, current__);
                return Ice.DispatchStatus.DispatchOK;
            };
            IceInternal.Direct direct__ = null;
            try
            {
                direct__ = new IceInternal.Direct(current__, run__);
                try
                {
                    Ice.DispatchStatus status__ = direct__.servant().collocDispatch__(direct__);
                    _System.Diagnostics.Debug.Assert(status__ == Ice.DispatchStatus.DispatchOK);
                }
                finally
                {
                    direct__.destroy();
                }
            }
            catch(Ice.SystemException)
            {
                throw;
            }
            catch(System.Exception ex__)
            {
                IceInternal.LocalExceptionWrapper.throwWrapper(ex__);
            }
            return result__;
        }

        public double toPort(string id, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            Ice.Current current__ = new Ice.Current();
            initCurrent__(ref current__, "toPort", Ice.OperationMode.Idempotent, context__);
            double result__ = 0.0;
            IceInternal.Direct.RunDelegate run__ = delegate(Ice.Object obj__)
            {
                RepoApi servant__ = null;
                try
                {
                    servant__ = (RepoApi)obj__;
                }
                catch(_System.InvalidCastException)
                {
                    throw new Ice.OperationNotExistException(current__.id, current__.facet, current__.operation);
                }
                result__ = servant__.toPort(id, current__);
                return Ice.DispatchStatus.DispatchOK;
            };
            IceInternal.Direct direct__ = null;
            try
            {
                direct__ = new IceInternal.Direct(current__, run__);
                try
                {
                    Ice.DispatchStatus status__ = direct__.servant().collocDispatch__(direct__);
                    _System.Diagnostics.Debug.Assert(status__ == Ice.DispatchStatus.DispatchOK);
                }
                finally
                {
                    direct__.destroy();
                }
            }
            catch(Ice.SystemException)
            {
                throw;
            }
            catch(System.Exception ex__)
            {
                IceInternal.LocalExceptionWrapper.throwWrapper(ex__);
            }
            return result__;
        }

        public string typeName(string id, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            Ice.Current current__ = new Ice.Current();
            initCurrent__(ref current__, "typeName", Ice.OperationMode.Idempotent, context__);
            string result__ = null;
            IceInternal.Direct.RunDelegate run__ = delegate(Ice.Object obj__)
            {
                RepoApi servant__ = null;
                try
                {
                    servant__ = (RepoApi)obj__;
                }
                catch(_System.InvalidCastException)
                {
                    throw new Ice.OperationNotExistException(current__.id, current__.facet, current__.operation);
                }
                result__ = servant__.typeName(id, current__);
                return Ice.DispatchStatus.DispatchOK;
            };
            IceInternal.Direct direct__ = null;
            try
            {
                direct__ = new IceInternal.Direct(current__, run__);
                try
                {
                    Ice.DispatchStatus status__ = direct__.servant().collocDispatch__(direct__);
                    _System.Diagnostics.Debug.Assert(status__ == Ice.DispatchStatus.DispatchOK);
                }
                finally
                {
                    direct__.destroy();
                }
            }
            catch(Ice.SystemException)
            {
                throw;
            }
            catch(System.Exception ex__)
            {
                IceInternal.LocalExceptionWrapper.throwWrapper(ex__);
            }
            return result__;
        }
    }
}

namespace RepoIce
{
    public abstract class RepoApiDisp_ : Ice.ObjectImpl, RepoApi
    {
        #region Slice operations

        public string name(string id)
        {
            return name(id, Ice.ObjectImpl.defaultCurrent);
        }

        public abstract string name(string id, Ice.Current current__);

        public void setName(string id, string name)
        {
            setName(id, name, Ice.ObjectImpl.defaultCurrent);
        }

        public abstract void setName(string id, string name, Ice.Current current__);

        public string[] children(string id)
        {
            return children(id, Ice.ObjectImpl.defaultCurrent);
        }

        public abstract string[] children(string id, Ice.Current current__);

        public void addChild(string id, string child)
        {
            addChild(id, child, Ice.ObjectImpl.defaultCurrent);
        }

        public abstract void addChild(string id, string child, Ice.Current current__);

        public void removeChild(string id, string child)
        {
            removeChild(id, child, Ice.ObjectImpl.defaultCurrent);
        }

        public abstract void removeChild(string id, string child, Ice.Current current__);

        public void removeElement(string id)
        {
            removeElement(id, Ice.ObjectImpl.defaultCurrent);
        }

        public abstract void removeElement(string id, Ice.Current current__);

        public string[] parents(string id)
        {
            return parents(id, Ice.ObjectImpl.defaultCurrent);
        }

        public abstract string[] parents(string id, Ice.Current current__);

        public void addParent(string id, string parent)
        {
            addParent(id, parent, Ice.ObjectImpl.defaultCurrent);
        }

        public abstract void addParent(string id, string parent, Ice.Current current__);

        public void removeParent(string id, string parent)
        {
            removeParent(id, parent, Ice.ObjectImpl.defaultCurrent);
        }

        public abstract void removeParent(string id, string parent, Ice.Current current__);

        public string[] outcomingLinks(string id)
        {
            return outcomingLinks(id, Ice.ObjectImpl.defaultCurrent);
        }

        public abstract string[] outcomingLinks(string id, Ice.Current current__);

        public string[] incomingLinks(string id)
        {
            return incomingLinks(id, Ice.ObjectImpl.defaultCurrent);
        }

        public abstract string[] incomingLinks(string id, Ice.Current current__);

        public string[] links(string id)
        {
            return links(id, Ice.ObjectImpl.defaultCurrent);
        }

        public abstract string[] links(string id, Ice.Current current__);

        public string typeName(string id)
        {
            return typeName(id, Ice.ObjectImpl.defaultCurrent);
        }

        public abstract string typeName(string id, Ice.Current current__);

        public string property(string id, string propertyName)
        {
            return property(id, propertyName, Ice.ObjectImpl.defaultCurrent);
        }

        public abstract string property(string id, string propertyName, Ice.Current current__);

        public string stringProperty(string id, string propertyName)
        {
            return stringProperty(id, propertyName, Ice.ObjectImpl.defaultCurrent);
        }

        public abstract string stringProperty(string id, string propertyName, Ice.Current current__);

        public void setProperty(string id, string propertyName, string value)
        {
            setProperty(id, propertyName, value, Ice.ObjectImpl.defaultCurrent);
        }

        public abstract void setProperty(string id, string propertyName, string value, Ice.Current current__);

        public void removeProperty(string id, string propertyName)
        {
            removeProperty(id, propertyName, Ice.ObjectImpl.defaultCurrent);
        }

        public abstract void removeProperty(string id, string propertyName, Ice.Current current__);

        public bool hasProperty(string id, string propertyName)
        {
            return hasProperty(id, propertyName, Ice.ObjectImpl.defaultCurrent);
        }

        public abstract bool hasProperty(string id, string propertyName, Ice.Current current__);

        public string from(string id)
        {
            return from(id, Ice.ObjectImpl.defaultCurrent);
        }

        public abstract string from(string id, Ice.Current current__);

        public void setFrom(string id, string from)
        {
            setFrom(id, from, Ice.ObjectImpl.defaultCurrent);
        }

        public abstract void setFrom(string id, string from, Ice.Current current__);

        public string to(string id)
        {
            return to(id, Ice.ObjectImpl.defaultCurrent);
        }

        public abstract string to(string id, Ice.Current current__);

        public void setTo(string id, string to)
        {
            setTo(id, to, Ice.ObjectImpl.defaultCurrent);
        }

        public abstract void setTo(string id, string to, Ice.Current current__);

        public double fromPort(string id)
        {
            return fromPort(id, Ice.ObjectImpl.defaultCurrent);
        }

        public abstract double fromPort(string id, Ice.Current current__);

        public void setFromPort(string id, double fromPort)
        {
            setFromPort(id, fromPort, Ice.ObjectImpl.defaultCurrent);
        }

        public abstract void setFromPort(string id, double fromPort, Ice.Current current__);

        public double toPort(string id)
        {
            return toPort(id, Ice.ObjectImpl.defaultCurrent);
        }

        public abstract double toPort(string id, Ice.Current current__);

        public void setToPort(string id, double toPort)
        {
            setToPort(id, toPort, Ice.ObjectImpl.defaultCurrent);
        }

        public abstract void setToPort(string id, double toPort, Ice.Current current__);

        public string otherEntityFromLink(string linkId, string firstNode)
        {
            return otherEntityFromLink(linkId, firstNode, Ice.ObjectImpl.defaultCurrent);
        }

        public abstract string otherEntityFromLink(string linkId, string firstNode, Ice.Current current__);

        public void exterminate()
        {
            exterminate(Ice.ObjectImpl.defaultCurrent);
        }

        public abstract void exterminate(Ice.Current current__);

        public void save()
        {
            save(Ice.ObjectImpl.defaultCurrent);
        }

        public abstract void save(Ice.Current current__);

        #endregion

        #region Slice type-related members

        public static new string[] ids__ = 
        {
            "::Ice::Object",
            "::RepoIce::RepoApi"
        };

        public override bool ice_isA(string s)
        {
            return _System.Array.BinarySearch(ids__, s, IceUtilInternal.StringUtil.OrdinalStringComparer) >= 0;
        }

        public override bool ice_isA(string s, Ice.Current current__)
        {
            return _System.Array.BinarySearch(ids__, s, IceUtilInternal.StringUtil.OrdinalStringComparer) >= 0;
        }

        public override string[] ice_ids()
        {
            return ids__;
        }

        public override string[] ice_ids(Ice.Current current__)
        {
            return ids__;
        }

        public override string ice_id()
        {
            return ids__[1];
        }

        public override string ice_id(Ice.Current current__)
        {
            return ids__[1];
        }

        public static new string ice_staticId()
        {
            return ids__[1];
        }

        #endregion

        #region Operation dispatch

        public static Ice.DispatchStatus name___(RepoApi obj__, IceInternal.Incoming inS__, Ice.Current current__)
        {
            checkMode__(Ice.OperationMode.Idempotent, current__.mode);
            IceInternal.BasicStream is__ = inS__.istr();
            is__.startReadEncaps();
            string id;
            id = is__.readString();
            is__.endReadEncaps();
            IceInternal.BasicStream os__ = inS__.ostr();
            string ret__ = obj__.name(id, current__);
            os__.writeString(ret__);
            return Ice.DispatchStatus.DispatchOK;
        }

        public static Ice.DispatchStatus setName___(RepoApi obj__, IceInternal.Incoming inS__, Ice.Current current__)
        {
            checkMode__(Ice.OperationMode.Normal, current__.mode);
            IceInternal.BasicStream is__ = inS__.istr();
            is__.startReadEncaps();
            string id;
            id = is__.readString();
            string name;
            name = is__.readString();
            is__.endReadEncaps();
            obj__.setName(id, name, current__);
            return Ice.DispatchStatus.DispatchOK;
        }

        public static Ice.DispatchStatus children___(RepoApi obj__, IceInternal.Incoming inS__, Ice.Current current__)
        {
            checkMode__(Ice.OperationMode.Normal, current__.mode);
            IceInternal.BasicStream is__ = inS__.istr();
            is__.startReadEncaps();
            string id;
            id = is__.readString();
            is__.endReadEncaps();
            IceInternal.BasicStream os__ = inS__.ostr();
            string[] ret__ = obj__.children(id, current__);
            os__.writeStringSeq(ret__);
            return Ice.DispatchStatus.DispatchOK;
        }

        public static Ice.DispatchStatus addChild___(RepoApi obj__, IceInternal.Incoming inS__, Ice.Current current__)
        {
            checkMode__(Ice.OperationMode.Normal, current__.mode);
            IceInternal.BasicStream is__ = inS__.istr();
            is__.startReadEncaps();
            string id;
            id = is__.readString();
            string child;
            child = is__.readString();
            is__.endReadEncaps();
            obj__.addChild(id, child, current__);
            return Ice.DispatchStatus.DispatchOK;
        }

        public static Ice.DispatchStatus removeChild___(RepoApi obj__, IceInternal.Incoming inS__, Ice.Current current__)
        {
            checkMode__(Ice.OperationMode.Normal, current__.mode);
            IceInternal.BasicStream is__ = inS__.istr();
            is__.startReadEncaps();
            string id;
            id = is__.readString();
            string child;
            child = is__.readString();
            is__.endReadEncaps();
            obj__.removeChild(id, child, current__);
            return Ice.DispatchStatus.DispatchOK;
        }

        public static Ice.DispatchStatus removeElement___(RepoApi obj__, IceInternal.Incoming inS__, Ice.Current current__)
        {
            checkMode__(Ice.OperationMode.Normal, current__.mode);
            IceInternal.BasicStream is__ = inS__.istr();
            is__.startReadEncaps();
            string id;
            id = is__.readString();
            is__.endReadEncaps();
            obj__.removeElement(id, current__);
            return Ice.DispatchStatus.DispatchOK;
        }

        public static Ice.DispatchStatus parents___(RepoApi obj__, IceInternal.Incoming inS__, Ice.Current current__)
        {
            checkMode__(Ice.OperationMode.Idempotent, current__.mode);
            IceInternal.BasicStream is__ = inS__.istr();
            is__.startReadEncaps();
            string id;
            id = is__.readString();
            is__.endReadEncaps();
            IceInternal.BasicStream os__ = inS__.ostr();
            string[] ret__ = obj__.parents(id, current__);
            os__.writeStringSeq(ret__);
            return Ice.DispatchStatus.DispatchOK;
        }

        public static Ice.DispatchStatus addParent___(RepoApi obj__, IceInternal.Incoming inS__, Ice.Current current__)
        {
            checkMode__(Ice.OperationMode.Normal, current__.mode);
            IceInternal.BasicStream is__ = inS__.istr();
            is__.startReadEncaps();
            string id;
            id = is__.readString();
            string parent;
            parent = is__.readString();
            is__.endReadEncaps();
            obj__.addParent(id, parent, current__);
            return Ice.DispatchStatus.DispatchOK;
        }

        public static Ice.DispatchStatus removeParent___(RepoApi obj__, IceInternal.Incoming inS__, Ice.Current current__)
        {
            checkMode__(Ice.OperationMode.Normal, current__.mode);
            IceInternal.BasicStream is__ = inS__.istr();
            is__.startReadEncaps();
            string id;
            id = is__.readString();
            string parent;
            parent = is__.readString();
            is__.endReadEncaps();
            obj__.removeParent(id, parent, current__);
            return Ice.DispatchStatus.DispatchOK;
        }

        public static Ice.DispatchStatus outcomingLinks___(RepoApi obj__, IceInternal.Incoming inS__, Ice.Current current__)
        {
            checkMode__(Ice.OperationMode.Idempotent, current__.mode);
            IceInternal.BasicStream is__ = inS__.istr();
            is__.startReadEncaps();
            string id;
            id = is__.readString();
            is__.endReadEncaps();
            IceInternal.BasicStream os__ = inS__.ostr();
            string[] ret__ = obj__.outcomingLinks(id, current__);
            os__.writeStringSeq(ret__);
            return Ice.DispatchStatus.DispatchOK;
        }

        public static Ice.DispatchStatus incomingLinks___(RepoApi obj__, IceInternal.Incoming inS__, Ice.Current current__)
        {
            checkMode__(Ice.OperationMode.Idempotent, current__.mode);
            IceInternal.BasicStream is__ = inS__.istr();
            is__.startReadEncaps();
            string id;
            id = is__.readString();
            is__.endReadEncaps();
            IceInternal.BasicStream os__ = inS__.ostr();
            string[] ret__ = obj__.incomingLinks(id, current__);
            os__.writeStringSeq(ret__);
            return Ice.DispatchStatus.DispatchOK;
        }

        public static Ice.DispatchStatus links___(RepoApi obj__, IceInternal.Incoming inS__, Ice.Current current__)
        {
            checkMode__(Ice.OperationMode.Idempotent, current__.mode);
            IceInternal.BasicStream is__ = inS__.istr();
            is__.startReadEncaps();
            string id;
            id = is__.readString();
            is__.endReadEncaps();
            IceInternal.BasicStream os__ = inS__.ostr();
            string[] ret__ = obj__.links(id, current__);
            os__.writeStringSeq(ret__);
            return Ice.DispatchStatus.DispatchOK;
        }

        public static Ice.DispatchStatus typeName___(RepoApi obj__, IceInternal.Incoming inS__, Ice.Current current__)
        {
            checkMode__(Ice.OperationMode.Idempotent, current__.mode);
            IceInternal.BasicStream is__ = inS__.istr();
            is__.startReadEncaps();
            string id;
            id = is__.readString();
            is__.endReadEncaps();
            IceInternal.BasicStream os__ = inS__.ostr();
            string ret__ = obj__.typeName(id, current__);
            os__.writeString(ret__);
            return Ice.DispatchStatus.DispatchOK;
        }

        public static Ice.DispatchStatus property___(RepoApi obj__, IceInternal.Incoming inS__, Ice.Current current__)
        {
            checkMode__(Ice.OperationMode.Idempotent, current__.mode);
            IceInternal.BasicStream is__ = inS__.istr();
            is__.startReadEncaps();
            string id;
            id = is__.readString();
            string propertyName;
            propertyName = is__.readString();
            is__.endReadEncaps();
            IceInternal.BasicStream os__ = inS__.ostr();
            string ret__ = obj__.property(id, propertyName, current__);
            os__.writeString(ret__);
            return Ice.DispatchStatus.DispatchOK;
        }

        public static Ice.DispatchStatus stringProperty___(RepoApi obj__, IceInternal.Incoming inS__, Ice.Current current__)
        {
            checkMode__(Ice.OperationMode.Idempotent, current__.mode);
            IceInternal.BasicStream is__ = inS__.istr();
            is__.startReadEncaps();
            string id;
            id = is__.readString();
            string propertyName;
            propertyName = is__.readString();
            is__.endReadEncaps();
            IceInternal.BasicStream os__ = inS__.ostr();
            string ret__ = obj__.stringProperty(id, propertyName, current__);
            os__.writeString(ret__);
            return Ice.DispatchStatus.DispatchOK;
        }

        public static Ice.DispatchStatus setProperty___(RepoApi obj__, IceInternal.Incoming inS__, Ice.Current current__)
        {
            checkMode__(Ice.OperationMode.Normal, current__.mode);
            IceInternal.BasicStream is__ = inS__.istr();
            is__.startReadEncaps();
            string id;
            id = is__.readString();
            string propertyName;
            propertyName = is__.readString();
            string value;
            value = is__.readString();
            is__.endReadEncaps();
            obj__.setProperty(id, propertyName, value, current__);
            return Ice.DispatchStatus.DispatchOK;
        }

        public static Ice.DispatchStatus removeProperty___(RepoApi obj__, IceInternal.Incoming inS__, Ice.Current current__)
        {
            checkMode__(Ice.OperationMode.Normal, current__.mode);
            IceInternal.BasicStream is__ = inS__.istr();
            is__.startReadEncaps();
            string id;
            id = is__.readString();
            string propertyName;
            propertyName = is__.readString();
            is__.endReadEncaps();
            obj__.removeProperty(id, propertyName, current__);
            return Ice.DispatchStatus.DispatchOK;
        }

        public static Ice.DispatchStatus hasProperty___(RepoApi obj__, IceInternal.Incoming inS__, Ice.Current current__)
        {
            checkMode__(Ice.OperationMode.Idempotent, current__.mode);
            IceInternal.BasicStream is__ = inS__.istr();
            is__.startReadEncaps();
            string id;
            id = is__.readString();
            string propertyName;
            propertyName = is__.readString();
            is__.endReadEncaps();
            IceInternal.BasicStream os__ = inS__.ostr();
            bool ret__ = obj__.hasProperty(id, propertyName, current__);
            os__.writeBool(ret__);
            return Ice.DispatchStatus.DispatchOK;
        }

        public static Ice.DispatchStatus from___(RepoApi obj__, IceInternal.Incoming inS__, Ice.Current current__)
        {
            checkMode__(Ice.OperationMode.Idempotent, current__.mode);
            IceInternal.BasicStream is__ = inS__.istr();
            is__.startReadEncaps();
            string id;
            id = is__.readString();
            is__.endReadEncaps();
            IceInternal.BasicStream os__ = inS__.ostr();
            string ret__ = obj__.from(id, current__);
            os__.writeString(ret__);
            return Ice.DispatchStatus.DispatchOK;
        }

        public static Ice.DispatchStatus setFrom___(RepoApi obj__, IceInternal.Incoming inS__, Ice.Current current__)
        {
            checkMode__(Ice.OperationMode.Normal, current__.mode);
            IceInternal.BasicStream is__ = inS__.istr();
            is__.startReadEncaps();
            string id;
            id = is__.readString();
            string from;
            from = is__.readString();
            is__.endReadEncaps();
            obj__.setFrom(id, from, current__);
            return Ice.DispatchStatus.DispatchOK;
        }

        public static Ice.DispatchStatus to___(RepoApi obj__, IceInternal.Incoming inS__, Ice.Current current__)
        {
            checkMode__(Ice.OperationMode.Idempotent, current__.mode);
            IceInternal.BasicStream is__ = inS__.istr();
            is__.startReadEncaps();
            string id;
            id = is__.readString();
            is__.endReadEncaps();
            IceInternal.BasicStream os__ = inS__.ostr();
            string ret__ = obj__.to(id, current__);
            os__.writeString(ret__);
            return Ice.DispatchStatus.DispatchOK;
        }

        public static Ice.DispatchStatus setTo___(RepoApi obj__, IceInternal.Incoming inS__, Ice.Current current__)
        {
            checkMode__(Ice.OperationMode.Normal, current__.mode);
            IceInternal.BasicStream is__ = inS__.istr();
            is__.startReadEncaps();
            string id;
            id = is__.readString();
            string to;
            to = is__.readString();
            is__.endReadEncaps();
            obj__.setTo(id, to, current__);
            return Ice.DispatchStatus.DispatchOK;
        }

        public static Ice.DispatchStatus fromPort___(RepoApi obj__, IceInternal.Incoming inS__, Ice.Current current__)
        {
            checkMode__(Ice.OperationMode.Idempotent, current__.mode);
            IceInternal.BasicStream is__ = inS__.istr();
            is__.startReadEncaps();
            string id;
            id = is__.readString();
            is__.endReadEncaps();
            IceInternal.BasicStream os__ = inS__.ostr();
            double ret__ = obj__.fromPort(id, current__);
            os__.writeDouble(ret__);
            return Ice.DispatchStatus.DispatchOK;
        }

        public static Ice.DispatchStatus setFromPort___(RepoApi obj__, IceInternal.Incoming inS__, Ice.Current current__)
        {
            checkMode__(Ice.OperationMode.Normal, current__.mode);
            IceInternal.BasicStream is__ = inS__.istr();
            is__.startReadEncaps();
            string id;
            id = is__.readString();
            double fromPort;
            fromPort = is__.readDouble();
            is__.endReadEncaps();
            obj__.setFromPort(id, fromPort, current__);
            return Ice.DispatchStatus.DispatchOK;
        }

        public static Ice.DispatchStatus toPort___(RepoApi obj__, IceInternal.Incoming inS__, Ice.Current current__)
        {
            checkMode__(Ice.OperationMode.Idempotent, current__.mode);
            IceInternal.BasicStream is__ = inS__.istr();
            is__.startReadEncaps();
            string id;
            id = is__.readString();
            is__.endReadEncaps();
            IceInternal.BasicStream os__ = inS__.ostr();
            double ret__ = obj__.toPort(id, current__);
            os__.writeDouble(ret__);
            return Ice.DispatchStatus.DispatchOK;
        }

        public static Ice.DispatchStatus setToPort___(RepoApi obj__, IceInternal.Incoming inS__, Ice.Current current__)
        {
            checkMode__(Ice.OperationMode.Normal, current__.mode);
            IceInternal.BasicStream is__ = inS__.istr();
            is__.startReadEncaps();
            string id;
            id = is__.readString();
            double toPort;
            toPort = is__.readDouble();
            is__.endReadEncaps();
            obj__.setToPort(id, toPort, current__);
            return Ice.DispatchStatus.DispatchOK;
        }

        public static Ice.DispatchStatus otherEntityFromLink___(RepoApi obj__, IceInternal.Incoming inS__, Ice.Current current__)
        {
            checkMode__(Ice.OperationMode.Idempotent, current__.mode);
            IceInternal.BasicStream is__ = inS__.istr();
            is__.startReadEncaps();
            string linkId;
            linkId = is__.readString();
            string firstNode;
            firstNode = is__.readString();
            is__.endReadEncaps();
            IceInternal.BasicStream os__ = inS__.ostr();
            string ret__ = obj__.otherEntityFromLink(linkId, firstNode, current__);
            os__.writeString(ret__);
            return Ice.DispatchStatus.DispatchOK;
        }

        public static Ice.DispatchStatus exterminate___(RepoApi obj__, IceInternal.Incoming inS__, Ice.Current current__)
        {
            checkMode__(Ice.OperationMode.Normal, current__.mode);
            inS__.istr().skipEmptyEncaps();
            obj__.exterminate(current__);
            return Ice.DispatchStatus.DispatchOK;
        }

        public static Ice.DispatchStatus save___(RepoApi obj__, IceInternal.Incoming inS__, Ice.Current current__)
        {
            checkMode__(Ice.OperationMode.Normal, current__.mode);
            inS__.istr().skipEmptyEncaps();
            obj__.save(current__);
            return Ice.DispatchStatus.DispatchOK;
        }

        private static string[] all__ =
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

        public override Ice.DispatchStatus dispatch__(IceInternal.Incoming inS__, Ice.Current current__)
        {
            int pos = _System.Array.BinarySearch(all__, current__.operation, IceUtilInternal.StringUtil.OrdinalStringComparer);
            if(pos < 0)
            {
                throw new Ice.OperationNotExistException(current__.id, current__.facet, current__.operation);
            }

            switch(pos)
            {
                case 0:
                {
                    return addChild___(this, inS__, current__);
                }
                case 1:
                {
                    return addParent___(this, inS__, current__);
                }
                case 2:
                {
                    return children___(this, inS__, current__);
                }
                case 3:
                {
                    return exterminate___(this, inS__, current__);
                }
                case 4:
                {
                    return from___(this, inS__, current__);
                }
                case 5:
                {
                    return fromPort___(this, inS__, current__);
                }
                case 6:
                {
                    return hasProperty___(this, inS__, current__);
                }
                case 7:
                {
                    return ice_id___(this, inS__, current__);
                }
                case 8:
                {
                    return ice_ids___(this, inS__, current__);
                }
                case 9:
                {
                    return ice_isA___(this, inS__, current__);
                }
                case 10:
                {
                    return ice_ping___(this, inS__, current__);
                }
                case 11:
                {
                    return incomingLinks___(this, inS__, current__);
                }
                case 12:
                {
                    return links___(this, inS__, current__);
                }
                case 13:
                {
                    return name___(this, inS__, current__);
                }
                case 14:
                {
                    return otherEntityFromLink___(this, inS__, current__);
                }
                case 15:
                {
                    return outcomingLinks___(this, inS__, current__);
                }
                case 16:
                {
                    return parents___(this, inS__, current__);
                }
                case 17:
                {
                    return property___(this, inS__, current__);
                }
                case 18:
                {
                    return removeChild___(this, inS__, current__);
                }
                case 19:
                {
                    return removeElement___(this, inS__, current__);
                }
                case 20:
                {
                    return removeParent___(this, inS__, current__);
                }
                case 21:
                {
                    return removeProperty___(this, inS__, current__);
                }
                case 22:
                {
                    return save___(this, inS__, current__);
                }
                case 23:
                {
                    return setFrom___(this, inS__, current__);
                }
                case 24:
                {
                    return setFromPort___(this, inS__, current__);
                }
                case 25:
                {
                    return setName___(this, inS__, current__);
                }
                case 26:
                {
                    return setProperty___(this, inS__, current__);
                }
                case 27:
                {
                    return setTo___(this, inS__, current__);
                }
                case 28:
                {
                    return setToPort___(this, inS__, current__);
                }
                case 29:
                {
                    return stringProperty___(this, inS__, current__);
                }
                case 30:
                {
                    return to___(this, inS__, current__);
                }
                case 31:
                {
                    return toPort___(this, inS__, current__);
                }
                case 32:
                {
                    return typeName___(this, inS__, current__);
                }
            }

            _System.Diagnostics.Debug.Assert(false);
            throw new Ice.OperationNotExistException(current__.id, current__.facet, current__.operation);
        }

        #endregion

        #region Marshaling support

        public override void write__(IceInternal.BasicStream os__)
        {
            os__.writeTypeId(ice_staticId());
            os__.startWriteSlice();
            os__.endWriteSlice();
            base.write__(os__);
        }

        public override void read__(IceInternal.BasicStream is__, bool rid__)
        {
            if(rid__)
            {
                /* string myId = */ is__.readTypeId();
            }
            is__.startReadSlice();
            is__.endReadSlice();
            base.read__(is__, true);
        }

        public override void write__(Ice.OutputStream outS__)
        {
            Ice.MarshalException ex = new Ice.MarshalException();
            ex.reason = "type RepoIce::RepoApi was not generated with stream support";
            throw ex;
        }

        public override void read__(Ice.InputStream inS__, bool rid__)
        {
            Ice.MarshalException ex = new Ice.MarshalException();
            ex.reason = "type RepoIce::RepoApi was not generated with stream support";
            throw ex;
        }

        #endregion
    }
}
