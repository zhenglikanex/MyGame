using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using XNode;
using System;
using UnityEditor;
using LitJson;
using System.IO;

namespace SkillEditor
{
    [Serializable]
    public class AnimEvent
    {
        [Tooltip("触发时间")]
        public float time;
        [Tooltip("事件名字")]
        public string eventName;
    }

    [Serializable]
    public class Condition
    {
        public enum ConditionType { kInt, kFloat, kBool, kAnim };
        [Tooltip("条件类型")]
        public ConditionType type;
        [Tooltip("条件名字")]
        public string name;

        public enum CheckTypeInt { kGreater, kLess, kEquals, kNotEquals,kGreaterAndEquals, kLessAndEquals };
        public CheckTypeInt checkTypeInt = CheckTypeInt.kGreater;
        public int valueInt = 0;

        public enum CheckTypeFloat { kGreater, kLess, kEquals, kNotEquals, kGreaterAndEquals, kLessAndEquals };
        public CheckTypeFloat checkTypeFloat = CheckTypeFloat.kGreater;
        public float valueFloat = 0.0f;

        public enum CheckTypeBool { kTrue, kFalse };
        public CheckTypeBool checkTypeBool = CheckTypeBool.kTrue;
        public bool valueBool = true;

        public Condition(ConditionType _type, string _name)
        {
            type = _type;
            name = _name;
        }

        public void Display(Rect rect)
        {
            if (type == ConditionType.kInt)
            {
                checkTypeInt = (CheckTypeInt)EditorGUI.EnumPopup(rect, new GUIContent(name), checkTypeInt);
                rect.y += EditorGUIUtility.singleLineHeight;
                rect.height = EditorGUIUtility.singleLineHeight;
                try
                {
                    valueInt = Convert.ToInt32(EditorGUI.TextField(rect, valueInt.ToString()));
                }
                catch (Exception e)
                {
                    Debug.LogError(e.ToString());
                }
            }
            else if (type == ConditionType.kFloat)
            {
                checkTypeFloat = (CheckTypeFloat)EditorGUI.EnumPopup(rect, new GUIContent(name), checkTypeFloat);
                rect.y += EditorGUIUtility.singleLineHeight;
                rect.height = EditorGUIUtility.singleLineHeight;
                try
                {
                    valueFloat = Convert.ToSingle(EditorGUI.TextField(rect, valueFloat.ToString()));
                }
                catch (Exception e)
                {
                    Debug.LogError(e.ToString());
                }
            }
            else if (type == ConditionType.kBool)
            {
                checkTypeBool = (CheckTypeBool)EditorGUI.EnumPopup(rect, new GUIContent(name), checkTypeBool);
                rect.y += EditorGUIUtility.singleLineHeight;
                if (checkTypeBool == CheckTypeBool.kTrue)
                {
                    valueBool = true;
                }
                else
                {
                    valueBool = false;
                }
            }
            else if (type == ConditionType.kAnim)
            {
                EditorGUI.LabelField(rect, "动画结束");
            }
        }

        public float GetHeight()
        {
            if (type == ConditionType.kInt)
            {
                return EditorGUIUtility.singleLineHeight * 2;
            }
            else if (type == ConditionType.kFloat)
            {
                return EditorGUIUtility.singleLineHeight * 2;
            }
            else if (type == ConditionType.kBool)
            {
                return EditorGUIUtility.singleLineHeight;
            }
            return EditorGUIUtility.singleLineHeight;
        }
    }

    [Serializable]
    public class Transition
    {
        [SerializeField]
        public List<Condition> conditions = new List<Condition>();
    }

    public class SkillGraphJson
    {
        public class State
        {
            public class Transition
            {
                public class Condition
                {
                    public string name;
                    public string type;
                    public string check_type;
                    public string value;
                }

                public string name;
                public List<Condition> conditions = new List<Condition>();
            }
            

            public class AnimEvent
            {
                public string name;
                public float time;
            }
           
            public string anim_name;
            public List<AnimEvent> anim_events = new List<AnimEvent>();
            public List<Transition> transitions = new List<Transition>();
        }

        public string entry;
        public Dictionary<string, State> states = new Dictionary<string, State>();
    }

    [CreateAssetMenu(fileName = "New Skill Graph", menuName = "SkillEditor/Skill Graph")]
    public class SkillGraph : NodeGraph
    {
        [Serializable]
        public class Parameter
        {
            [SerializeField]
            public Condition.ConditionType type;
            [SerializeField]
            public string name = "";
        }

        [SerializeField]
        public List<Parameter> parameters = new List<Parameter>();
        // 导出json配置
        public void ExportJson()
        {
            var entry = GetEntryNode();
            if(entry == null)
            {
                return;
            }
            var default_connect = entry.GetOutputPort("exit").Connection;
            if(default_connect == null || default_connect.node == null)
            {
                return;
            }

            SkillGraphJson skill_json = new SkillGraphJson();
            skill_json.entry = default_connect.node.name;
            foreach (var skill in GetSkillNode())
            {
                var state = new SkillGraphJson.State();

                if (skill.animClip)
                {
                    state.anim_name = skill.animClip.name;
                    foreach (var item in skill.animEvents)
                    {
                        var anim_event = new SkillGraphJson.State.AnimEvent();
                        anim_event.name = item.eventName;
                        anim_event.time = item.time;
                        state.anim_events.Add(anim_event);
                    }
                }

                for (int index = 0; index < skill.transitions.Count; ++index)
                {
                    var connect = skill.GetOutputPort("transitions " + index).Connection;
                    if (connect != null && connect.node != null)
                    {
                        var transition = new SkillGraphJson.State.Transition();
                        transition.name = connect.node.name;
                        foreach (var condition in skill.transitions[index].conditions)
                        {
                            var json_condition = new SkillGraphJson.State.Transition.Condition();
                            json_condition.name = condition.name;
                            if (condition.type == Condition.ConditionType.kInt)
                            {
                                json_condition.type = "int";
                                json_condition.value = condition.valueInt.ToString();
                                if (condition.checkTypeInt == Condition.CheckTypeInt.kLess)
                                {
                                    json_condition.check_type = "less";
                                }
                                else if (condition.checkTypeInt == Condition.CheckTypeInt.kGreater)
                                {
                                    json_condition.check_type = "greater";
                                }
                                else if (condition.checkTypeInt == Condition.CheckTypeInt.kEquals)
                                {
                                    json_condition.check_type = "equals";
                                }
                                else if (condition.checkTypeInt == Condition.CheckTypeInt.kNotEquals)
                                {
                                    json_condition.check_type = "not_equals";
                                }
                                else if (condition.checkTypeInt == Condition.CheckTypeInt.kGreaterAndEquals)
                                {
                                    json_condition.check_type = "greater_and_equals";
                                }
                                else if (condition.checkTypeInt == Condition.CheckTypeInt.kLessAndEquals)
                                {
                                    json_condition.check_type = "less_and_equals";
                                }
                            }
                            else if (condition.type == Condition.ConditionType.kFloat)
                            {
                                json_condition.type = "float";
                                json_condition.value = condition.valueFloat.ToString();
                                if (condition.checkTypeFloat == Condition.CheckTypeFloat.kLess)
                                {
                                    json_condition.check_type = "less";
                                }
                                else if (condition.checkTypeFloat == Condition.CheckTypeFloat.kGreater)
                                {
                                    json_condition.check_type = "greater";
                                }
                                else if (condition.checkTypeFloat == Condition.CheckTypeFloat.kEquals)
                                {
                                    json_condition.check_type = "equals";
                                }
                                else if (condition.checkTypeFloat == Condition.CheckTypeFloat.kNotEquals)
                                {
                                    json_condition.check_type = "not_equals";
                                }
                                else if (condition.checkTypeFloat == Condition.CheckTypeFloat.kGreaterAndEquals)
                                {
                                    json_condition.check_type = "greater_and_equals";
                                }
                                else if (condition.checkTypeFloat == Condition.CheckTypeFloat.kLessAndEquals)
                                {
                                    json_condition.check_type = "less_and_equals";
                                }
                            }
                            else if (condition.type == Condition.ConditionType.kBool)
                            {
                                json_condition.type = "bool";
                                json_condition.value = condition.valueBool.ToString();
                                if (condition.checkTypeBool == Condition.CheckTypeBool.kFalse)
                                {
                                    json_condition.check_type = "false";
                                }
                                else if (condition.checkTypeBool == Condition.CheckTypeBool.kTrue)
                                {
                                    json_condition.check_type = "true";
                                }
                            }
                            else if (condition.type == Condition.ConditionType.kAnim)
                            {
                                json_condition.type = "anim";
                            }
                            transition.conditions.Add(json_condition);
                        }

                        state.transitions.Add(transition);
                    }
                }
                skill_json.states.Add(skill.name, state);
            }
            var json = JsonMapper.ToJson(skill_json);
            FileStream f = new FileStream(string.Format("Assets/Resources/Config/SkillGraph/{0}.json",name), FileMode.Create, FileAccess.Write);
            StreamWriter stream = new StreamWriter(f);
            stream.Write(json);
            stream.Close();

        }

        private Entry GetEntryNode()
        {
            foreach(var node in nodes)
            {
                if(node.name == "Entry" && typeof(SkillEditor.Entry) == node.GetType())
                {
                    return node as Entry;
                }
            }

            return null;
        }

        private List<Skill> GetSkillNode()
        {
            List<Skill> skills = new List<Skill>();
            foreach(var node in nodes)
            {
                if(typeof(Skill) == node.GetType())
                {
                    skills.Add(node as Skill);
                }
            }

            return skills;
        }
    }
}