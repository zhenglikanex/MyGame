using System.Collections;
using System.Collections.Generic;
using System.Reflection;
using UnityEngine;

public abstract class BaseUI : MonoBehaviour ,IListener,ISender
{
    public abstract UIManager.UIType GetUIType();

    public virtual bool Open()
    {
        return true;
    }

    public virtual void Init(params object[] ui_params)
    {
        Debug.Log("this is UI Init");
    }

    public void Close()
    {
        UIManager.Instance.Close(this);
    }

    public virtual void OnDestroy()
    {
        MessageManager.Instance.RemoveListener(this);
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
