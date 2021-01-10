using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraController : MonoBehaviour {
    private Transform target_;
    public Vector3 offset_;

    // Use this for initialization
    void Start () {
    }
	
	// Update is called once per frame
	void Update () {
        UpdateCamPos();
	}

    private void UpdateCamPos()
    {
        if (target_)
        {
            transform.localPosition = target_.localPosition + offset_;
        }
    }

    public void LockTarget(Transform trans,bool rotate)
    {
        target_ = trans;

        // 设置摄像机坐标
        UpdateCamPos();

        // 是否需要旋转
        if (rotate)
        {
            // 绕对象的up旋转180
            transform.RotateAround(target_.position, Vector3.up, 180);

            //重新计算偏移
            offset_ = transform.localPosition - target_.localPosition;
        }
    }
}
