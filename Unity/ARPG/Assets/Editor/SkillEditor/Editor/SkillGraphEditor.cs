using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using XNode.Examples.StateGraph;
using XNodeEditor;
using SkillEditor;
using UnityEditor;
using UnityEditorInternal;

[CustomNodeGraphEditor(typeof(SkillGraph))]
public class StateGraphEditor : NodeGraphEditor
{
    public Rect guiRect = new Rect(new Vector2(0, 0), new Vector2(300, 500));
    public Vector2 paramScrollPos = new Vector2(0,0);
    public ReorderableList list = null;
    public override void OnGUI() 
    {
        guiRect.height = window.position.height;
        if (guiRect.Contains(Event.current.mousePosition))
        {
            window.IsHoveringGUI = true;
        }
        else
        {
            window.IsHoveringGUI = false;
        }
        //Debug.Log("OnOpen");
        EditorGUILayout.BeginVertical("button",GUILayout.Width(guiRect.width),GUILayout.Height(guiRect.height));
        paramScrollPos = EditorGUILayout.BeginScrollView(paramScrollPos, GUILayout.Width(guiRect.width), GUILayout.Height(guiRect.height - EditorGUIUtility.singleLineHeight * 2));
        ShowParameterList();
        EditorGUILayout.EndScrollView();        
        if (GUILayout.Button("保存"))
        {
            var graph = serializedObject.targetObject as SkillGraph;
            graph.ExportJson();
        }
        EditorGUILayout.EndVertical();

        serializedObject.ApplyModifiedProperties();
        serializedObject.Update();
    }

    public void ShowParameterList()
    {
        if (list == null)
        {
            var graph = serializedObject.targetObject as SkillGraph;
            list = new ReorderableList(graph.parameters, null, true, true, true, true);
            var conditions = serializedObject.FindProperty("conditions");
            // Override drawHeaderCallback to display node's name instead
            list.drawHeaderCallback = (Rect rect) =>
            {
                string title = serializedObject.targetObject.name;
                EditorGUI.LabelField(rect, "parameters");
            };

            list.drawElementCallback = (Rect rect, int index, bool isActive, bool isFocused) =>
            {
                var paramerters = serializedObject.FindProperty("parameters");
                var element = paramerters.GetArrayElementAtIndex(index);
                var name = element.FindPropertyRelative("name");
                //Debug.Log(name.stringValue);
                EditorGUI.PropertyField(rect, element, true);
            };
            list.elementHeightCallback = (int index) =>
            {
                var paramerters = serializedObject.FindProperty("parameters");
                if (paramerters.arraySize > 0)
                {
                    var element = paramerters.GetArrayElementAtIndex(index);
                    return EditorGUI.GetPropertyHeight(element);
                }
                return EditorGUIUtility.singleLineHeight;

            };
            list.onRemoveCallback = (ReorderableList rl) =>
            {
                graph.parameters.RemoveAt(rl.index);
                serializedObject.ApplyModifiedProperties();
                serializedObject.Update();
            };

            list.onAddDropdownCallback = (Rect buttonRect, ReorderableList l) =>
            {
                var menu = new GenericMenu();

                
                foreach (var name in System.Enum.GetNames(typeof(Condition.ConditionType)))
                {
                    if(name == Condition.ConditionType.kAnim.ToString())
                    {
                        continue;
                    }
                    menu.AddItem(new GUIContent(name),
                                  false,
                                  (object target) =>
                                  {
                                      SkillGraph.Parameter paramter = new SkillGraph.Parameter();
                                      paramter.type = (Condition.ConditionType)System.Enum.Parse(typeof(Condition.ConditionType), name);
                                      paramter.name = "param" + graph.parameters.Count;
                                      graph.parameters.Add(paramter);

                                      serializedObject.ApplyModifiedProperties();
                                      serializedObject.Update();
                                  },
                                  name);
                }

                menu.ShowAsContext();
            };
        }
        
        list.DoLayoutList();
    }

    /// <summary> 
    /// Overriding GetNodeMenuName lets you control if and how nodes are categorized.
    /// In this example we are sorting out all node types that are not in the XNode.Examples namespace.
    /// </summary>
    public override string GetNodeMenuName(System.Type type)
    {
        if (type.Namespace == "SkillEditor")
        {
            return base.GetNodeMenuName(type).Replace("Skill Editor/", "");
        }
        else return null;
    }
}
