using Exortech.NetReflector;

namespace QReal.Utils
{
    [ReflectorType("SvnOptions")]
    public class SvnOptions : SvnLoginOptions
    {
        [ReflectorProperty("url", Required = true)]
        public string Url { get; set; }
    }
}
