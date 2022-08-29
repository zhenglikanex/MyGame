using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;

public class MoudleManager : Singleton<MoudleManager>
{
    public Dictionary<string, BaseMoudle> moudle_dic_ = new Dictionary<string, BaseMoudle>();

    public bool Initalized()
    {
        RegisterAllMoudle();

        return true;
    }

    public void RegisterAllMoudle()
    {
        LoadMoudle<GameUserMoudle>();
    }

    public void LoadMoudle<T>() where T : BaseMoudle
    {
        Type t = typeof(T);
        BaseMoudle moudle = System.Activator.CreateInstance(t) as BaseMoudle;
        moudle.Load();

        moudle_dic_.Add(t.ToString(), moudle);
    }

    public T Get<T>() where T : BaseMoudle
    {
        string key = typeof(T).ToString();
        if (!moudle_dic_.ContainsKey(key))
        {
            return null;
        }
        return moudle_dic_[key] as T;
    }
}
