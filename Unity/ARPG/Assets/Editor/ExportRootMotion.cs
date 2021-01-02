using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using System;
using System.IO;
using LitJson;

public class RootMotion
{
    public Vector3 deltaPosition;
    public Quaternion deltaRotation;
}

public class ExportRootMotion : MonoBehaviour
{
    public static int FrameRate = 30;

    private static void ReigsterJsonParse()
    {
        JsonMapper.RegisterExporter<Vector2>((v, w) =>
        {
            w.WriteObjectStart();//开始写入对象

            w.WritePropertyName("x");//写入属性名
            w.Write(v.x.ToString("f3"));//写入值

            w.WritePropertyName("y");
            w.Write(v.y.ToString("f3"));

            w.WriteObjectEnd();
        });

        JsonMapper.RegisterExporter<Vector3>((v, w) =>
        {
            w.WriteObjectStart();//开始写入对象

            w.WritePropertyName("x");//写入属性名
            w.Write(v.x.ToString("f3"));//写入值

            w.WritePropertyName("y");
            w.Write(v.y.ToString("f3"));

            w.WritePropertyName("z");
            w.Write(v.z.ToString("f3"));

            w.WriteObjectEnd();
        });

        JsonMapper.RegisterExporter<Vector4>((v, w) =>
        {
            w.WriteObjectStart();//开始写入对象

            w.WritePropertyName("x");//写入属性名
            w.Write(v.x.ToString("f3"));//写入值

            w.WritePropertyName("y");
            w.Write(v.y.ToString("f3"));

            w.WritePropertyName("z");
            w.Write(v.z.ToString("f3"));

            w.WritePropertyName("w");
            w.Write(v.w.ToString("f3"));

            w.WriteObjectEnd();
        });

        JsonMapper.RegisterExporter<Matrix4x4>((v, w) =>
        {
            w.WriteObjectStart();//开始写入对象
            Type type = typeof(Matrix4x4);
            for (int i = 0; i < 4; ++i)
            {
                for (int j = 0; j < 4; ++j)
                {
                    string name = string.Format("m{0}{1}", i, j);
                    var field = type.GetField(name);
                    w.WritePropertyName(name);
                    float value = (float)field.GetValue(v);
                    w.Write(value.ToString("f3"));
                }
            }

            w.WriteObjectEnd();
        });

        JsonMapper.RegisterExporter<Quaternion>((v,w) =>
        {
            w.WriteObjectStart();
            w.WritePropertyName("x");//写入属性名
            w.Write(v.x.ToString("f3"));//写入值

            w.WritePropertyName("y");
            w.Write(v.y.ToString("f3"));

            w.WritePropertyName("z");
            w.Write(v.z.ToString("f3"));

            w.WritePropertyName("w");
            w.Write(v.w.ToString("f3"));
            w.WriteObjectEnd();
        });
    }


    // Start is called before the first frame update
    [MenuItem("Tools/ExportRootMotion")]
    private static void Exprot()
    {
        ReigsterJsonParse();
        var objects = FindObjectsOfType<ExportTarget>();
        Debug.Log("objects :" + objects.Length);

        foreach (var obj in objects)
        {
            Dictionary<string, Dictionary<float, RootMotion>> anims = new Dictionary<string, Dictionary<float, RootMotion>>();
            Dictionary<float, RootMotion> root_motions = new Dictionary<float, RootMotion>();

            var anim = obj.GetComponent<Animator>();
            anim.Update(0);
            anim.SetFloat("forward", 0);

            for (float forward = 0.0f; forward < 2.1f; forward += 0.1f)
            {
                anim.SetFloat("forward", forward);
                anim.Update(1.0f);

                RootMotion root_motion = new RootMotion();
                root_motion.deltaPosition = anim.deltaPosition;
                root_motion.deltaRotation = anim.deltaRotation;
                
                root_motions.Add(forward, root_motion);
            }

            anims.Add("Locomotion", root_motions);

            var json = JsonMapper.ToJson(anims);
            FileStream f = new FileStream(string.Format("{0}RootMotion.json",obj.gameObject.name),FileMode.Create, FileAccess.Write);
            StreamWriter stream = new StreamWriter(f);
            stream.Write(json);
            stream.Close();
        }
    }

    private static void ExportLocomotion()
    {
        
    }
}
