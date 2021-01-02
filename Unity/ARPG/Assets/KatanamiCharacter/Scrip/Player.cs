using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Player : MonoBehaviour {
	
	public Animator anim;
	public Rigidbody rbody;

	private float inputH;
	private float inputV;
	private bool run;

	// Use this for initialization
	void Start () 
	{
		anim = GetComponent<Animator>();
		rbody = GetComponent<Rigidbody>();
		run = false;
	}


	// Update is called once per frame
	void Update ()
	{
		if (Input.GetMouseButtonDown (0)) {
			int n = Random.Range (0, 2);

			if (n == 0) {
				anim.Play ("Dame_01", -1, 0F);
			} else {
				anim.Play ("Dame_02", -1, 0F);
			}
		}
			if (Input.GetKeyDown ("1")) {
				anim.Play ("Attack_01", -1, 0F);
			}
			if (Input.GetKeyDown ("2")) {
				anim.Play ("Attack_02", -1, 0F);
			}
			if (Input.GetKeyDown ("3")) {
				anim.Play ("Attack_03", -1, 0F);
			}
			if (Input.GetKeyDown ("4")) {
				anim.Play ("Attack_04", -1, 0F);
			}
			
			if (Input.GetKeyDown ("5")) {
				anim.Play ("Attack_05", -1, 0F);
			}
			if (Input.GetKeyDown ("6")) {
				anim.Play ("Attack_06", -1, 0F);
			}
			if (Input.GetKeyDown ("7")) {
				anim.Play ("Attack_07", -1, 0F);
			}
			if (Input.GetKeyDown ("8")) {
				anim.Play ("Death_01", -1, 0F);
			}
			if (Input.GetKeyDown ("9")) {
				anim.Play ("Death_02", -1, 0F);
			}
			if (Input.GetKeyDown ("0")) {
			anim.Play ("Idle_nonWeapon", -1, 0F);
			}
			
			if (Input.GetKeyDown ("g")) {
				anim.Play ("Crouch", -1, 0F);
			}
			if (Input.GetKeyDown ("t")) {
				anim.Play ("Crouch_Move_F", -1, 0F);
			}
			if (Input.GetKeyDown ("r")) {
				anim.Play ("Crouch_Move_L", -1, 0F);
			}
			if (Input.GetKeyDown ("y")) {
				anim.Play ("Crouch_Move_R", -1, 0F);
			}
		if(Input.GetKey(KeyCode.LeftShift)) 
		{
			run = true;
		}
		else
		{
			run = false;
		}

		if (Input.GetKey (KeyCode.Space)) 
		{
			anim.SetBool ("jump", true);
		} 
		else
		{
			anim.SetBool ("jump",false);
		}

		inputH = Input.GetAxis ("Horizontal");
		inputV = Input.GetAxis ("Vertical");

		anim.SetFloat("inputH",inputH);
		anim.SetFloat("inputV",inputV);
		anim.SetBool("run",run);


		float moveX = inputH *20f* Time.deltaTime;
		float moveZ = inputV *50f* Time.deltaTime;

		if (moveZ <= 0f) 
		{
			moveX = 0f;
		} 
		else if(run)
		{
			moveX*=3f;
			moveZ*=3f;

		}

		rbody.velocity = new Vector3(moveX,0f,moveZ);
	}
}


