using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
using XNode;
using UnityEditor.Animations;

namespace SkillEditor
{
	public class Skill : Node
	{
		[Tooltip("描述")]
		public string desc = "";

		[Serializable]
		public class Empty { }

		[Input] public Empty enter;
		[Output(dynamicPortList = true,connectionType =ConnectionType.Override), Tooltip("技能切换条件")] public List<Transition> transitions = new List<Transition>();

		//public AnimationClip animClip = null;
		public AnimatorState animClip = null;
		public List<AnimEvent> animEvents = new List<AnimEvent>();
		// Use this for initialization
		protected override void Init()
		{
			base.Init();
		}

		// Return the correct value of an output port when requested
		public override object GetValue(NodePort port)
		{
			return null; // Replace this
		}

		public void OnInspectorGUI()
		{
			Debug.Log("面板的更新");
		}
	}
}