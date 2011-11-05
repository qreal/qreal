// @@TEST@@

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Text;
using System.IO;
using Ubiq.MessagingAPI;

namespace DeviceService
{
    public class Message: Msg
    {
/*  M e s s a g e   c o d e s   */
@@MessageCodes@@
        public const int KMsgError                = 255;

/*  E r r o r   c o d e s   */
@@ErrorCodes@@

/*   Internal fields    */
@@Properties@@

        private int iCommand;
        public int Command
        {
            get
            {
                return iCommand;
            }
        }
        private int iCode;
        public int Code
        {
            get
            {
                return iCode;
            }
        }

        private byte[] iData;
        private MemoryStream iOutStream;

        public int Length
        {
            get
            {
                if (iData != null)
                    return iData.GetLength(0);
                else if (iOutStream != null)
                    return (int) iOutStream.Length;
                else return -1;
            }
        }

        private BinaryReader iReader;
        private BinaryWriter iWriter;

        public static bool iBigEndianFlag = true;

        public void initAsError()
        {
@@InitFieldsWithDefaults@@
            iCommand = KMsgError;
            iCode = KErrBadParameters;
            iReader = null;
            iWriter = null;
            iData = null;
        }

        public Message(@@ConstructorArgs@@int aCommand)
        {
@@InitFieldsWithArgs@@
            iCommand = aCommand;
            iCode = KErrNo;
            iData = null;
            iReader = null;
            iWriter = null;
        }

        public static Message ErrorMessage(@@ConstructorArgs@@int aErrCode)
        {
            Message iMsg = new Message(@@ConstructorActualArgs@@KMsgError);
            iMsg.iCode = aErrCode;
            return iMsg;
        }
        
        public Message(byte[] aData)
        {
            iData = null;
            iReader = null;
            iWriter = null;
            if (!Parse(aData))
            {
                initAsError();
            }
        }

        public Message(@@ConstructorArgs@@byte[] aData)
        {
            iData = null;
            iReader = null;
            iWriter = null;
            if (!Parse(aData))
            {
                initAsError();
            }
            else 
            {
    @@InitFieldsWithArgs@@
            }
        }

        public byte GetByte()
        {
            bool iOk = ReadAvailable();
            Debug.Assert(iOk);
            return iReader.ReadByte();
        }

        public short GetInt16()
        {
            bool iOk = ReadAvailable();
            Debug.Assert(iOk);
            short res = iReader.ReadInt16();
            if (iBigEndianFlag) res = Convert16(res);
            return res;
        }

        public int GetInt32()
        {
            bool iOk = ReadAvailable();
            Debug.Assert(iOk);
            int res = iReader.ReadInt32();
            if (iBigEndianFlag) res = Convert32 (res);
            return res;
        }

        public MemoryStream GetData()
        {
            Debug.Assert(iData != null);
            return new MemoryStream(iData);
        }

        public MemoryStream GetData(int aPos)
        {
            Debug.Assert(iData != null);
            return new MemoryStream(iData, aPos, iData.GetLength(0)-aPos);
        }

        public byte[] Pack()
        {
            MemoryStream iOutStream = new MemoryStream();
            BinaryWriter iPackWriter = new BinaryWriter(iOutStream);
            int iLength = KHeaderSize;
            if (iData != null) iLength += iData.GetLength(0);
            if (iBigEndianFlag) iLength = Convert32(iLength);
            iPackWriter.Write(iLength);
            byte iDest = (byte)101; // Dispatcher
            if (iCommand == KMsgDataToSend || iCommand == KMsgSetParameters) iDest = (byte)100; // Driver
            iPackWriter.Write(iDest);
@@PackFields@@
            iPackWriter.Write((byte)iCommand);
            iPackWriter.Write((byte)iCode);
            if (iData != null) iPackWriter.Write(iData);
            iPackWriter.Flush();
            iPackWriter.Close();
            return iOutStream.ToArray();
        }

        public void Add(byte aData)
        {
            bool iOk = WriteAvailable();
            Debug.Assert(iOk);
            iWriter.Write(aData);
        }

        public void Add(Int16 aData)
        {
            bool iOk = WriteAvailable();
            Debug.Assert(iOk);
            if (iBigEndianFlag) aData = Convert16(aData);
            iWriter.Write(aData);
        }

        public void Add(int aData)
        {
            bool iOk = WriteAvailable();
            Debug.Assert(iOk);
            if (iBigEndianFlag) aData = Convert32(aData);
            iWriter.Write(aData);
        }

        public void Add(byte[] aData)
        {
            bool iOk = WriteAvailable();
            Debug.Assert(iOk);
            iWriter.Write(aData);
        }

        public void Close()
        {

            if (iReader != null)
            {
                iReader.Close();
                iReader = null;
            }
            else if (iWriter != null)
            {
                Debug.Assert(iData == null && iOutStream != null);
                iWriter.Flush();
                iWriter.Close();
                iData = iOutStream.ToArray();
                iWriter = null;
                iOutStream = null;
            }
        }

        public static void SetBigEndian()
        {
            iBigEndianFlag = true;
        }

        public static void SetLittleEndian()
        {
            iBigEndianFlag = false;
        }

       public static int Convert32(int i)
        {
            UInt32 src = (UInt32)(i);
            UInt32 res = 0;
            for (int ii = 0; ii < 4; ++ii)
                res |= (((src >> ((3 - ii) * 8)) & 0xff) << (ii * 8));
            return (int)(res);
        }

        public static short Convert16(short i)
        {
            UInt32 src = (UInt32)(i);
            UInt32 res;
            res = ((src >> 8) & 0xff) | ((src & 0xff) << 8);
            return (short)(res);
        }

        public static void ConvertData (byte [] data, int pos, int len)
        {
            if (len == 4)
            {
                byte b = data[pos]; data[pos] = data[pos+3]; data[pos+3] = b;
                b = data[pos+1];data[pos+1] = data[pos+2]; data[pos+2] = b;
            }
            else if (len == 2)
            {
                byte b = data[pos]; data[pos] = data[pos+1]; data[pos+1] = b;
            }
        }

        private void Init()
        {
@@InitFieldsWithDefaults@@
            iCommand = KMsgNoCommand;
            iCode = KErrNo;
            iData = null;
            iOutStream = null;
            iReader = null;
            iWriter = null;
        }

        private bool Parse(byte[] aData)
        {
            if (iData != null || iReader != null || iWriter != null) return false;

            BinaryReader iRdr = new BinaryReader(new MemoryStream(aData));
            int iLen = iRdr.ReadInt32();
            if (iBigEndianFlag) iLen = Convert32(iLen);
//            if (iLen != aData.GetLength(0))
//                return false;
// Here some sort of Try/Catch should be - for the case of incorrect data reading
            byte aux = iRdr.ReadByte();   // dest - now obsolete
@@UnpackFields@@
            iCommand = (int)iRdr.ReadByte();
            iCode = (int)iRdr.ReadByte();

            if (iLen > KHeaderSize)
            {
                Debug.Assert(iData == null);
                iData = (byte[])Array.CreateInstance (typeof (byte), iLen-KHeaderSize);
                Array.Copy(aData, KHeaderSize, iData, 0, iLen - KHeaderSize);
                iReader = new BinaryReader(new MemoryStream(iData));
            }
            return true;
        }

        private bool ReadAvailable()
        {
            bool res = (iWriter == null && iData != null);
            if (iReader == null) iReader = new BinaryReader(new MemoryStream(iData));
            return res;
        }

        private bool WriteAvailable()
        {
            bool res = (iReader == null && iData == null);
            if (iWriter == null)
            {
                iOutStream = new MemoryStream();
                iWriter = new BinaryWriter(iOutStream);
            }
            return res;
        }

        public override bool SendAvailable()
        {
            return (iOutStream == null && iWriter == null);
        }
    }
}
