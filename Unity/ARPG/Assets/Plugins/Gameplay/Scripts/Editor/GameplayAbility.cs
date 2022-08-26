using System.Collections;
using System.Collections.Generic;
using System.Linq;
using Sirenix.OdinInspector;
using UnityEditor;
using UnityEngine;

namespace GameplayEditor
{
    [CreateAssetMenu(fileName = "GameplayAbility", menuName = "Gameplay/GameplayAbility")]
    public class GameplayAbility : SerializedScriptableObject
    {
        public GameplayEffect CooldownGameplayEffect;

        public GameplayEffect CostGameplayEffect;

        [FoldoutGroup("Tag"), ValueDropdown("GetAllGameplayTags"), CustomValueDrawer("DrawTagLabel"), Searchable]
        public List<int> AbilityTags = new List<int>();

        [FoldoutGroup("Tag"), ValueDropdown("GetAllGameplayTags"), CustomValueDrawer("DrawTagLabel"), Searchable]
        public List<int> CancelAbility = new List<int>();

        [FoldoutGroup("Tag"), ValueDropdown("GetAllGameplayTags"), CustomValueDrawer("DrawTagLabel"), Searchable]
        public List<int> BlockAbilitiesWithTag = new List<int>();

        [FoldoutGroup("Tag"), ValueDropdown("GetAllGameplayTags"), CustomValueDrawer("DrawTagLabel"), Searchable]
        public List<int> ActivationOwnedTags = new List<int>();

        [FoldoutGroup("Tag"), ValueDropdown("GetAllGameplayTags"), CustomValueDrawer("DrawTagLabel"), Searchable]
        public List<int> ActivationRequiredTags = new List<int>();

        [FoldoutGroup("Tag"), ValueDropdown("GetAllGameplayTags"), CustomValueDrawer("DrawTagLabel"), Searchable]
        public List<int> ActivationBlockedTags = new List<int>();

        [FoldoutGroup("Tag"), ValueDropdown("GetAllGameplayTags"), CustomValueDrawer("DrawTagLabel"), Searchable]
        public List<int> SourceRequiredTags = new List<int>();

        [FoldoutGroup("Tag"), ValueDropdown("GetAllGameplayTags"), CustomValueDrawer("DrawTagLabel"), Searchable]
        public List<int> SourceBlockedTags = new List<int>();

        [FoldoutGroup("Tag"), ValueDropdown("GetAllGameplayTags"), CustomValueDrawer("DrawTagLabel"), Searchable]
        public List<int> TargetRequiredTags = new List<int>();

        [FoldoutGroup("Tag"), ValueDropdown("GetAllGameplayTags"), CustomValueDrawer("DrawTagLabel"), Searchable]
        public List<int> TargetBlockedTags = new List<int>();

        private IEnumerable GetAllGameplayTags
        {
            get
            {
                var list = new ValueDropdownList<int>();
                foreach (var tag in GameplayTagManager.Instance.Tags)
                {
                    var name = tag.Name.Replace('.', '/');
                    list.Add(name, tag.Key);
                }

                return list;
            }
        }

        private int DrawTagLabel(int key)
        {
            var tag = GameplayTagManager.Instance.GetGameplayTagByKey(key);
            if (tag != null) EditorGUILayout.LabelField(tag.Name);
            return key;
        }

        private IEnumerable GetAllGameplayEffect
        {
            get
            {
                List<GameplayEffect> results = new List<GameplayEffect>() {null};
                results.AddRange(AssetDatabase.FindAssets("t:GameplayEffect")
                    .Select(guid =>
                        AssetDatabase.LoadAssetAtPath<GameplayEffect>(AssetDatabase.GUIDToAssetPath(guid))));
                return results;
            }
        }
    }
}