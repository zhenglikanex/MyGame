using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
public class GameLoginState : FSMState
{
    public override void Enter()
    {
        //切换到正式的场景
        //SceneManager.LoadScene(Global.ScenePath + "Main", LoadSceneMode.Single);
    }

    public override void Leave()
    {
        // 关闭所有界面
        UIManager.Instance.CloseAll();
    }

    public override void InputHandler(object input)
    {
    }
}
