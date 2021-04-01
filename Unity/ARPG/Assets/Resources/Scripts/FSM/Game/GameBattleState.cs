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

        GameObject prefab = Resources.Load<GameObject>("Prefab/Actor/Hero");
        if (prefab)
        {
            GameObjectPool pool = PoolManager.Instance.RegisterPool("Hero", prefab);
            pool.AlllocCapacity(100);
        }

        // 加载战斗场景
        SceneManager.LoadScene("Resources/Scenes/Battle", LoadSceneMode.Single);
    }

    public override void Leave()
    {
        PoolManager.Instance.RemovePool("Hero");
    }

    public override void InputHandler(object input)
    {
    }
}
