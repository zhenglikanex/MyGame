using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameObjectPool : MonoBehaviour
{
    private GameObject prefab_;

    private Queue<GameObject> caches_ = new Queue<GameObject>();

    public string Name
    {
        get;
        set;
    }

    public int Capacity
    {
        get;
        set;
    }

    public void Init(string name, GameObject prefab)
    {
        Name = name;
        prefab_ = prefab;
    }

    public GameObject Create()
    {
        GameObject go = caches_.Dequeue();
        if(!go)
        {
            go = GameObject.Instantiate<GameObject>(prefab_);
        }
        go.SetActive(true);
        go.GetComponent<PoolObject>().Init();

        return go;
    }

    public void Destory(GameObject go)
    {
        // 对象是否有效，
        if (!go)
        {
            return;
        }

        //是否同一个prefab实例
        PoolObject pool_cache_obj = go.GetComponent<PoolObject>();
        if(!pool_cache_obj || pool_cache_obj.OwnerPoolName != Name)
        {
            Debug.Log("GameObject不属于" + Name);
            return;
        }

        if (caches_.Count >= Capacity)
        {
            Object.Destroy(go);
        }
        else
        {
            // 外部必须确保go恢复初始状态
            go.GetComponent<PoolObject>().Clear();
            go.SetActive(false);
            go.transform.SetParent(this.transform);
            caches_.Enqueue(go);
        }
    }

    public void AlllocCapacity(int capacity)
    {
        if (capacity > Capacity)
        {
            // 扩张
            for (int i = Capacity; i < capacity; ++i)
            {
                GameObject go = GameObject.Instantiate(prefab_);
                go.transform.SetParent(this.transform);
                go.SetActive(false);

                go.AddComponent<PoolObject>().OwnerPoolName = Name;

                caches_.Enqueue(go);
            }

            Capacity = capacity;
        }
        else if (capacity < Capacity)
        {
            //收缩
            for(int i = Capacity;i > capacity; --i)
            {
                GameObject go = caches_.Dequeue();
                if(go)
                {
                    Object.Destroy(go);
                }
            }

            Capacity = capacity;
        }
    }

    public void Clear()
    {
        Capacity = 0;
        foreach (GameObject go in caches_)
        {
            Object.Destroy(go);
        }
        caches_.Clear();
    }
}
