using System;
using System.Collections.Generic;
using System.Text;
using Exortech.NetReflector;

namespace QReal.Utils
{
    [ReflectorType("SvnLoginOptions")]
    public class SvnLoginOptions
    {
        [ReflectorProperty("username", Required = true)]
        public string Username { get; set; }

        [ReflectorProperty("password", Required = true)]
        public string Password { get; set; }
    }
}
