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
/*  Что с константами делать?     */
        const string KBoxName = "DeviceServicesMailBox";
        private List <DevRecord> devTab;
        static Random random = new Random();
        const int portNumber = 4546;
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

        public override void OnTcpIpMessage(Message msg)
        {
            switch (msg.Command)
            {
                case Message.KMsgRegistrationRequest:
                    {
                        Register(msg.devID, msg.socket);  // включает в себя AddDevice
                        SendCommandToDevice(msg.devID, Message.KMsgConfirmRegistration);
                    }
                    break;
                case Message.KMsgRegistrationCancel:
                    {
                        NotifyUsersError(msg.devID, Message.KErrDeviceRegistrationCancel);
                        RemoveDevice(msg.devID);
                    }
                    break;
                case Message.KMsgDeviceReady:
                    {
                        Register(msg.devID, msg.socket); // включает в себя AddDevice
                        App.Logwrite("Сообщение о готовности устройства № " + msg.devID.ToString() + ",подключённого через сокет № " + msg.socket.ToString());
                    }
                    break;
                case Message.KMsgStopTransfer:
                    {
                        NotifyUsers(msg.devID, Message.KMsgStopTransfer);
                    }
                    break;
                case Message.KMsgReceivedData:
                    {
                        NotifyUsers(msg.devID, msg.Buf);
                    }
                    break;
            }
        }

        public override void OnMailboxMessage (Message msg)
        {
            switch (msg.Command)
            {
                case Message.KMsgDeviceRequest:
                    {
                        AddUser(msg.devID, msg.Sender);
                        if (!DeviceExists(msg.devID))
                            NotifyUsersErr(msg.devID, Message.KErrDeviceNotRegistered);
                        else if (SearchSocket(msg.devID) == 0)
                            NotifyUsersErr(msg.devID, Message.KErrDeviceNotReady);
                        else
                            NotifyUsers(msg.devID, Message.KMsgDeviceReady);
                    }
                break;
                case Message.KMsgEndSession:
                    {
                        int socket = SearchSocket(msg.devID);
                        if (socket != 0)
                        {
                            SendCommandToDevice(msg.devID, Message.KMsgEndSession);
                            RemoveUser(msg.devID, msg.Sender);
                        }
                    }
                break;
                case Message.KMsgSuspendTransfer:
                    {
                        if (SearchSocket(msg.devID) != 0)
                        {
                            SendCommandToDevice(msg.devID, Message.KMsgSuspendTransfer);
                        }
                    }
                break;
                case Message.KMsgResumeTransfer:
                    {
                        if (SearchSocket(msg.devID) != 0)
                        {
                            if (!SendCommandToDevice(msg.devID, Message.KMsgREsumeTransfer))
                                NotifyUsersErr(msg.devID, Message.KMsgDeviceNotReady);
                        }
                    }
                break;
                case Message.KMsgSendingData:
                case Message.KMsgSetParameters:
                    {
                        if (SearchSocket(msg.devID) != 0)
                        {
                            if (!SendDataToDevice(msg.devID, msg.Buf))
                                NotifyUsersErr(msg.devID, Message.KMsgDeviceNotReady);
                        }
                    }
                break;
                default:
                    Debug.Assert (false);
                break;
            }
        }

        internal void Register(int devID, int socket)
        {
        }

        internal bool SendCommandToDevice(int devID, int messageCode)
        {
        }

        internal bool SendDataToDevice (int devID, [] byte data)
        {
        }

        internal void NotifyUsers (int devID, int messageCode)
        {
        }

        internal void NotifyUsersErr (int devID, int messageCode)
        {
        }

        internal void RemoveDevice (int devID)
        {
        }

        internal void AddUser (int devID, MailBox user)
        {
        }

        internal void RemoveUser (int devID, MailBox user)
        {
        }

        internal bool DeviceExists (int devID)
        {
        }

        internal int SearchSocket (int devID)
        {
        }
    }
}