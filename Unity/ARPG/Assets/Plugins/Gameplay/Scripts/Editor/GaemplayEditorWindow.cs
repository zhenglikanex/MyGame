
#if UNITY_EDITOR
using Sirenix.OdinInspector.Editor;
using Sirenix.OdinInspector.Demos.RPGEditor;
using Sirenix.Utilities;
using Sirenix.Utilities.Editor;
using UnityEditor;
using UnityEngine;
using System.Linq;
using UnityEngine.Playables;

namespace GameplayEditor
{
    public class GameplayEditorWindow : OdinMenuEditorWindow
    {
        

        [MenuItem("Gameplay/Gameplay Editor")]
        private static void Open()
        {
            var window = GetWindow<GameplayEditorWindow>();
            window.position = GUIHelper.GetEditorWindowRect().AlignCenter(800, 500);
        }

        protected override OdinMenuTree BuildMenuTree()
        {
            var tree = new OdinMenuTree(true);
            tree.DefaultMenuStyle.IconSize = 28.00f;
            tree.Config.DrawSearchToolbar = true;

            tree.Add("GameplayTag", GameplayTagManager.Instance);

            tree.Add("GameplayAbility", null);

            // Adds all characters.
            tree.AddAllAssetsAtPath("GameplayAbility", "Assets/Editor/GameplayAbility", typeof(GameplayAbility), true);

            // Adds the character overview table.
            GameplayEffectOverview.Instance.UpdateGameplayEffectOverview();
            tree.Add("GameplayEffect", null);

            // Adds all characters.
            tree.AddAllAssetsAtPath("GameplayEffect", "Assets/Editor/GameplayEffect", typeof(GameplayEffect), true);

            return tree;
        }

        protected override void OnBeginDrawEditors()
        {
            var selected = this.MenuTree.Selection.FirstOrDefault();
            var toolbarHeight = this.MenuTree.Config.SearchToolbarHeight;

            // Draws a toolbar with the name of the currently selected menu item.
            SirenixEditorGUI.BeginHorizontalToolbar(toolbarHeight);
            {
                if (selected != null)
                {
                    GUILayout.Label(selected.Name);
                }

                if (SirenixEditorGUI.ToolbarButton(new GUIContent("Create GameplayAbility")))
                {
                    ScriptableObjectCreator.ShowDialog<GameplayAbility>("Assets/Editor/GameplayAbility", obj =>
                    {
                        //obj.Name = obj.name;
                        base.TrySelectMenuItemWithObject(obj); // Selects the newly created item in the editor
                    });
                }

                if (SirenixEditorGUI.ToolbarButton(new GUIContent("Create GameplayEffect")))
                {
                    ScriptableObjectCreator.ShowDialog<GameplayEffect>("Assets/Editor/GameplayEffect", obj =>
                    {
                        //obj.Name = obj.name;
                        base.TrySelectMenuItemWithObject(obj); // Selects the newly created item in the editor
                    });
                }

            }
            SirenixEditorGUI.EndHorizontalToolbar();
        }
    }
}

#endif
