using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PoolObject : MonoBehaviour {

    public bool IsUse = false;
    public string OwnerPoolName = "";

    public void Init()
    {
        OnInit();
    }

    public virtual void OnInit()
    {

    }
    
    public void Clear()
    {
        OnClear();
    }

    public virtual void OnClear()
    {

    }
}
