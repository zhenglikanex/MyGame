using System.Collections;
using System.Collections.Generic;
using UnityEngine;

// 基于GameObject组件的全局单例
public class GlobalGameObjectSingleton<T> : MonoBehaviour  where T : MonoBehaviour {

    private static T instance_;
    public static T Instance
    {
        get
        {
            if (instance_ == null)
            {
                // 根据类名查找对象
                GameObject obj = GameObject.Find(typeof(T).Name);
                if (!obj)
                {
                    obj = new GameObject();
                    obj.name = typeof(T).Name;
                    DontDestroyOnLoad(obj);
                }

                instance_ = obj.GetComponent<T>();

                if (instance_ == null)
                {
                    instance_ = obj.AddComponent<T>();
                }
            }
            return instance_;
        }
    }
}
