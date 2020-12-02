using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MainScene : MonoBehaviour {

	// Use this for initialization
	void Start () {
        UIManager.Instance.OpenUI<LoginUI>();
	}
	
	// Update is called once per frame
	void Update () {
		
	}
}
