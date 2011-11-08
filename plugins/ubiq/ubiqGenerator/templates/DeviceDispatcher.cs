using System;
using System.Diagnostics;
using System.Collections.Generic;
using System.Text;
using System.IO;
using Ubiq;
using Ubiq.TcpIpAPI;
using Ubiq.MessagingAPI;
using Ubiq.QRealAPI;

namespace DeviceService
{
    public class DeviceDispatcher: QRealApp
    {
@@Constants@@
@@Fields@@
        public DeviceDispatcher ()
        {
@@InitCode@@
        }

        public override void MainOverride()
        {
@@MessageInputMethods@@
            ProcessIncomingMessages();
        }

@@Preprocessors@@
@@EventHandlers@@

@@HelperFunctions@@

}
