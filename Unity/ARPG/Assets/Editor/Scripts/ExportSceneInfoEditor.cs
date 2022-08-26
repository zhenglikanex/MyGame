using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using System.IO;
using System;
using LitJson;
using UnityEngine.SceneManagement;


public class ExportSceneInfoEditor : EditorWindow
{
    public enum RigidType
    {
        kStatic,
        kDynamic,
        kKinematic
    };

    public class Collision
    {

    }

    public class OBBCollision : Collision
    {
        public string type = "box";
        public Matrix4x4 transform; // Transform
        public Vector3 e;   //Positive halfwidth extents of OBB along each axis
        public bool trigger;
        public RigidType rigid_type;
    }

    public class CapsuleCollision : Collision
    {
        public string type = "capsule";
        public Matrix4x4 transform; // Transform
        public float r;      // Radius
        public float half_h;
        public bool trigger;
        public RigidType rigid_type;
    }

    public class Obstacle
    {
        public Collision collider;
        public Matrix4x4 transform;
    }

    public class SceneInfo
    {
        public List<Obstacle> obstacles = new List<Obstacle>();
    }

    [MenuItem("Tools/ExprotSceneInfo")]
    static void Export()
    {
        ExportAnimationEditor.ReigsterJsonParse();

        SceneInfo info = new SceneInfo();
        
        var world = GameObject.Find("World");
        if(world)
        {
            var colliders = world.transform.GetComponentsInChildren<Collider>();
            foreach(var collider in colliders)
            {
                var collisionInfo = GetCollisionInfo(collider);
                if(collisionInfo != null)
                {
                    Obstacle obstacle = new Obstacle();
                    obstacle.collider = collisionInfo;
                    obstacle.transform = collider.transform.localToWorldMatrix;
                    info.obstacles.Add(obstacle);
                }
            }
        }

        if (!Directory.Exists("Assets/Resources/Config/Scene/"))
        {
            Directory.CreateDirectory("Assets/Resources/Config/Scene/");
        }

        var json = JsonMapper.ToJson(info);
        FileStream f = new FileStream(string.Format("Assets/Resources/Config/Scene/{0}.json",SceneManager.GetActiveScene().name), FileMode.Create, FileAccess.Write);
        StreamWriter stream = new StreamWriter(f);
        stream.Write(json);
        stream.Close();
    }

    static Collision GetCollisionInfo(Collider collider)
    {
        if (collider.GetType() == typeof(BoxCollider))
        {
            var boxCollider = (BoxCollider)collider;

            OBBCollision collision = new OBBCollision();
            collision.transform = Matrix4x4.Translate(boxCollider.center);
            collision.e = boxCollider.size / 2.0f;
            collision.e.x *= collider.transform.lossyScale.x;
            collision.e.y *= collider.transform.lossyScale.y;
            collision.e.z *= collider.transform.lossyScale.z;
            collision.trigger = boxCollider.isTrigger;
            collision.rigid_type = RigidType.kStatic;

            return collision;
        }
        else if (collider.GetType() == typeof(CapsuleCollider))
        {
            var capsuleCollider = (CapsuleCollider)collider;
            CapsuleCollision collision = new CapsuleCollision();
            collision.r = capsuleCollider.radius;
            collision.half_h = capsuleCollider.height * 0.5f - capsuleCollider.radius;
            collision.trigger = capsuleCollider.isTrigger;
            collision.rigid_type = RigidType.kStatic;

            if (capsuleCollider.direction == 0)//x
            {
                collision.transform = Matrix4x4.TRS(capsuleCollider.center , Quaternion.identity, new Vector3(1, 1, 1));
            }
            else if (capsuleCollider.direction == 1) //y
            {
                collision.transform = Matrix4x4.TRS(capsuleCollider.center, Quaternion.Euler(0, 0, 90), new Vector3(1, 1, 1));
            }
            else if (capsuleCollider.direction == 2) //z
            {
                collision.transform = Matrix4x4.TRS(capsuleCollider.center, Quaternion.Euler(0, 90, 0), new Vector3(1, 1, 1));
            }

            return collision;
        }
        return null;
    }
}