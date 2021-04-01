using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using XNode;
using System;

namespace SkillEditor
{
	public class Combo : Node
	{
        [Serializable]
        public class Empty { }

        [Input] public Empty enter;
		[Output] public Empty exit;
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
	}
}