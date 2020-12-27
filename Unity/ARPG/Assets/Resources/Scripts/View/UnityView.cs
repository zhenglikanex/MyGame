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

    public virtual void UpdateForward(float x,float y,float z)
    {

    }

    public virtual void MovePosition(float x,float y,float z)
    {

    }

    public virtual void MoveForward(float x,float y,float z)
    {

    }
}
