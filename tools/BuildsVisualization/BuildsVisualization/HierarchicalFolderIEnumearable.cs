using System;
using System.Collections.Generic;
using System.Web;
using System.Web.UI;
using System.IO;
using System.Collections;

namespace BuildsVisualization
{
    public class HierarchicalFolderIEnumearable : IHierarchicalEnumerable
    {
        public string dirName;

        public HierarchicalFolderIEnumearable(string dirName)
        {
            this.dirName = dirName;
        }

        public IHierarchyData GetHierarchyData(object enumeratedItem)
        {
            return new HierarchicalFolder(enumeratedItem as DirectoryInfo);
        }

        public IEnumerator GetEnumerator()
        {
            return new HierarhyFolderEnumerator(dirName);
        }
    }
}
