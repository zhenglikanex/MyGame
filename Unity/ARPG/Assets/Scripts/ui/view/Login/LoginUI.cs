using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
public class LoginUI : NormalUI {

    private Text user_name_text_;
    private Text password_text_;
    private Button register_button_;
    private Button login_button_;

	// Use this for initialization
	void Start () {
        Debug.Log("this is start");

        user_name_text_ = this.transform.Find("Bg/UserNameInput/Text").GetComponent<Text>();
        password_text_ = this.transform.Find("Bg/PasswordInput/Text").GetComponent<Text>();
        register_button_ = this.transform.Find("Bg/RegisterButton").GetComponent<Button>();
        login_button_ = this.transform.Find("Bg/LoginButton").GetComponent<Button>();

        register_button_.onClick.AddListener(() =>
        {
            Debug.Log("111111111111111111");
            //UIManager.Instance.OpenUI<RegisterAccountUI>();
        });

        login_button_.onClick.AddListener(() =>
        {
            //GameUserControl.Instance.RequestLogin(user_name_text_.text, password_text_.text);
        });
	}
	
	// Update is called once per frame
	void Update () {
		
	}
}
