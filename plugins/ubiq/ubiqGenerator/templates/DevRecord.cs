using System;
using System.Collections.Generic;
using System.Text;
using Ubiq.MessagingAPI;

namespace DeviceService
{
  class DevRecord
  {
    /* device registration id */
    private short id;
    public short Id
    {
      set
      {
        id = value;
      }
      get
      {
        return id;
      }
    }
    private int socket;
    public int Socket
    {
      set
      {
        socket = value;
      }
      get
      {
        return socket;
      }
    }

    private List<MailBox> uAMailBoxList;
    public List<MailBox> UAMailBoxList
    {
      set
      {
          uAMailBoxList = value;
      }
      get
      {
          return uAMailBoxList;
      }
    }

    public DevRecord(short id, int socket)
    {
      this.Id = id;
      this.Socket = socket;
      this.UAMailBoxList = new List<MailBox>();
    }

    public DevRecord (short id)
    {
        this.Id = id;
        this.Socket = 0;
        this.UAMailBoxList = new List<MailBox>();
    }
  }
}
