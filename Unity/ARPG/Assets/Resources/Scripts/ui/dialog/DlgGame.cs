using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
public class DlgGame : NormalUI
{
    private Button btn_start_;
    private Button btn_end_;
    private Text text_ping_;

    // Start is called before the first frame update
    void Start()
    {
        btn_start_ = transform.Find("btn_start").GetComponent<Button>();
        btn_end_ = transform.Find("btn_end").GetComponent<Button>();
        text_ping_ = transform.Find("text_ping").GetComponent<Text>();

        btn_start_.onClick.AddListener(() =>
        {
            BattleController.Instance.StartBattle();
            UpdateBtnState();
        });

        btn_end_.onClick.AddListener(() =>
        {
            BattleController.Instance.EndBattle();
            UpdateBtnState();
        });

        AddListener("ping_update", (Message msg) =>
         {
             var ping = msg.PopData<int>();
             text_ping_.text = string.Format("ping:{0}", ping);
         });

        UpdateBtnState();
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    void UpdateBtnState()
    {
        btn_start_.gameObject.SetActive(!BattleController.Instance.IsBattleing);
        btn_end_.gameObject.SetActive(BattleController.Instance.IsBattleing);
    }
}
