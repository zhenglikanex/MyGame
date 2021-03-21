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
            Proto.PlayerInfo player = new Proto.PlayerInfo();
            player.Id = MyId;
            player.ActorAsset = "hero.json";
            Proto.PlayerInfo player2 = new Proto.PlayerInfo();
            player2.Id = 1;
            player2.ActorAsset = "hero.json";
            Proto.GamePlayerInfos player_infos = new Proto.GamePlayerInfos();
            player_infos.PlayerInfos.Add(player);
            player_infos.PlayerInfos.Add(player2);
            var data = player_infos.ToByteArray();
            CPlusPlusBridge.InitGame(data, data.Length);

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
