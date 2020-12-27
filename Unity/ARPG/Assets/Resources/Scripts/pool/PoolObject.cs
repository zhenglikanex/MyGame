using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PoolObject : MonoBehaviour {

    public bool IsUse = false;
    public string OwnerPoolName
    {
        get;
        set;
    }

    public void Init()
    {
        IsUse = true;
        OnInit();
    }

    public virtual void OnInit()
    {

    }

    public void Clear()
    {
        OnClear();
        IsUse = false;
    }

    public virtual void OnClear()
    {

    }
}
