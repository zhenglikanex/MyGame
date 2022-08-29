using System.Collections;
using System.Collections.Generic;
using UnityEngine;

//协同帮助器
public class CoroutineHelper : SceneGameObjectSingleton<CoroutineHelper> {

	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		
	}

    // 开启一个协程
    void StartCoroutine(System.Func<IEnumerator> func)
    {
        StartCoroutine(func);
    }
}
