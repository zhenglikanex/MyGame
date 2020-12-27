using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public interface IListener
{
    void AddListener(string message_type,MessageManager.MessageHandler handler);
    void RemoveListener(string message_type);
}