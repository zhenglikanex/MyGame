using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Google.Protobuf;
using System;

public class BattleController : SceneGameObjectSingleton<BattleController>
{

    public GameObject go;
    public uint MyId = 0;
    public int count = 0;
    public bool IsBattleing
    {
        get;
        set;
    }

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
            CPlusPlusBridge.UpdateGame(Time.deltaTime);
            ++count;
            if (!go.activeSelf && count > 2)
            {
                go.SetActive(true);
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
            Proto.GamePlayerInfos player_infos = new Proto.GamePlayerInfos();
            player_infos.PlayerInfos.Add(player);
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
            Proto.GameCommond cmd = new Proto.GameCommond();
            cmd.XAxis = Input.GetAxis("Horizontal");
            cmd.YAxis = Input.GetAxis("Vertical");
            cmd.Attack = false;
            cmd.Jump = false;

            Proto.GameCommondGroup group = new Proto.GameCommondGroup();
            group.Commonds.Add(MyId, cmd);

            var data = group.ToByteArray();
            CPlusPlusBridge.GameInput(data, data.Length);
        }
    }
}
