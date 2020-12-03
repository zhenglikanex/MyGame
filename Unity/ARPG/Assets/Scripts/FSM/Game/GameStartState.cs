using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

// 这个状态只会在启动时进入一次,用来进行一些初始化处理
public class GameStartState : FSMState {
    public override void Enter()
    {

        Debug.Log("游戏启动");

        //注册界面
        UIManager.Instance.Initalized();

        //加载moudle
        MoudleManager.Instance.Initalized();

        // 初始化网络
        if (network.NetClient.Instance.Initalized())
        {
            GameController.Instance.NetClient_ = network.NetClient.Instance;

            // 注册消息委托
            network.NetClient.Instance.MessageHandler = GameController.Instance.NetMessageHandler;
            network.NetClient.Instance.ResponseHandler = network.Responser.Instance.MessageHandler;

            try
            {
                GameController.Instance.NetClient_.Connect("127.0.0.1", 5010);
            }
            catch(System.Exception e)
            {
                Debug.Log("NetClient Error:" + e.Message);
            }

            // 注册网络消息
            InitAllNetController();

            //加载配置
            LoadAllConfig();

            // 切换到登入状态
            GameController.Instance.DoTransition(Transition.kGameToLoginTrans);
            //GameUserControl.Instance.RequestRegisterAccount("zhenglikanex", "zhengli945");
        } else
        {
            Debug.LogError("网络初始化失败！");
        }
    }

    public override void Leave()
    {
    }

    public override void InputHandler(object input)
    {
    }

    void InitAllNetController()
    {
        //GameUserControl.Instance.Initalized();
        //GameMatchingControl.Instance.Initalized();
        //GameBattleController.Instance.Initalized();
    }

    void LoadAllConfig()
    {
        
    }

    void RegisterUI()
    {
        
    }

}
