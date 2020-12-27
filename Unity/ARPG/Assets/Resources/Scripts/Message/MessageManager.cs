using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MessageManager : Singleton<MessageManager>
{
    public delegate void MessageHandler(Message message);

     //监听者对应的消息处理
    private Dictionary<IListener,Dictionary<string, MessageHandler>> listener_by_message_handers_dic_
        = new Dictionary<IListener, Dictionary<string, MessageHandler>>();

    public void AddListener(IListener listener,string message_type,MessageHandler handler)
    {
        if(!listener_by_message_handers_dic_.ContainsKey(listener))
        {
            listener_by_message_handers_dic_.Add(listener, new Dictionary<string, MessageHandler>());
        }

        Dictionary<string, MessageHandler> message_handlers = listener_by_message_handers_dic_[listener];
        if(message_handlers.ContainsKey(message_type))
        {
            message_handlers.Add(message_type, handler);
        }
        else
        {
            message_handlers[message_type] = handler;
        }
    }

    public void RemoveListener(IListener listener,string message_type)
    {
        if(!listener_by_message_handers_dic_.ContainsKey(listener))
        {
            return;
        }

        Dictionary<string, MessageHandler> message_handlers = listener_by_message_handers_dic_[listener];
        message_handlers.Remove(message_type);
    }

    public void RemoveListener(IListener listener)
    {
        listener_by_message_handers_dic_.Remove(listener);
    }

    public void Dispatch(Message message)
    {
        foreach(Dictionary<string, MessageHandler> message_handlers in listener_by_message_handers_dic_.Values)
        {
            if(message_handlers.ContainsKey(message.MessageType))
            {
                message_handlers[message.MessageType](message);
            }
        }
    }
}