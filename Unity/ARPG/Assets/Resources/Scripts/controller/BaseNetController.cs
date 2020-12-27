using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

public class BaseNetController<T> where T : new()
{
    public delegate void NetMessageHandler(byte[] data);

    private Dictionary<uint, NetMessageHandler> proto_by_handler_dic_ = new Dictionary<uint, NetMessageHandler>();
    private static T instance_;

    public static T Instance
    {
        get
        {
            if (instance_ == null)
            {
                instance_ = new T();
            }
            return instance_;
        }
    }

    public virtual bool Initalized()
    {
        return true;
    }

    public void RegisterNetMessageHandler(uint proto_id,NetMessageHandler handler)
    {
        proto_by_handler_dic_.Add(proto_id, handler);
    }

    public void ReceiveMessage(network.Message message)
    {
        if(proto_by_handler_dic_.ContainsKey(message.ProtoId))
        {
            byte[] data = new byte[message.Size];
            message.Read(ref data, message.Size);

            proto_by_handler_dic_[message.ProtoId](data);
        }
    }

}
