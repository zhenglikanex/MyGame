using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameScene : MonoBehaviour {

	// Use this for initialization
	void Start () {
        UIManager.Instance.OpenUI<DlgGame>();
	}
	
	// Update is called once per frame
	void Update () {
		
	}
}
