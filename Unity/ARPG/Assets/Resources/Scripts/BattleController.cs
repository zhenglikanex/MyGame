using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Google.Protobuf;
using System;

public class BattleController : SceneGameObjectSingleton<BattleController>
{

    public uint MyId = 0;
    public bool IsBattleing
    {
        get;
        set;
    }

    public int skill = 0;

    void Awake()
    {
        IsBattleing = false;

        UIManager.Instance.OpenUI<DlgGame>();
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
            if (Input.GetKeyDown("r"))
            {
                CPlusPlusBridge.GameRollback();
            }

            //Debug.Log(string.Format("deltaTime {0}", Time.deltaTime));
            CPlusPlusBridge.UpdateGame(Time.deltaTime);

            if(skill == 0)
            {
                skill = Input.GetKeyDown("j") ? 100010 : 0;
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
            //Proto.GamePlayerInfos player_infos = new Proto.GamePlayerInfos();
            //for (uint i =0;i<40;++i)
            //{
            //    Proto.PlayerInfo player = new Proto.PlayerInfo();
            //    player.Id = i;
            //    player.ActorAsset = "hero.json";
            //    player_infos.PlayerInfos.Add(player);
            //}
            //var data = player_infos.ToByteArray();
            //CPlusPlusBridge.InitGame(data, data.Length);

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
            Proto.GameCommond cmd = new Proto.GameCommond();
            cmd.XAxis = Input.GetAxis("Horizontal");
            cmd.YAxis = Input.GetAxis("Vertical");
            cmd.Skill = skill;
            cmd.Jump = false;

            Proto.GameCommondGroup group = new Proto.GameCommondGroup();
            group.Commonds.Add(MyId, cmd);

            var data = group.ToByteArray();
            CPlusPlusBridge.GameInput(data, data.Length);

            skill = 0;
        }
    }
}
