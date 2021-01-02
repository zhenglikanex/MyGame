using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Google.Protobuf;

public class BattleController : SceneGameObjectSingleton<BattleController>
{
    public uint MyId = 0;
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
            PlayerInfo player = new PlayerInfo();
            player.Id = MyId;
            player.ActorId = 1;
            GamePlayerInfos player_infos = new GamePlayerInfos();
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
            GameCommond cmd = new GameCommond();
            cmd.XAxis = Input.GetAxis("Horizontal");
            cmd.YAxis = Input.GetAxis("Vertical");

            cmd.Attack = false;
            cmd.Jump = false;

            GameCommondGroup group = new GameCommondGroup();
            group.Commonds.Add(MyId, cmd);

            var data = group.ToByteArray();
            CPlusPlusBridge.GameInput(data, data.Length);
        }
    }
}
