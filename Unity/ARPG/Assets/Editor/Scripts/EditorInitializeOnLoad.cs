using System;
using LitJson;
using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

[InitializeOnLoad]
public class EditorInitializeOnLoad
{
    static EditorInitializeOnLoad()
    {
        JsonMapper.RegisterExporter<Vector2>((v, w) =>
        {
            w.WriteObjectStart();
            w.WritePropertyName("x");//写入属性名
            w.Write(Math.Round(v.x, 3));//写入值

            w.WritePropertyName("y");
            w.Write(Math.Round(v.y, 3));

            w.WriteObjectEnd();
        });

        JsonMapper.RegisterExporter<Vector3>((v, w) =>
        {
            w.WriteObjectStart();
            w.WritePropertyName("x");//写入属性名
            w.Write(Math.Round(v.x, 3));//写入值

            w.WritePropertyName("y");
            w.Write(Math.Round(v.y, 3));

            w.WritePropertyName("z");
            w.Write(Math.Round(v.z, 3));
            w.WriteObjectEnd();
        });

        JsonMapper.RegisterExporter<Vector4>((v, w) =>
        {
            w.WriteObjectStart();
            w.WritePropertyName("x");//写入属性名
            w.Write(Math.Round(v.x, 3));//写入值

            w.WritePropertyName("y");
            w.Write(Math.Round(v.y, 3));

            w.WritePropertyName("z");
            w.Write(Math.Round(v.z, 3));

            w.WritePropertyName("w");
            w.Write(Math.Round(v.w, 3));
            w.WriteObjectEnd();
        });

        JsonMapper.RegisterExporter<Matrix4x4>((v, w) =>
        {
            v = v.transpose; //unity的m[i,j]对用glm中的m[j,i]
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
                    w.Write(Math.Round(value, 3));
                }
            }

            w.WriteObjectEnd();
        });

        JsonMapper.RegisterExporter<Quaternion>((v, w) =>
        {
            w.WriteObjectStart();
            w.WritePropertyName("x");//写入属性名
            w.Write(Math.Round(v.x, 3));//写入值

            w.WritePropertyName("y");
            w.Write(Math.Round(v.y, 3));

            w.WritePropertyName("z");
            w.Write(Math.Round(v.z, 3));

            w.WritePropertyName("w");
            w.Write(Math.Round(v.w, 3));
            w.WriteObjectEnd();
        });
    }
}