using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;
using XNode.Examples.StateGraph;
using XNodeEditor;
using UnityEditorInternal;
using XNode;
using System.Linq;

namespace SkillEditor
{
    [CustomNodeEditor(typeof(Skill))]
    public class SkillEditor : NodeEditor
    {
        public int index = 0;
        public bool IsShow= true;
        public Dictionary<object,ReorderableList> transiditions = new Dictionary<object, ReorderableList>();
        public ReorderableList animEventList = null;

        public override int GetWidth()
        {
            return 280;
        }
        public override void OnBodyGUI()
        {
            showInputPort();
            showAnimClipUI();
            showTransitionsUI();
            // Draw GUI
        }

        public void showInputPort()
        {
            foreach (XNode.NodePort prot in target.Inputs)
            {
                NodeEditorGUILayout.PortField(prot);
            }
        }

        public void showAnimClipUI()
        {
            string animFieldName = "animClip";
            string eventFieldName = "animEvents";
            var skill = serializedObject.targetObject as Skill;
            var animClip = serializedObject.FindProperty(animFieldName);
            EditorGUILayout.PropertyField(animClip, true);
            if (IsShow && skill.animClip != null)
            {

                if (skill.animClip.motion.GetType() == typeof(AnimationClip))
                {
                    var clip = skill.animClip.motion as AnimationClip;
                    GUILayout.Label("动画时长:" + clip.length);
                }
                else
                {
                    GUILayout.Label("动画时长:BlendTree");
                }
                
                GUILayout.Label("动画事件");

                if (animEventList == null)
                {
                    animEventList = new ReorderableList(skill.animEvents, null, true, true, true, true);
                    animEventList.drawHeaderCallback = (Rect rect) =>
                    {
                        string title = serializedObject.targetObject.name;
                        EditorGUI.LabelField(rect, "events");
                    };
                    animEventList.elementHeightCallback = (int index) =>
                    {
                        return EditorGUI.GetPropertyHeight(serializedObject.FindProperty(eventFieldName).GetArrayElementAtIndex(index));
                    };

                    animEventList.drawElementCallback =
                    (Rect rect, int index, bool isActive, bool isFocused) =>
                    {
                        EditorGUI.PropertyField(rect, serializedObject.FindProperty(eventFieldName).GetArrayElementAtIndex(index), true);
                    };
                    animEventList.onAddCallback = (ReorderableList rl) =>
                    {
                        skill.animEvents.Add(new AnimEvent());
                        serializedObject.ApplyModifiedProperties();
                        serializedObject.Update();

                    };
                    animEventList.onRemoveCallback = (ReorderableList list) =>
                    {
                        skill.animEvents.RemoveAt(list.index);
                        serializedObject.ApplyModifiedProperties();
                        serializedObject.Update();
                    };
                }
                animEventList.DoLayoutList();
            }
        }

        public void showTransitionsUI()
        {
            string fieldName = "transitions";
            var skill = serializedObject.targetObject as Skill;
            var transitions = skill.transitions;

            if (IsShow)
            {
                GUILayout.Label("转变列表");
                if (GUILayout.Button("添加"))
                {
                    // Add dynamic port postfixed with an index number
                    string newName = fieldName + " 0";
                    int i = 0;
                    while (skill.HasPort(newName)) newName = fieldName + " " + (++i);
                    skill.AddDynamicOutput(typeof(Transition), Node.ConnectionType.Override, XNode.Node.TypeConstraint.None, newName);
                    serializedObject.Update();
                    EditorUtility.SetDirty(skill);
                    skill.transitions.Add(new Transition());
                    serializedObject.ApplyModifiedProperties();
                }

                Rect r = EditorGUILayout.BeginVertical();
                int index = 0;
                foreach (var transition in transitions)
                {
                    var list = CreateList(transition.conditions, index);
                    r.y += list.GetHeight() + 5;
                    if (GUILayout.Button("删除", GUILayout.Width(100)))
                    {
                        var indexedPorts = skill.DynamicPorts.Select(x =>
                        {
                            string[] split = x.fieldName.Split(' ');
                            if (split != null && split.Length == 2 && split[0] == fieldName)
                            {
                                int i = -1;
                                if (int.TryParse(split[1], out i))
                                {
                                    return new { index = i, port = x };
                                }
                            }
                            return new { index = -1, port = (XNode.NodePort)null };
                        }).Where(x => x.port != null);
                        List<XNode.NodePort> dynamicPorts = indexedPorts.OrderBy(x => x.index).Select(x => x.port).ToList();

                        if (dynamicPorts[index] == null)
                        {
                            Debug.LogWarning("No port found at index " + index + " - Skipped");
                        }
                        else if (dynamicPorts.Count <= index)
                        {
                            Debug.LogWarning("DynamicPorts[" + index + "] out of range. Length was " + dynamicPorts.Count + " - Skipped");
                        }
                        else
                        {
                            // Clear the removed ports connections
                            dynamicPorts[index].ClearConnections();
                            // Move following connections one step up to replace the missing connection
                            for (int k = index + 1; k < dynamicPorts.Count(); k++)
                            {
                                for (int j = 0; j < dynamicPorts[k].ConnectionCount; j++)
                                {
                                    XNode.NodePort other = dynamicPorts[k].GetConnection(j);
                                    dynamicPorts[k].Disconnect(other);
                                    dynamicPorts[k - 1].Connect(other);
                                }
                            }
                            // Remove the last dynamic port, to avoid messing up the indexing
                            skill.RemoveDynamicPort(dynamicPorts[dynamicPorts.Count() - 1].fieldName);
                            serializedObject.Update();
                            EditorUtility.SetDirty(skill);
                        }

                        skill.transitions.RemoveAt(index);
                        // Error handling. If the following happens too often, file a bug report at https://github.com/Siccity/xNode/issues
                        if (dynamicPorts.Count <= skill.transitions.Count)
                        {
                            while (dynamicPorts.Count <= skill.transitions.Count)
                            {
                                skill.transitions.RemoveAt(skill.transitions.Count - 1);
                            }
                            UnityEngine.Debug.LogWarning("Array size exceeded dynamic ports size. Excess items removed.");
                        }
                        serializedObject.ApplyModifiedProperties();
                        serializedObject.Update();
                        return;
                    }
                    var port = skill.GetPort(fieldName + " " + index);
                    if (port != null)
                    {
                        Vector2 pos = new Vector2(r.width + 15, r.y);
                        NodeEditorGUILayout.PortField(pos, port);
                    }
                    r.y += EditorGUIUtility.singleLineHeight;
                    ++index;
                }
                if (GUILayout.Button("收起"))
                {
                    IsShow = false;
                }
                EditorGUILayout.EndVertical();
            }
            else
            {
                int count = 0;
                foreach (var port in skill.DynamicOutputs) { ++count; }
                Rect r = EditorGUILayout.BeginVertical(GUILayout.Height(Enumerable.Count(skill.DynamicOutputs) * EditorGUIUtility.singleLineHeight));
                r.x += 100;
                EditorGUILayout.LabelField("退出");
                int index = 0;
                foreach (var transition in transitions)
                {
                    var port = skill.GetPort("transitions " + index);
                    Vector2 pos = new Vector2(r.width + 15, r.y);
                    NodeEditorGUILayout.PortField(pos, port);
                    r.y += EditorGUIUtility.singleLineHeight;
                    ++index;
                }
                EditorGUILayout.EndVertical();
                if (GUILayout.Button("展开"))
                {
                    IsShow = true;
                }
            }
        }

        public ReorderableList CreateList(List<Condition> conditions,int transIndex)
        {
            ReorderableList list = null;
            if (transiditions.ContainsKey(conditions))
            {
                list = transiditions[conditions];
            }
            else
            {
                list = CreateReorderableList(conditions,transIndex);
                transiditions.Add(conditions, list);
            }
            list.DoLayoutList();
            return list;
        }

        public ReorderableList CreateReorderableList(List<Condition> conditions, int transIndex)
        {
            ReorderableList list = new ReorderableList(conditions, null, true, true, true, true);
            
            // Override drawHeaderCallback to display node's name instead
            list.drawHeaderCallback = (Rect rect) =>
            {
                string title = serializedObject.targetObject.name;
                EditorGUI.LabelField(rect, "conditions");
            };
            list.elementHeightCallback = (int index) =>
            {
                if (index < conditions.Count)
                {
                    return conditions[index].GetHeight();
                }
                return 0;
            };

            list.drawElementCallback =
            (Rect rect, int index, bool isActive, bool isFocused) =>
            {
                if (index < conditions.Count)
                {
                    conditions[index].Display(rect);
                }
            };
            list.onAddDropdownCallback = (Rect buttonRect, ReorderableList l) =>
            {
                var skill = this.serializedObject.targetObject as Skill;
                var graph = skill.graph as SkillGraph;
                var menu = new GenericMenu();
                menu.AddItem(new GUIContent("动画结束"),
                                  false,
                                  (object target) =>
                                  {
                                      conditions.Add(new Condition(Condition.ConditionType.kAnim,"exit anim"));
                                      serializedObject.ApplyModifiedProperties();
                                      serializedObject.Update();
                                  },
                                  null);
                foreach (var parameter in graph.parameters)
                {
                    menu.AddItem(new GUIContent(parameter.name),
                                  false,
                                  (object target) =>
                                  {
                                      conditions.Add(new Condition(parameter.type, parameter.name));
                                      serializedObject.ApplyModifiedProperties();
                                      serializedObject.Update();
                                  },
                                  parameter);
                }

                menu.ShowAsContext();
            };

            return list;
        }
    }
}
