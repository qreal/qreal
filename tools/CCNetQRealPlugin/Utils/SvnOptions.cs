using Exortech.NetReflector;

namespace QReal.Utils
{
    [ReflectorType("SvnOptions")]
    public class SvnOptions
    {
        [ReflectorProperty("url", Required = true)]
        public string Url { get; set; }
    }
}
