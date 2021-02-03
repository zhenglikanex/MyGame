using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using System;



[Serializable]
public class AnimationInfo
{
    public string name;
    public float min;
    public float max;
    public float step;
    public string param;
    public bool isBlendTree;
}

[Serializable]
public class EditorAnimation
{
    public Animator target;
    public List<AnimationInfo> animations = new List<AnimationInfo>();
}

public class ExportAnimationEditorConfig : ScriptableObject
{
    public int FrameRate = 33;
    [SerializeField]
    public List<EditorAnimation> animators = new List<EditorAnimation>();
}
