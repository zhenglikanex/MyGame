using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Follow3DObject : MonoBehaviour {

    private Transform target_;
    public Vector2 offset_ = Vector2.zero;
    public RectTransform rect_trans_;
	// Use this for initialization
	void Start () {
        rect_trans_ = transform as RectTransform;
	}
	
	// Update is called once per frame
	void Update () {
        if (target_)
        {
            Vector2 pos = Camera.main.WorldToScreenPoint(target_.position);
            rect_trans_.anchoredPosition = pos - new Vector2(Screen.width / 2 ,Screen.height / 2);
            rect_trans_.anchoredPosition += offset_;
        }
    }

    public void Follow(Transform target)
    {
        target_ = target;
    }
}
