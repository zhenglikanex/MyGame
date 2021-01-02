using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ChanceWeapon : MonoBehaviour {

	public GameObject weapon_01;
	public GameObject weapon_02;


	void Start ()
	{
		
		weapon_01.SetActive(false);
		weapon_02.SetActive(true);
	}



	// Update is called once per frame
	void Update () {

		if (Input.GetKeyDown ("0")){
			switchWeaponsPlease();

			}	
			


		}	

	void switchWeaponsPlease(){
		if(weapon_01.activeSelf){
			weapon_01.SetActive(false);
			weapon_02.SetActive(true);

		}
		else
		{
			weapon_01.SetActive(true);
			weapon_02.SetActive(false);

		}
			
	}
}