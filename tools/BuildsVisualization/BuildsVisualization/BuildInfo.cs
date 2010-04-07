using System;
using System.Collections.Generic;
using System.Web;
using System.IO;

namespace BuildsVisualization
{
    public class BuildInfo : CommitInfo
    {
        private string url;

        public string Url
        {
            get
            {
                return url;
            }
        }

        private string version;

        public string Version
        {
            get
            {
                return version;
            }
        }

        public BuildInfo(DirectoryInfo dirInfo)
        {
            string dirName = dirInfo.Name;
            this.version = dirName;
            int revision = Convert.ToInt32(dirName.Split('.')[2]);
            CommitInfo commitInfo = SvnProcessHelper.GetSvnCommitInfo(revision);
            this.message = commitInfo.Message;
            this.autor = commitInfo.Autor;
            string dirFullName = dirInfo.FullName;
            string newUrl = dirFullName.Substring(dirFullName.IndexOf("Builds") + 7) + @"\" + dirInfo.GetFiles()[0].Name;
            this.url = newUrl.Replace('\\','/');
        }
    }
}
