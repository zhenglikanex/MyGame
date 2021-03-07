using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DebugManager : SceneGameObjectSingleton<DebugManager>
{
    private readonly string Path = "Prefab/Debug/BoundingBox/";
    private Dictionary<string, List<Transform>> groups_ = new Dictionary<string, List<Transform>>();

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void Clear()
    {
        foreach (var item in groups_)
        {
            foreach (var obj in item.Value)
            {
                obj.gameObject.SetActive(false);
            }
        }
    }

    public void DrawCube(Vector3 position,Quaternion rotation,Vector3 size)
    {
        var transform = GetCube();
        if(transform)
        {
            transform.parent = this.transform;
            //position.y += size.y * 0.5f;
            transform.position = position;
            transform.rotation = rotation;
            transform.localScale = size;

            Matrix4x4 mat = Matrix4x4.Rotate(rotation);
            Matrix4x4 mat2 = transform.worldToLocalMatrix;
            Debug.Log("111");
        }

        
    }

    public void DrawSphere(Vector3 position,float radius)
    {
        var transform = GetSphere();
        if (transform)
        {
            transform.parent = this.transform;
            transform.position = position;
            transform.localScale = new Vector3(radius,radius,radius);
        }
    }

    public void DrawCapsule(Vector3 position, Quaternion rotation,float height,float radius)
    {
        var transform = GetCapsule();
        if (transform)
        {
            transform.parent = this.transform;
            position.y += height * 0.5f;
            transform.position = position;
            transform.rotation = rotation;
            transform.localScale = new Vector3(radius * 2, height * 0.5f, radius * 2);
        }
    }

    private Transform GetCube()
    {
        return GetInstance("Cube");
    }

    private Transform GetSphere()
    {
        return GetInstance("Sphere");
    }

    private Transform GetCapsule()
    {
        return GetInstance("Capsule");
    }

    private Transform GetInstance(string type)
    {
        if (groups_.ContainsKey(type))
        {
            foreach (var obj in groups_[type])
            {
                if (!obj.gameObject.activeSelf)
                {
                    obj.gameObject.SetActive(true);
                    return obj;
                }
            }
        }

        GameObject prefab = Resources.Load<GameObject>(Path + type);
        GameObject go = GameObject.Instantiate(prefab);
        go.SetActive(true);

        if (groups_.ContainsKey(type))
        {
            groups_[type].Add(go.transform);
        }
        else
        {
            var group = new List<Transform>();
            group.Add(go.transform);
            groups_.Add(type, group);
        }

        return go.transform;
    }
}
