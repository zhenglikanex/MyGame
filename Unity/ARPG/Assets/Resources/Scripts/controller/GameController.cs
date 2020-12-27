using System.Collections;
using System.Collections.Generic;
using UnityEngine;

//这个控制器将在放在一个预启动场景中,保证整次游戏只会运行一次
//这个类保证在整个游戏运行过程中一直存在
public class GameController : MonoBehaviour
{

    private static GameController instance_;

    public network.NetClient NetClient_
    {
        get;
        set;
    }

    public FSMSystem FSMSystem_
    {
        get;
        set;
    }

    public static GameController Instance
    {
        get
        {
            return instance_;
        }
    }

    private void Awake()
    {
#if UNITY_EDITOR
        //CPlusPlusBridge.LoadDLL("MyGameClient");
#endif

        instance_ = this;
    }

    // Use this for initialization
    void Start()
    {
        Object.DontDestroyOnLoad(gameObject);

        InitGameFSM();

        Random.InitState(10000);
    }

    public void InitGameFSM()
    {
        FSMSystem_ = new FSMSystem();

        FSMSystem_.RegisterStateFactory(StateID.kGameStartState, () => 
        {
            GameStartState state = new GameStartState();
            state.AddTransition(Transition.kGameToBattleTrans, StateID.kGameBattleState);

            return state;
        });

        FSMSystem_.RegisterStateFactory(StateID.kGameBattleState, () =>
         {
             GameBattleState state = new GameBattleState();
             return state;
         });

        // 初始化Start状态
        FSMSystem_.InitCurState(StateID.kGameStartState);
    }

    public void DoTransition(Transition trans)
    {
        FSMSystem_.DoTransition(trans);
    }

    // Update is called once per frame
    void Update()
    {
        if(NetClient_ != null)
        {
            NetClient_.Update();
        }
    }

    public void NetMessageHandler(network.Message message)
    {
        //if(message.MoudleId == (ushort)NetMoudleID.kUserMoudle)
        //{
        //    GameUserControl.Instance.ReceiveMessage(message);
        //}
        //else if(message.MoudleId == (ushort)NetMoudleID.kMatchingMoudle)
        //{
        //    GameMatchingControl.Instance.ReceiveMessage(message);
        //}
        //else if(message.MoudleId == (ushort)NetMoudleID.kBattleMoudle)
        //{
        //    GameBattleController.Instance.ReceiveMessage(message);
        //}
    }

    void OnApplicationQuit()
    {

    }
}