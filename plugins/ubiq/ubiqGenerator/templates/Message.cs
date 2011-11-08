using System;
using System.Collections.Generic;
//using System.Linq;
using System.Diagnostics;
using System.Text;
using System.IO;
using Ubiq.MessagingAPI;
using Ubiq.QRealAPI;

namespace DeviceService
{
    public class Message: QRealMessage
    {
        public const int KHeaderSize             = 9; // 4 + 1 + 2 + 1 + 1
/*  M e s s a g e   c o d e s   */
@@MessageCodes@@
        public const int KMsgError                = 255;
     
        
/*  E r r o r   c o d e s   */
        public const int KErrNo                      = 0;
        public const int KErrWrongCode               = -1;
@@ErrorCodes@@

/*   Fields    */
@@Properties@@
        public int Command  {get; set;}
        public int Code     {get; set;}

        private byte[] _data;

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

        private MemoryStream _outStream;
        private BinaryReader _reader;
        private BinaryWriter _writer;

        public Message(@@ConstructorArgs@@int command)
        {
            Init();
@@InitFieldsWithArgs@@
            Command = command;
        }

        public static Message ErrorMessage(@@ConstructorArgs@@int errCode)
        {
            Message msg = new Message(@@ConstructorActualArgs@@KMsgError);
            msg.Code = errCode;
            return msg;
        }
        
        public Message(byte[] aData)
        {
            Init();
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

        public Message(@@ConstructorArgs@@byte[] aData)
        {
            Init();
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

        public override void Init()
        {
@@InitFieldsWithDefaults@@
            Command = KMsgNoCommand;
            Code = KErrNo;
            _data = null;
            _outStream = null;
            _reader = null;
            _writer = null;
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


        public override bool Parse(byte[] aData)
        {
            if (_data != null || _reader != null || _writer != null) return false;

            var rdr = new BinaryReader(new MemoryStream(aData));
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
            rdr.Close();
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
