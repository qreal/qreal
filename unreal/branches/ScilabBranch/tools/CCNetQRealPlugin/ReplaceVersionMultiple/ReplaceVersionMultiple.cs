using System;
using System.IO;
using QReal.Utils;
using Exortech.NetReflector;
using ThoughtWorks.CruiseControl.Core;

namespace QReal.ReplaceVersionMultiple
{
    [ReflectorType("replaceVersionMultiple")]
    public class ReplaceVersionMultiple : ITask
    {
        [ReflectorProperty("targetFiles", Required = true)]
        public string TargetFiles { get; set; }

        [ReflectorProperty("version", Required = false)]
        public VersionInfo Version { get; set; }

        public void Run(IIntegrationResult result)
        {
            try
            {
                string label = GetLabel(result.Label);
                foreach (string targetFileIter in TargetFiles.Split(new string[] { " " }, StringSplitOptions.RemoveEmptyEntries))
                {
                    string targetFile = targetFileIter.Trim();
                    FileInfo fi = new FileInfo(targetFile);
                    StreamReader reader = new StreamReader(targetFile, System.Text.Encoding.GetEncoding(1251));

                    char[] buffer = new char[fi.Length];

                    int size = 0;
                    size = reader.Read(buffer, 0, buffer.Length);
                    string fileInMem = null;
                    if (0 < size)
                    {
                        fileInMem = new string(buffer, 0, size);
                        fileInMem = fileInMem.Replace("0.0.0.0", label);
                    }
                    reader.Close();

                    if (null != fileInMem)
                    {
                        FileStream fs = new FileStream(targetFile,
                                        FileMode.Truncate, FileAccess.Write, FileShare.None);
                        StreamWriter writer = new StreamWriter(fs, System.Text.Encoding.GetEncoding(1251));
                        writer.Write(fileInMem.ToCharArray());
                        writer.Flush();
                        writer.Close();
                    }
                }
            }
            catch (Exception ex)
            {
                throw new Exception("Failed during multiple version replace " + ex.ToString());
            }
        }

        private string GetLabel(string sourceLabel)
        {
            string label = sourceLabel;
            if (Version != null)
            {
                string[] labelParsed = StringHelper.ParseLabel(sourceLabel);
                labelParsed[0] = Version.Major.ToString();
                labelParsed[1] = Version.Minor.ToString();
                labelParsed[2] = SvnProcessHelper.GetSvnRevision(Version.SvnOptions).ToString();
                label = labelParsed[0] + "." + labelParsed[1] + "." + labelParsed[2] + "." + labelParsed[3];
            }
            return label;
        }
    }
}
