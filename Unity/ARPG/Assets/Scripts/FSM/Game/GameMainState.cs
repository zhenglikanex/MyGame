using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameMainState : FSMState
{
    public override void Enter()
    {
        Debug.Log("Enter GameMainState");

        GameUserMoudle moudle = MoudleManager.Instance.Get<GameUserMoudle>();
        if (moudle != null)
        {
            // 打开主界面
            //UIManager.Instance.OpenUI<MainUI>();
        }
        else
        {
            Debug.Log("游戏错误");
        }
    }

    public override void Leave()
    {
    }

    public override void InputHandler(object input)
    {
    }
}
