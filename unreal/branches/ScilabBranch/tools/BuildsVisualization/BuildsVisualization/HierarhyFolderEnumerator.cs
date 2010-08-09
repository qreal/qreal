using System;
using System.Collections.Generic;
using System.Web;
using System.Collections;
using System.IO;
using System.Text.RegularExpressions;

namespace BuildsVisualization
{
    public class HierarhyFolderEnumerator : IEnumerator
    {
        private DirectoryInfo dirInfo;
        private int currentChild = -1;

        public HierarhyFolderEnumerator(string dirName)
        {
            this.dirInfo = new DirectoryInfo(dirName);
        }

        public object Current
        {
            get { return dirInfo.GetDirectories()[currentChild]; }
        }

        public bool MoveNext()
        {
            if (IsCurrentBuildsFolder())
            {
                return false;
            }
            int childrenCount = dirInfo.GetDirectories().Length;
            while (currentChild < childrenCount - 1)
            {
                currentChild++;
                if (dirInfo.GetDirectories()[currentChild].Name == "bin")
                {
                    continue;
                }
                return true;
            }
            return false;
        }

        private bool IsCurrentBuildsFolder()
        {
            foreach (DirectoryInfo directory in dirInfo.GetDirectories())
            {
                Regex buildRegEx = new Regex(@"[\d*.]{4}");
                if (!buildRegEx.IsMatch(directory.Name))
                {
                    return false;
                }
            }
            return true;
        }

        public void Reset()
        {
            currentChild = -1;
        }
    }
}
