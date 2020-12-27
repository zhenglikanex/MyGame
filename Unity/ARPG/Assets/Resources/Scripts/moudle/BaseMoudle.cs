using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public abstract class BaseMoudle : IListener,ISender
{
    public virtual void Load()
    {

    }

    public void AddListener(string message_type, MessageManager.MessageHandler handler)
    {
        MessageManager.Instance.AddListener(this, message_type, handler);
    }
    public void RemoveListener(string message_type)
    {
        MessageManager.Instance.RemoveListener(this, message_type);
    }

    public void Send(Message message)
    {
        MessageManager.Instance.Dispatch(message);
    }

}
