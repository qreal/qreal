using Exortech.NetReflector;

namespace QReal.Utils
{
    [ReflectorType("VersionInfo")]
    public class VersionInfo
    {
        [ReflectorProperty("major", Required = true)]
        public int Major { get; set; }

        [ReflectorProperty("minor", Required = true)]
        public int Minor { get; set; }

        [ReflectorProperty("svnOptions", Required = true)]
        public SvnOptions SvnOptions { get; set; }
    }
}
