using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PoolManager : GlobalGameObjectSingleton<PoolManager> {

    public string prefab_path = "";

    // prefab对应的具体对象池
    private Dictionary<string,GameObjectPool> name_by_pool_dic_ = new Dictionary<string,GameObjectPool>();

	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		
	}

    public GameObjectPool RegisterPool<T>() where T : PoolObject
    {
        string name = typeof(T).ToString();
        GameObject prefab = Resources.Load<GameObject>(prefab_path + name);
        if(prefab)
        {
            return RegisterPool(name,prefab);
        }
        return null;
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

    public void RemovePool<T>() where T : PoolObject
    {
        RemovePool(typeof(T).ToString());
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
   
    public GameObjectPool GamePool<T>() where T : PoolObject
    {
        return GetPoolByName(typeof(T).ToString());
    }

    public GameObjectPool GetPoolByName(string name)
    {
        if(name_by_pool_dic_.ContainsKey(name))
        {
            return name_by_pool_dic_[name];
        }
        return null;
    }

    public GameObject Create<T>() where T : PoolObject
    {
        return Create(typeof(T).ToString());
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
        PoolObject pool_cache_obj = go.GetComponent<PoolObject>();
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
