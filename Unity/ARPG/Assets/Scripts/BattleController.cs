using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Google.Protobuf;
using System;
using UnityEditor;

public enum KeyStatus
{
    kDown,
    kUp
}

public class GameInput
{
    public float xAxis;       // “°∏À ‰»Î
    public float yAxis;       // “°∏À ‰»Î

    public Dictionary<string, KeyStatus> abilities = new Dictionary<string, KeyStatus>();
};

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
    private GameInput _input = new GameInput();

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

        {
            List<string> abilities = new List<string>();
            abilities.Add("GA_HuoQiu");
            abilityBindInfos.Add(KeyCode.L, abilities);
        }

        {
            List<string> abilities = new List<string>();
            abilities.Add("GA_LeiFa");
            abilityBindInfos.Add(KeyCode.O, abilities);
        }
    }

    // Start is called before the first frame update
    void Start()
    {
#if UNITY_EDITOR
        EditorApplication.playModeStateChanged += PlayModeStateChanged;
#endif
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

    public GameInput InputHandler()
    {
        if(IsBattleing)
        {
            _input.abilities.Clear();
            _input.xAxis = Input.GetAxis("Horizontal");
            _input.yAxis = Input.GetAxis("Vertical");

            foreach(var bindInfo in abilityBindInfos)
            {
                if(keyStatusDic.ContainsKey(bindInfo.Key))
                {
                    foreach(var ability in bindInfo.Value)
                    {
                        _input.abilities.Add(ability,keyStatusDic[bindInfo.Key]);
                    }
                }
            }

            keyStatusDic.Clear();

            return _input;
        }

        return null;
    }

#if UNITY_EDITOR
    static void PlayModeStateChanged(PlayModeStateChange obj)
    {
        switch (obj)
        {
            case PlayModeStateChange.ExitingPlayMode://Õ£÷π≤•∑≈¡¢º¥º‡Ã˝
                BattleController.Instance.EndBattle();
                break;
        }
    }
#endif
}
