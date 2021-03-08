using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UnityView : PoolObject
{
    public static readonly int kInvaild = -1;
    public int Id = kInvaild;
    // Start is called before the first frame update
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {

    }

    public override void OnInit()
    {
        
    }

    public override void OnClear()
    {

    }

    public virtual void UpdatePosition(float x,float y,float z)
    {
        
    }

    public virtual void UpdateRotation(float x,float y,float z, float w)
    {

    }

    public virtual void MovePosition(float x,float y,float z)
    {

    }

    public virtual void MoveRotation(float x,float y,float z, float w)
    {

    }

    public virtual void PlayAnim(string str)
    {

    }
}
