﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Actor : UnityView
{
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
        base.OnInit();
    }

    public override void OnClear()
    {
        base.OnClear();
    }

    public override void UpdatePosition(float x, float y, float z)
    {
        Vector3 pos = new Vector3(x, y, z);
        transform.localPosition = pos;
    }

    public override void UpdateForward(float x, float y, float z)
    {
        Vector3 forward = new Vector3(x, y, z);
        transform.forward = forward;
    }

    public override void MovePosition(float x,float y,float z)
    {
        Vector3 pos = new Vector3(x, y, z);
        transform.localPosition = pos;
    }

    public override void MoveForward(float x, float y, float z)
    {
        Vector3 forward = new Vector3(x, y, z);
        transform.forward = forward;
    }
}
