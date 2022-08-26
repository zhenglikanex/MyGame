using System.Collections;
using System.Collections.Generic;
using System.Linq;
using Sirenix.OdinInspector;
using Sirenix.Utilities;
using UnityEditor;
using UnityEngine;

namespace GameplayEditor
{
    [GlobalConfig("Editor/GameplayEffect")]
    public class GameplayEffectOverview : GlobalConfig<GameplayEffectOverview>
    {
        [ReadOnly] [ListDrawerSettings(Expanded = true)]
        public GameplayEffect[] AllGameplayEffects;

#if UNITY_EDITOR
        [Button(ButtonSizes.Medium), PropertyOrder(-1)]
        public void UpdateGameplayEffectOverview()
        {
            // Finds and assigns all scriptable objects of type Character
            this.AllGameplayEffects = AssetDatabase.FindAssets("t:GameplayEffect")
                .Select(guid => AssetDatabase.LoadAssetAtPath<GameplayEffect>(AssetDatabase.GUIDToAssetPath(guid)))
                .ToArray();
        }
#endif
    }
}

