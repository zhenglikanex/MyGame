using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Singleton<T> where T : new()
{
    
    private static T instance_;

    public static T  Instance
    {
        get
        {
            if(instance_ == null)
            {
                instance_ = new T();
            }
            return instance_;
        }
    }
    
}