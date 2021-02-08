using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using System.IO;
using System;
using LitJson;


public class ExportActorInfoEditor : EditorWindow
{
    public class Collision
    {
        
    }

    public class OBBCollision : Collision
    {
        public string type = "obb";
        public Vector3 c;     // OBB center point
        public Matrix4x4 u; // Transform
        public Vector3 e;   //Positive halfwidth extents of OBB along each axis
    }

    public class CapsuleCollision : Collision
    {
        public string type = "capsule";
        public Vector3 a;      // Medial line segment start point
        public Vector3 b;      // Medial line segment end point
        public float r;      // Radius
    }

    public class ActorInfo
    {
        public string modelAsset;
        public string animAsset;
        public Collision bodyCollision;
        public Collision weaponCollision;
    }

    [MenuItem("Tools/ExprotActorInfo")]
    static void Export()
    {
        ExportAnimationEditor.ReigsterJsonParse();

        string bodyCollisionName = "BodyCollision";
        string weaponCollisionName = "WeaponCollision";

        //路径
        string fullPath = "Assets/Resources/Prefab/Actor/";

        List<string> assets = new List<string>();

        //获取指定路径下面的所有资源文件
        if (Directory.Exists(fullPath))
        {
            DirectoryInfo direction = new DirectoryInfo(fullPath);
            FileInfo[] files = direction.GetFiles("*", SearchOption.AllDirectories);

            Debug.Log(files.Length);

            for (int i = 0; i < files.Length; i++)
            {
                if (files[i].Name.EndsWith(".meta"))
                {
                    continue;
                }
                var full_name = files[i].FullName;
                var start = full_name.IndexOf("Assets");
                assets.Add(full_name.Substring(start, full_name.Length - start));
            }
        }

       
        foreach (var asset in assets)
        {
            var go = AssetDatabase.LoadAssetAtPath<GameObject>(asset);

            var bodyCollision = GetCollider(go, bodyCollisionName);
            var weaponCollision = GetCollider(go, weaponCollisionName);
            if (!bodyCollision || !weaponCollision)
            {
                continue;
            }
            ActorInfo info = new ActorInfo();
            info.modelAsset = go.name;
            info.animAsset = go.name + ".json";
            info.bodyCollision = GetCollisionInfo(bodyCollision);
            info.weaponCollision = GetCollisionInfo(weaponCollision);

            if (!Directory.Exists("Assets/Resources/Config/Actor/"))
            {
                Directory.CreateDirectory("Assets/Resources/Config/Actor/");
            }

            var json = JsonMapper.ToJson(info);
            FileStream f = new FileStream(string.Format("Assets/Resources/Config/Actor/{0}.json", go.gameObject.name), FileMode.Create, FileAccess.Write);
            StreamWriter stream = new StreamWriter(f);
            stream.Write(json);
            stream.Close();
        }
    }

    static Collider GetCollider(GameObject go,string name)
    {
        var children = go.GetComponentsInChildren<ExportCollisionTag>();
        foreach(var child in children)
        {
            if(child.gameObject.name == name)
            {
                return child.gameObject.GetComponent<Collider>();
            }
        }

        return null;
    }

    static Collision GetCollisionInfo(Collider collider)
    {
        if (collider.GetType() == typeof(BoxCollider))
        {
            var boxCollider = (BoxCollider)collider;
            OBBCollision collision = new OBBCollision();
            collision.c = boxCollider.center;
            collision.u = collider.gameObject.transform.worldToLocalMatrix;
            collision.e = boxCollider.size / 2.0f;

            return collision;
        }
        else if (collider.GetType() == typeof(CapsuleCollider))
        {
            var capsuleCollider = (CapsuleCollider)collider;
            CapsuleCollision collision = new CapsuleCollision();
            
            if(capsuleCollider.direction == 0)      //x
            {
                collision.a = capsuleCollider.center - Vector3.right * (capsuleCollider.height * 0.5f);
                collision.b = capsuleCollider.center + Vector3.right * (capsuleCollider.height * 0.5f);
                collision.r = capsuleCollider.radius;
            }
            else if(capsuleCollider.direction == 1) //y
            {
                collision.a = capsuleCollider.center - Vector3.up * (capsuleCollider.height * 0.5f);
                collision.b = capsuleCollider.center + Vector3.up * (capsuleCollider.height * 0.5f);
                collision.r = capsuleCollider.radius;
            }
            else if(capsuleCollider.direction == 2) //z
            {
                collision.a = capsuleCollider.center - Vector3.forward * (capsuleCollider.height * 0.5f);
                collision.b = capsuleCollider.center + Vector3.forward * (capsuleCollider.height * 0.5f);
                collision.r = capsuleCollider.radius;
            }

            return collision;
        }

        return null;
    }
}

