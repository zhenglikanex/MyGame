using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PoolManager : GlobalGameObjectSingleton<PoolManager> {
    
    // prefab对应的具体对象池
    private Dictionary<string,GameObjectPool> name_by_pool_dic_ = new Dictionary<string,GameObjectPool>();

	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		
	}

    // 注册对象池
    public GameObjectPool RegisterPool(string name,GameObject prefab)
    {
        if (name_by_pool_dic_.ContainsKey(name))
        {
            return null;
        }

        GameObject go = new GameObject();
        go.name = name + "Pool";
        go.transform.SetParent(this.transform);


        GameObjectPool pool = go.AddComponent<GameObjectPool>();
        pool.Init(name, prefab);
        name_by_pool_dic_.Add(name, pool);

        return pool;
    }

    // 移除对象池
    public void RemovePool(string name)
    {
        if(name_by_pool_dic_.ContainsKey(name))
        {
            GameObjectPool pool = name_by_pool_dic_[name];
            pool.Clear();

            GameObject.Destroy(pool.gameObject);

            name_by_pool_dic_.Remove(name);
        }
        
    }

    public GameObjectPool GetPoolByName(string name)
    {
        if(name_by_pool_dic_.ContainsKey(name))
        {
            return name_by_pool_dic_[name];
        }
        return null;
    }

    // 创建对象
    public GameObject Create(string name)
    {
        if(name_by_pool_dic_.ContainsKey(name))
        {
            return name_by_pool_dic_[name].Create();
        }
        return null;
    }

    // 销毁对象
    public void Destroy(GameObject go)
    {
        PoolCacheObj pool_cache_obj = go.GetComponent<PoolCacheObj>();
        if (!pool_cache_obj)
        {
            Object.Destroy(go);
        }
        else
        {
            GameObjectPool pool = GetPoolByName(pool_cache_obj.OwnerPoolName);
            if(pool)
            {
                pool.Destory(go);
            }
        }
    }
}
