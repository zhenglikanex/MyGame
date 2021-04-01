using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using XNode;
using System;

namespace SkillEditor
{
    public class Entry : Node
    {
        [Serializable]
        public class Empty { }

        [Output(connectionType = ConnectionType.Override)] public Empty exit;
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