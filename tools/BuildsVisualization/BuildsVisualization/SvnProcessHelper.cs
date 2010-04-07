using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using System.Diagnostics;
using System.IO;
using System.Threading;

namespace BuildsVisualization
{
    public class SvnProcessHelper
    {
        private static string Quote(string stringToBeQuoted)
        {
            return String.Format(@"""{0}""", stringToBeQuoted);
        }

        public static CommitInfo GetSvnCommitInfo(int revision)
        {
            StringBuilder argBuilder = new StringBuilder();
            argBuilder.Append(" log");
            argBuilder.Append(" --xml");
            argBuilder.Append(" -r" + revision);
            argBuilder.Append(" --non-interactive");
            argBuilder.Append(" --no-auth-cache");
            argBuilder.Append(" " + Quote("http://unreal.tepkom.ru/svn/unreal/"));

            ProcessStartInfo info = new ProcessStartInfo("svn.exe", argBuilder.ToString());
            info.UseShellExecute = false;
            info.ErrorDialog = false;
            info.CreateNoWindow = true;
            info.RedirectStandardOutput = true;

            Process process = new Process();
            process.StartInfo = info;
            process.Start();
            string processResult = process.StandardOutput.ReadToEnd();
            process.WaitForExit();

            XmlDocument xml = new XmlDocument();
            xml.LoadXml(processResult);

            XmlNode node = xml.SelectSingleNode("/log/logentry");
            XmlNode messageNode = node.SelectSingleNode("msg");
            string message = messageNode != null ? EncodeMessageForConsoleOutput(messageNode.InnerText) : null;
            XmlNode authorNode = node.SelectSingleNode("author");
            string author = authorNode != null ? authorNode.InnerText : null;
            CommitInfo commitInfo = new CommitInfo(revision, message, author);
            return commitInfo;
        }

        public static string Convert(string value, Encoding src, Encoding trg)
        {
            Decoder dec = src.GetDecoder();
            byte[] ba = trg.GetBytes(value);
            int len = dec.GetCharCount(ba, 0, ba.Length);
            char[] ca = new char[len];
            dec.GetChars(ba, 0, ba.Length, ca, 0);
            return new string(ca);
        }

        private static string EncodeMessageForConsoleOutput(string Message)
        {
            string converted1 = Convert(Message, Encoding.Default, Encoding.GetEncoding("cp866"));
            return Convert(converted1, Encoding.UTF8, Encoding.Default);
        }
    }
}
