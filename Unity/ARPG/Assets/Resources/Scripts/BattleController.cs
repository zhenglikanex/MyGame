using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Google.Protobuf;
using System;

enum KeyStatus
{
    kDown,
    kUp
}

public class BattleController : SceneGameObjectSingleton<BattleController>
{
    public uint MyId { get; set; }

    public bool IsBattleing
    {
        get;
        set;
    }

    private Dictionary<KeyCode, List<string>> abilityBindInfos = new Dictionary<KeyCode, List<string>>();
    private Dictionary<KeyCode, KeyStatus> keyStatusDic = new Dictionary<KeyCode, KeyStatus>();

    void Awake()
    {
        IsBattleing = false;

        UIManager.Instance.OpenUI<DlgGame>();

        
        {
            List<string> abilities = new List<string>();
            abilities.Add("GA_Roll");
            abilityBindInfos.Add(KeyCode.LeftControl, abilities);
        }

        {
            List<string> abilities = new List<string>();
            abilities.Add("GA_Jump");
            abilities.Add("GA_DoubleJump");
            abilityBindInfos.Add(KeyCode.Space, abilities);
        }

        {
            List<string> abilities = new List<string>();
            abilities.Add("GA_Sprint");
            abilityBindInfos.Add(KeyCode.LeftShift, abilities);
        }

        {
            List<string> abilities = new List<string>();
            abilities.Add("GA_LightAttackA");
            abilities.Add("GA_LightAttackB");
            abilities.Add("GA_LightAttackC");
            abilities.Add("GA_DashLightAttack");
            abilities.Add("GA_JumpAttackA");
            abilities.Add("GA_JumpAttackB");
            abilities.Add("GA_JumpAttackC");
            abilityBindInfos.Add(KeyCode.J, abilities);
        }

        {
            List<string> abilities = new List<string>();
            abilities.Add("GA_HeavyAttackA");
            abilities.Add("GA_HeavyAttackB");
            abilities.Add("GA_HeavyAttackC");
            abilities.Add("GA_HeavyAttackD");
            abilities.Add("GA_DashHeavyAttack");
            abilityBindInfos.Add(KeyCode.K, abilities);
        }
    }

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        if(IsBattleing)
        {
            //Debug.Log(string.Format("deltaTime {0}", Time.deltaTime));
            CPlusPlusBridge.UpdateGame(Time.deltaTime / 10);

            foreach(var key in abilityBindInfos.Keys)
            {
                if(Input.GetKeyDown(key))
                {
                    keyStatusDic.Add(key, KeyStatus.kDown);
                }

                if(Input.GetKeyUp(key))
                {
                    keyStatusDic.Add(key, KeyStatus.kUp);
                }
            }
        }
    }

    private void OnDestroy()
    {
        EndBattle();
    }

    public void StartBattle()
    {
        if(!IsBattleing)
        {
            CPlusPlusBridge.JoinMatch();

            IsBattleing = true;
        }
    }

    public void EndBattle()
    {
        if(IsBattleing)
        {
            CPlusPlusBridge.DestoryGame();
            IsBattleing = false;
        }
    }

    public void InputHandler()
    {
        if(IsBattleing)
        {
            DebugManager.Instance.Clear();
            Proto.CS2CPlusPlusCommand command = new Proto.CS2CPlusPlusCommand();
            command.XAxis = Input.GetAxis("Horizontal");
            command.YAxis = Input.GetAxis("Vertical");

            foreach(var bindInfo in abilityBindInfos)
            {
                if(keyStatusDic.ContainsKey(bindInfo.Key))
                {
                    foreach(var ability in bindInfo.Value)
                    {
                        command.Abilities.Add(ability,(uint)keyStatusDic[bindInfo.Key]);
                    }
                }
            }

            keyStatusDic.Clear();

            var data = command.ToByteArray();
            CPlusPlusBridge.GameInput(data, data.Length);
        }
    }
}
