using System.Collections;
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
        var animator = GetComponent<Animator>();
        animator.SetFloat("forward", 2.0f);
        //Debug.Log(animator.deltaPosition);
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

    public void OnAnimatorMove()
    {
        var animator = GetComponent<Animator>();
        var state = animator.GetCurrentAnimatorStateInfo(0);
        var p = state.normalizedTime - (int)state.normalizedTime;
        var time = p * state.length;
        //transform.position += animator.deltaPosition;
        Debug.Log(string.Format("time:{0},z:{1}", time, animator.velocity.z));
    }
}
