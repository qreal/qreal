using System;
using System.Collections.Generic;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.IO;

namespace BuildsVisualization
{
    public partial class _Default : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            string dirName = @"C:\Builds";
            HierarchicalFolderIEnumearable folders = new HierarchicalFolderIEnumearable(dirName);
            buildPlatformsTreeView.DataSource = folders;
            buildPlatformsTreeView.DataBind();
        }

        protected void buildPlatformsTreeView_SelectedNodeChanged(object sender, EventArgs e)
        {
            string directorySelected = buildPlatformsTreeView.SelectedNode.DataPath;
            DirectoryInfo dirInfo = new DirectoryInfo(directorySelected);
            DirectoryInfo[] directories = dirInfo.GetDirectories();
            List<BuildInfo> builds = new List<BuildInfo>();
            foreach (DirectoryInfo build in directories)
            {
                builds.Add(new BuildInfo(build));
            }
            buildsGridView.DataSource = builds;
            buildsGridView.DataBind();
        }
    }
}
