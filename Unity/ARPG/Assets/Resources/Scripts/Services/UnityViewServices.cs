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
                views.Add(view.Id, view);
            }
            go.transform.SetParent(GameObject.Find("World").transform);

            // todo:暂时加载这里
            var controller = Camera.main.gameObject.GetComponent<CameraController>();
            controller.LockTarget(go.transform, false);

            return view;
        }

        Debug.LogError(string.Format("CreateViewError:{}", asset));
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

    public void UpdateRotation(int id,float x,float y,float z, float w)
    {
        if (views.ContainsKey(id))
        {
            var view = views[id];
            view.UpdateRotation(x, y, z,w);
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

    public void MoveRotation(int id,float x,float y,float z, float w)
    {
        if (views.ContainsKey(id))
        {
            var view = views[id];
            view.MoveRotation(x, y, z,w);
        }
    }

    public void PlayAnim(int id,string str)
    {
        if(views.ContainsKey(id))
        {
            var view = views[id];
            view.PlayAnim(str);
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
