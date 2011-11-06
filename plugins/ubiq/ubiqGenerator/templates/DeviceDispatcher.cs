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
@@MessageInputMethods@@
            ProcessIncomingMessages();
        }

@@EventHandlers@@

        internal short Register(int devID, int peerSocket)
        {
            int newID = devID;
            bool newFlag = false;
            if (newID == 0)
            {
                newFlag = true;
                newID = (short)random.Next(1000, 10000);
            }
            bool chck = false;
            while (!chck)
            {
                chck = true;
                foreach (DevRecord devRecord in devTab)
                {
                    if (devRecord.ID == newID)
                    {
                        if (newFlag)
                        {
                            newID = (short)random.Next(1000, 10000);
                            chck = false;
                            break;
                        }
                        else
                            return newID;
                    }
                }
            }
            if (!newFlag)
                AddDevice(newID, peerSocket);
            return newID;
        }

        internal bool SendCommandToDevice(int devID, int messageCode)
        {
        }

        internal bool SendDataToDevice (int devID, [] byte data)
        {
        }

        private void NotifyUsers(short devID, byte messageCode)
        {
            List<MailBox> userList = GetUserList(devID);
            if (userList != null)
            {
                Message msg = new Message(devID, messageCode);
                foreach (MailBox userBox in userList)
                    _sendMessage(msg, userBox);
            }
        }

        internal void NotifyUsers (short devID, byte[] data)
        {
            List<MailBox> userList = GetUserList(devID);
            if (userList != null)
            {
                Message msg = new Message(devID, data);
                foreach (MailBox userBox in userList)
                {
                    _sendMessage(msg, userBox);
                }
            }
        }

        internal void NotifyUsersErr(short devID, int errCode)
        {
            List<MailBox> userList = GetUserList(devID);
            if (userList != null)
            {
                Message msg = Message.ErrorMessage(devID, (byte)errCode);
                foreach (MailBox mailBox in userList)
                    _sendMessage(msg, mailBox);
            }

        }        
        
       internal void RemoveDevice (int devID)
       {
           foreach (DevRecord devRecord in devTab)
               if (devRecord.ID == devID) devTab.Remove(devRecord);
       }

       internal void AddUser (int devID, MailBox userBox)
       {
            foreach (DevRecord devRecord in devTab) 
            {
                if (devRecord.ID == devID && !devRecord.UserList.Contains(userBox)) 
                    devRecord.UserList.Add(userBox);
            }
        }

        internal void RemoveUser (int devID, MailBox user)
        {
            foreach (DevRecord devRecord in devTab) 
            {
                if (devRecord.ID == devID && devRecord.UserList.Contains(userBox)) 
                    devRecord.UserList.Remove(userBox);
            }
        }


       internal bool DeviceExists (int devID)
       {
           foreach (DevRecord devRecord in devTab)
               if (devRecord.ID == devID) return true;
           return false;
       }

       internal int SearchSocket (int devID)
       {
           foreach (DevRecord devRecord in devTab)
           {
               if (devRecord.ID == devID && devRecord.Socket != 0) return devRecord.Socket;
           }
           return 0;
       }
    }
}
