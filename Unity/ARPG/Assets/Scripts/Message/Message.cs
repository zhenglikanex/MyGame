using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Message
{
    string message_type_;
    Queue<object> datas_ = new Queue<object>();
    public Message(string type)
    {
        message_type_ = type;
    }

    public string MessageType
    {
        get { return message_type_; }
    }    

    public void PushData<T>(T data)
    {
        datas_.Enqueue(data);
    }

    // 顺序弹出data,有可能抛出异常
    public T PopData<T>()
    {
        object data = datas_.Dequeue();
        return (T)data;
    }

    public void Send()
    {
        MessageManager.Instance.Dispatch(this);
    }
}

