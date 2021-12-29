using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
public class DlgGame : NormalUI
{
    private Button btn_start_;
    private Button btn_end_;
    private Text text_ping_;
    private Scrollbar hp_;
    private Scrollbar mp_;

    // Start is called before the first frame update
    void Start()
    {
        btn_start_ = transform.Find("btn_start").GetComponent<Button>();
        btn_end_ = transform.Find("btn_end").GetComponent<Button>();
        text_ping_ = transform.Find("text_ping").GetComponent<Text>();
        hp_ = transform.Find("hp").GetComponent<Scrollbar>();
        mp_ = transform.Find("mp").GetComponent<Scrollbar>();

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

        AddListener("refresh_ui", (Message msg) =>
         {
             int hp = msg.PopData<int>();
             int max_hp = msg.PopData<int>();
             int mp = msg.PopData<int>();
             int max_mp = msg.PopData<int>();

             hp_.value = hp / max_hp;
             mp_.value = mp / max_mp;
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
        hp_.gameObject.SetActive(BattleController.Instance.IsBattleing);
        mp_.gameObject.SetActive(BattleController.Instance.IsBattleing);
    }
}
