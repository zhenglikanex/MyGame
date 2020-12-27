using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
public class GameBattleState : FSMState
{
    public override void Enter()
    {
        //GameObject prefab = Resources.Load<GameObject>(Global.PrefabPath + "FlyText");
        //if (prefab)
        //{
        //    GameObjectPool pool = PoolManager.Instance.RegisterPool("FlyText", prefab);
        //    pool.AlllocCapacity(20);
        //}
        // 加载战斗场景
        SceneManager.LoadScene("Resources/Scenes/Battle", LoadSceneMode.Single);
    }

    public override void Leave()
    {
        //PoolManager.Instance.RemovePool("FlyText");
    }

    public override void InputHandler(object input)
    {
    }
}
