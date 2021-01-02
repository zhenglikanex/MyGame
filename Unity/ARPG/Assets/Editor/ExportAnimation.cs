using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using System;
using System.IO;

using JsonAnimtion = System.Collections.Generic.Dictionary<string, string>;

[Serializable]
public class Bone
{
    public Matrix4x4 matrix;
}

[Serializable]
public class AnimtionFrame
{
    [SerializeField]
    public Dictionary<string,Bone> bones = new Dictionary<string, Bone>();
}


public class Test
{
    public int a;
    public int b;
}

public class ExportAnimation : MonoBehaviour
{
    public static int FrameRate = 30;

    [MenuItem("Tools/ExportAnimation")]
    private static void Exprot()
    {
        var objects = FindObjectsOfType<ExportTarget>();
        Debug.Log("objects :" + objects.Length);

        Dictionary<string, JsonAnimtion> anims = new Dictionary<string, JsonAnimtion>();
        foreach (var obj in objects)
        {
            var anim = new JsonAnimtion();
            // 得到根节点到target节点的矩阵
            var animator = obj.GetComponent<Animator>();
            foreach (var name in obj.ExportAnims)
            {
                string entry = "{";
                foreach (var node in obj.Exports)
                {
                    var bone_name = node.name;
                    string json = "[";

                    animator.Play(name, 0, 0.0f);
                    animator.Update(0);
                    var info = animator.GetCurrentAnimatorStateInfo(0);
                    do
                    {
                        info = animator.GetCurrentAnimatorStateInfo(0);
                        var matrix = node.localToWorldMatrix * obj.transform.worldToLocalMatrix;
                        var j = JsonUtility.ToJson(matrix);
                        json = json + j + ",";
                        animator.Update(1.0f / FrameRate);
                    } while (info.normalizedTime < 1.0f);

                    json = json.Substring(0, json.Length - 1) + "]";
                    entry = entry + "\"" + bone_name + "\":" + json + ",";
                }

                entry = entry.Substring(0, entry.Length - 1) + "}";

                anim.Add(name, entry);
            }

            anims.Add(obj.name, anim);
        }

        string str = "{";
        foreach(var anim in anims)
        {
            str = str + "\"" + anim.Key + "\":";
            string anim_s = "{";
            foreach(var name in anim.Value)
            {
                
            }
        }
        str = str.Substring(0, str.Length - 1) + "}";
        FileStream f = new FileStream("anims.json", FileMode.Create, FileAccess.Write);
        StreamWriter stream = new StreamWriter(f);
        stream.Write(str);
        stream.Close();
        f.Close();
    }
}
