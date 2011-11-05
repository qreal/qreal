using System;
using System.Collections.Generic;
//using System.Linq;
using System.Diagnostics;
using System.Text;
using System.IO;
using Ubiq.MessagingAPI;

namespace DeviceService
{
    public class Message: Msg
    {
        public const int KHeaderSize             = 9; // 4 + 1 + 2 + 1 + 1
/*  M e s s a g e   c o d e s   */
@@MessageCodes@@
        public const int KMsgError                = 255;
     
        
/*  E r r o r   c o d e s   */
@@ErrorCodes@@

/*   Internal fields    */
@@Properties@@
        public int Command  {get; set;}
        public int Code     {get; set;}

        private byte[] _data;
        private MemoryStream _outStream;

        public int Length
        {
            get
            {
                if (_data != null)
                    return _data.GetLength(0);
                else if (_outStream != null)
                    return (int) _outStream.Length;
                else return -1;
            }
        }

        private BinaryReader _reader;
        private BinaryWriter _writer;

        public static bool bigEndianFlag = true;

        public Message(@@ConstructorArgs@@int command)
        {
@@InitFieldsWithArgs@@
            Command = command;
            Code = KErrNo;
            _data = null;
            _reader = null;
            _writer = null;
        }

        public static Message ErrorMessage(@@ConstructorArgs@@int errCode)
        {
            Message msg = new Message(@@ConstructorActualArgs@@KMsgError);
            msg.Code = errCode;
            return msg;
        }
        
        public Message(byte[] aData)
        {
            _data = null;
            _reader = null;
            _writer = null;
            if (!Parse(aData))
            {
@@InitFieldsWithDefaults@@
                Command = KMsgError;
                Code = KErrBadParameters;
                _reader = null;
                _writer = null;
                _data = null;
            }
        }

        public Message(int deviceID, byte[] aData)
        {
            _data = null;
            _reader = null;
            _writer = null;
            if (!Parse(aData))
            {
@@InitFieldsWithDefaults@@
                Command = KMsgError;
                Code = KErrBadParameters;
                _reader = null;
                _writer = null;
                _data = null;
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
            return _reader.ReadByte();
        }

        public short GetInt16()
        {
            bool iOk = ReadAvailable();
            Debug.Assert(iOk);
            short res = _reader.ReadInt16();
            if (bigEndianFlag) res = Convert16(res);
            return res;
        }

        public int GetInt32()
        {
            Debug.Assert(ReadAvailable());
            int res = _reader.ReadInt32();
            if (bigEndianFlag) res = Convert32 (res);
            return res;
        }

        public MemoryStream GetData()
        {
            Debug.Assert(_data != null);
            return new MemoryStream(_data);
        }

        public MemoryStream GetData(int aPos)
        {
            Debug.Assert(_data != null);
            return new MemoryStream(_data, aPos, _data.GetLength(0)-aPos);
        }

        public byte[] Pack()
        {
            MemoryStream outStream = new MemoryStream();
            BinaryWriter packWriter = new BinaryWriter(outStream);
            int iLength = KHeaderSize;
            if (_data != null) iLength += _data.GetLength(0);
            if (bigEndianFlag) iLength = Convert32(iLength);
            packWriter.Write(iLength);
            byte iDest = (byte)101; // Dispatcher
            if (Command == KMsgDataToSend || Command == KMsgSetParameters) iDest = (byte)100; // Driver
            packWriter.Write(iDest);
@@PackFields@@
            packWriter.Write((byte)Command);
            packWriter.Write((byte)Code);
            if (_data != null) packWriter.Write(_data);
            packWriter.Flush();
            packWriter.Close();
            return outStream.ToArray();
        }

        public void Add(byte aData)
        {
            Debug.Assert(WriteAvailable ());
            _writer.Write(aData);
        }

        public void Add(Int16 aData)
        {
            Debug.Assert(WriteAvailable());
            if (bigEndianFlag) aData = Convert16(aData);
            _writer.Write(aData);
        }

        public void Add(int aData)
        {
            Debug.Assert(WriteAvailable());
            if (bigEndianFlag) aData = Convert32(aData);
            _writer.Write(aData);
        }

        public void Add(byte[] aData)
        {
            Debug.Assert(WriteAvailable());
            _writer.Write(aData);
        }

        public void Close()
        {

            if (_reader != null)
            {
                _reader.Close();
                _reader = null;
            }
            else if (_writer != null)
            {
                Debug.Assert(_data == null && _outStream != null);
                _writer.Flush();
                _writer.Close();
                _data = _outStream.ToArray();
                _writer = null;
                _outStream = null;
            }
        }

        public static void SetBigEndian()
        {
            bigEndianFlag = true;
        }

        public static void SetLittleEndian()
        {
            bigEndianFlag = false;
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
            Command = KMsgNoCommand;
            Code = KErrNo;
            _data = null;
            _outStream = null;
            _reader = null;
            _writer = null;
        }

        private bool Parse(byte[] aData)
        {
            if (_data != null || _reader != null || _writer != null) return false;

            BinaryReader rdr = new BinaryReader(new MemoryStream(aData));
            int len = rdr.ReadInt32();
            if (bigEndianFlag) len = Convert32(len);
            byte aux = rdr.ReadByte();   // dest - now obsolete

@@UnpackFields@@
            Command = (int)rdr.ReadByte();
            Code = (int)rdr.ReadByte();

            if (len > KHeaderSize)
            {
                Debug.Assert(_data == null);
                _data = (byte[])Array.CreateInstance (typeof (byte), len-KHeaderSize);
                Array.Copy(aData, KHeaderSize, _data, 0, len - KHeaderSize);
                _reader = new BinaryReader(new MemoryStream(_data));
            }
            return true;
        }

        private bool ReadAvailable()
        {
            bool res = (_writer == null && _data != null);
            if (_reader == null) _reader = new BinaryReader(new MemoryStream(_data));
            return res;
        }

        private bool WriteAvailable()
        {
            bool res = (_reader == null && _data == null);
            if (_writer == null)
            {
                _outStream = new MemoryStream();
                _writer = new BinaryWriter(_outStream);
            }
            return res;
        }

        public override bool SendAvailable()
        {
            return (_outStream == null && _writer == null);
        }
    }
}
