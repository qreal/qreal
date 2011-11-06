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
/* Структура Message должна быть в том же namespace   */

    public class DeviceDispatcher: QRealApp
    {
@@Constants@@
@@Fields@@
        public DeviceDispatcher ()
        {
/* Возможно, инициализация полей  */
        }

        public override void MainOverride()
        {
            UseMessageInput(new TcpIPMessageInput(portNumber));
            UseMessageInput(new AsyncMailBox(KBoxName));

            ProcessIncomingMessages();
        }

@@EventHandlers@@

    }
}