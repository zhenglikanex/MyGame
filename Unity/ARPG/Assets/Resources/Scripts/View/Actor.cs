﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Actor : UnityView
{
    private Animator animator;
    private Quaternion TargetRotation { get; set; }

    private void Awake()
    {
        animator = GetComponent<Animator>();
    }

    // Start is called before the first frame update
    void Start()
    {
        TargetRotation = transform.rotation;
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
        transform.position = pos;
    }

    public override void UpdateForward(float x, float y, float z)
    {
        Vector3 forward = new Vector3(x, y, z);
        transform.forward = forward;
    }

    public override void MovePosition(float x, float y, float z)
    {
        Vector3 pos = new Vector3(x, y, z);
        transform.position = pos;
    }

    public override void MoveForward(float x, float y, float z)
    {
        Vector3 forward = new Vector3(x, y, z);
        transform.forward = forward;
    }

    public override void PlayAnim(string str)
    {
        string[] anim_params = str.Split('|');
        if (anim_params.Length == 2)
        {
            string animName = anim_params[0];
            string animParam = anim_params[1];
            if (animName == "Locomotion")
            {
                if (!animator.GetCurrentAnimatorStateInfo(0).IsName(animName))
                {
                    animator.Play(animName);
                }
                else
                {
                    Debug.LogError("!!!!!!!");
                }
                animator.SetFloat("forward", float.Parse(animParam));
                Debug.Log(animator.GetCurrentAnimatorStateInfo(0).normalizedTime);
                Debug.Log(float.Parse(animParam));
            }
            else
            {
                animator.Play(name);
            }
        }
        else
        {
            string animName = str;
            Debug.Log(name);
            //animator.SetFloat("forward", 0);
            animator.Play(animName);
        }
    }

}
