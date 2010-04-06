using System;
using System.Xml;
using ThoughtWorks.CruiseControl.Core.Util;

namespace QReal.Utils
{
    public class SvnProcessHelper
    {
        public static int GetSvnRevision(SvnOptions svnOptions)
        {
            ProcessArgumentBuilder argBuilder = new ProcessArgumentBuilder();
            argBuilder.AppendArgument("log");
            argBuilder.AppendArgument("--xml");
            argBuilder.AppendArgument("--limit 1");
            argBuilder.AddArgument(StringHelper.Quote(svnOptions.Url));
            ProcessResult result = RunSvnProcess(svnOptions, argBuilder);

            XmlDocument xml = new XmlDocument();
            xml.LoadXml(result.StandardOutput);

            XmlNode node = xml.SelectSingleNode("/log/logentry/@revision");
            return Convert.ToInt32(node.InnerText);
        }

        public static ProcessResult RunSvnProcess(SvnOptions svnLoginOptions, ProcessArgumentBuilder argBuilder)
        {
            argBuilder.AddArgument("--non-interactive");
            argBuilder.AddArgument("--no-auth-cache");

            ProcessInfo info = new ProcessInfo("svn.exe", argBuilder.ToString());

            ProcessExecutor executor = new ProcessExecutor();
            ProcessResult result = executor.Execute(info);
            return result;
        }
    }
}
