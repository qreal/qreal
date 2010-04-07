using System;
using System.Collections.Generic;
using System.Web;
using System.Web.UI;
using System.IO;

namespace BuildsVisualization
{
    public class HierarchicalFolder : IHierarchyData
    {
        private DirectoryInfo dirInfo;

        public HierarchicalFolder(DirectoryInfo dirInfo)
        {
            this.dirInfo = dirInfo;
        }

        public IHierarchicalEnumerable GetChildren()
        {
            return new HierarchicalFolderIEnumearable(dirInfo.FullName);
        }

        public IHierarchyData GetParent()
        {
            return new HierarchicalFolder(dirInfo.Parent);
        }

        public bool HasChildren
        {
            get { return dirInfo.GetDirectories().Length > 0; }
        }

        public object Item
        {
            get { return dirInfo.Name; }
        }

        public string Path
        {
            get { return dirInfo.FullName; }
        }

        public string Type
        {
            get { return "FolderHierarhical"; }
        }
    }
}
