using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UnityViewServices : GlobalGameObjectSingleton<UnityViewServices>
{
    [SerializeField]
    public Queue<int> old_ids = new Queue<int>();
    public Dictionary<int, UnityView> views = new Dictionary<int, UnityView>();
    
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public UnityView CreateView(string asset)
    {
        var go = PoolManager.Instance.Create(asset);
        if(go)
        {
            var view = go.GetComponent<UnityView>();
            if(view)
            {
                view.Id = GenViewId();
            }
            go.transform.SetParent(GameObject.Find("World").transform);

            return view;

        }
        return null;
    }

    public void DestroyView(int id)
    {
        if(views.ContainsKey(id))
        {
            var view = views[id];
            PoolManager.Instance.Destroy(view.gameObject);
            views.Remove(id);
        }
    }

    public void UpdatePosition(int id,float x,float y,float z)
    {
        if(views.ContainsKey(id))
        {
            var view = views[id];
            view.UpdatePosition(x, y, z);
        }
    }

    public void UpdateForward(int id,float x,float y,float z)
    {
        if (views.ContainsKey(id))
        {
            var view = views[id];
            view.UpdateForward(x, y, z);
        }
    }

    public void MovePosition(int id,float x,float y,float z)
    {
        if (views.ContainsKey(id))
        {
            var view = views[id];
            view.MovePosition(x, y, z);
        }
    }

    public void MoveForward(int id,float x,float y,float z)
    {
        if (views.ContainsKey(id))
        {
            var view = views[id];
            view.MoveForward(x, y, z);
        }
    }

    int GenViewId()
    {
        if(old_ids.Count <= 0)
        {
            return views.Count;
        }
        return old_ids.Dequeue();
    }
}
