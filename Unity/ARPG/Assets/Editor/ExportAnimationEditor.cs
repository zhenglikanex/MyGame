using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using System;
using System.IO;
using LitJson;

public class RootMotion
{
    public float time;
    public Vector3 velocity;
    public Vector3 angularVelocity;
}

public class Bone
{
    public Matrix4x4 transform;
}

public class Skeleton
{
    public float time;
    public Dictionary<string, Bone> bones = new Dictionary<string, Bone>();
}

public class ConfigAnimationClip
{
    public float length;
    public List<RootMotion> rootMotions = new List<RootMotion>();
    public List<Skeleton> skeletons = new List<Skeleton>();
}

public class ConfigAnimation
{
    public Dictionary<string, ConfigAnimationClip> clips = new Dictionary<string, ConfigAnimationClip>();
}

public class ExportAnimationEditor : EditorWindow
{
    public ExportAnimationEditorConfig config;
    public SerializedObject so;

    

    [MenuItem("Tools/AnimationExport")]
    static public void Open()
    {
        var window = GetWindow<ExportAnimationEditor>();
        window.LoadConfig();
    }

    public void LoadConfig()
    {
        config = AssetDatabase.LoadAssetAtPath<ExportAnimationEditorConfig>("Assets/Editor/ExportAnimationEditor.asset");
        if (config == null)
        {
            AssetDatabase.CreateAsset(CreateInstance<ExportAnimationEditorConfig>(), "Assets/Editor/ExportAnimationEditor.asset");
            AssetDatabase.Refresh();
            config = AssetDatabase.LoadAssetAtPath<ExportAnimationEditorConfig>("Assets/Editor/ExportAnimationEditor.asset");
        }
        so = new SerializedObject(config);
    }

    public void OnGUI()
    {
        EditorGUILayout.LabelField(new GUIContent("导出列表"));
        EditorGUILayout.PropertyField(so.FindProperty("FrameRate"), true);
        EditorGUILayout.PropertyField(so.FindProperty("animators"), true);

        var my_so = new SerializedObject(this);
        my_so.ApplyModifiedProperties();
        my_so.Update();

        if (GUILayout.Button("加载"))
        {
            Load();
        }

        if (GUILayout.Button("导出"))
        {
            Export();
        }

        so.ApplyModifiedProperties();
        so.Update();
    }

    public void Load()
    {
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
            var target = AssetDatabase.LoadAssetAtPath<Animator>(asset);
            if (target != null)
            {
                foreach (var e in config.animators)
                {
                    if (e.target == target)
                    {
                        target = null;
                        return;
                    }
                }

                var animator = Instantiate(target);
                var controller = animator.runtimeAnimatorController as UnityEditor.Animations.AnimatorController;
                if (controller != null)
                {
                    var item = new EditorAnimation();
                    item.target = target;
                    foreach (var state in controller.layers[0].stateMachine.states)
                    {
                        var info = new AnimationInfo();
                        
                        info.name = state.state.name;
                        info.step = 0.1f;
                        info.isBlendTree = state.state.motion.GetType() == typeof(UnityEditor.Animations.BlendTree);
                        if(info.isBlendTree)
                        {
                            var blendTree = state.state.motion as UnityEditor.Animations.BlendTree;
                            if(blendTree)
                            {
                                info.param = blendTree.blendParameter;
                            }
                        }
                        item.animations.Add(info);
                    }
                    config.animators.Add(item);
                }
                DestroyImmediate(animator.gameObject);
            }
        }
    }
    public void Export()
    {
        string[] exportBoneNames =
{
            "LeftWeapon",
            "RightWeapon"
        };

        ReigsterJsonParse();

        float dt = 1.0f / config.FrameRate;

        if (!Directory.Exists("Assets/Resources/Config/Anim/"))
        {
            Directory.CreateDirectory("Assets/Resources/Config/Anim/");
        }

        foreach(var item in config.animators)
        {
            ConfigAnimation cfgAnimation = new ConfigAnimation();

            var animator = Instantiate(item.target);
            animator.transform.position = Vector3.zero;
            animator.applyRootMotion = true;
            Scheduler.Instance.DelayCall(() => {
                foreach (var animation in item.animations)
                {
                    if (animation.isBlendTree)
                    {
                        float step = Math.Max(animation.step, 0.01f);
                        for (float param = animation.min; param < animation.max + step; param += step)
                        {
                            param = (float)Math.Round(param, 1);
                            ConfigAnimationClip clip = new ConfigAnimationClip();

                            animator.transform.position = Vector3.zero;
                            animator.SetFloat(animation.param, param);
                            animator.Play(animation.name, 0, 0);
                            animator.Update(0);    //多更新一个dt去掉0的情况

                            var state = animator.GetCurrentAnimatorStateInfo(0);
                            clip.length = state.length;

                            float time = 0.0f;
                            do
                            {

                                // rootmotion
                                RootMotion rootMotion = new RootMotion();
                                rootMotion.time = time;
                                rootMotion.velocity = animator.velocity;
                                rootMotion.angularVelocity = animator.angularVelocity;
                                clip.rootMotions.Add(rootMotion);

                                Debug.Log(string.Format("{0}   {1}", state.length, animator.GetCurrentAnimatorStateInfo(0).length));

                                //bones
                                var skeleton = new Skeleton();
                                skeleton.time = time;
                                foreach (var name in exportBoneNames)
                                {
                                    var go = GetGameObject(animator.gameObject, name);
                                    if (go != null)
                                    {
                                        var bone = new Bone();
                                        bone.transform = go.transform.parent.localToWorldMatrix.transpose;
                                        skeleton.bones.Add(name, bone);
                                    }
                                }
                                clip.skeletons.Add(skeleton);

                                animator.Update(dt);

                                time += dt;

                            } while (time < state.length);

                            //如果没有有效的rootmtion值删除所有数据
                            bool flag = false;
                            foreach (var rootMotion in clip.rootMotions)
                            {
                                var velocity = rootMotion.velocity;
                                var angularVelocity = rootMotion.angularVelocity;
                                if (velocity.x > 0.0001f || velocity.y > 0.0001f || velocity.z > 0.0001f || angularVelocity.x > 0.0001f || angularVelocity.y > 0.0001f || angularVelocity.z > 0.0001f)
                                {
                                    flag = true;
                                    break;
                                }
                            }

                            if (!flag)
                            {
                                clip.rootMotions.Clear();
                            }

                            cfgAnimation.clips.Add(string.Format("{0}|{1:F1}", animation.name, param), clip);
                        }
                    }
                    else
                    {
                        ConfigAnimationClip clip = new ConfigAnimationClip();

                        animator.transform.position = Vector3.zero;
                        animator.Play(animation.name, 0, 0);
                        animator.Update(0);    //多更新一个dt去掉0的情况

                        var state = animator.GetCurrentAnimatorStateInfo(0);
                        clip.length = state.length;

                        float time = 0.0f;
                        do
                        {
                            RootMotion rootMotion = new RootMotion();
                            rootMotion.time = time;
                            rootMotion.velocity = animator.velocity;
                            rootMotion.angularVelocity = animator.angularVelocity;
                            clip.rootMotions.Add(rootMotion);

                            //bones
                            var skeleton = new Skeleton();
                            skeleton.time = time;
                            foreach (var name in exportBoneNames)
                            {
                                var go = GetGameObject(animator.gameObject, name);
                                if (go != null)
                                {
                                    var bone = new Bone();
                                    bone.transform = go.transform.parent.localToWorldMatrix.transpose;
                                    if (animation.name == "skill100010")
                                    {
                                        var pos = go.transform.position;
                                    }

                                    skeleton.bones.Add(name, bone);
                                }
                            }
                            clip.skeletons.Add(skeleton);

                            animator.Update(dt);

                            time += dt;

                        } while (time < state.length);

                        //如果没有有效的rootmtion值删除所有数据
                        bool flag = false;
                        foreach (var rootMotion in clip.rootMotions)
                        {
                            var velocity = rootMotion.velocity;
                            var angularVelocity = rootMotion.angularVelocity;
                            Debug.Log(velocity.z);
                            if (velocity.x > 0.0001f || velocity.y > 0.0001f || velocity.z > 0.0001f || angularVelocity.x > 0.0001f || angularVelocity.y > 0.0001f || angularVelocity.z > 0.0001f)
                            {
                                flag = true;
                                break;
                            }
                        }

                        if (!flag)
                        {
                            clip.rootMotions.Clear();
                        }

                        cfgAnimation.clips.Add(animation.name, clip);
                    }
                }

                var json = JsonMapper.ToJson(cfgAnimation);
                FileStream f = new FileStream(string.Format("Assets/Resources/Config/Anim/{0}.json", item.target.gameObject.name), FileMode.Create, FileAccess.Write);
                StreamWriter stream = new StreamWriter(f);
                stream.Write(json);
                stream.Close();
            }, 1);
            

            //DestroyImmediate(animator.gameObject);
        }
    }

    public static void ReigsterJsonParse()
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

    static GameObject GetGameObject(GameObject go, string name)
    {
        var children = go.GetComponentsInChildren<Transform>();
        foreach (var child in children)
        {
            if (child.gameObject.name == name)
            {
                return child.gameObject;
            }
        }

        return null;
    }
}
