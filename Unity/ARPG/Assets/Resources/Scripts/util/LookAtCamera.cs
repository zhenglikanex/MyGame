using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LookAtCamera : MonoBehaviour {

	// Use this for initialization
	void Start () {
        
    }
	
	// Update is called once per frame
	void Update () {
        Vector3 target = Camera.main.transform.position;
        this.transform.LookAt(target, Camera.main.transform.rotation * Vector3.up);
        transform.Rotate(new Vector3(0, 1, 0),180);
        //target.z = transform.position.z + 100;

        //transform.Rotate(0, 90, 0);
    }
}
