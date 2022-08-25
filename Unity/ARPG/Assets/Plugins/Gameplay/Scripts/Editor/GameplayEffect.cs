using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Security.Permissions;
using Sirenix.OdinInspector;
using UnityEditor;
using UnityEngine;

namespace GameplayEditor
{
    [CreateAssetMenu(fileName = "GameplayEffect", menuName = "Gameplay/GameplayEffect")]
    public class GameplayEffect : SerializedScriptableObject
    {
        public enum PeriodicInhibitionPolicy
        {
            //是当Duration时因某些原因阻止了Effect的进行，当阻止结束，从被阻止周期继续进行
            NeverReset = 1,

            //意思是当Duration时因某些原因阻止了Effect的进行，当阻止结束，从0开始计算周期，但在0时刻不计算Attribute
            ResetPeriod = 2,

            //意思是当Duration时因某些原因阻止了Effect的进行，当阻止结束，从0开始计算周期，且在0时刻计算Attribute
            ExecuteAndResetPeriod = 3,
        }

        //GE的叠加方式
        public enum GameplayStackingType
        {
            None = 1, //不会叠加，如果多次释放则每个Effect相当于单个Effect
            AggregateBySource = 2, //在自身叠加
            AggregateByTarget = 3, //在目标叠加
        }

        public enum GameplayStackExpirationPolicy
        {
            ClearEntireStack = 1, //Duration结束时清除所有层数
            RemoveSingleStackAndRefreshDuration = 2, //Duration结束时减少一层，然后重新经历一个Duration，一直持续到层数减为0
            //调用函数处理
        }

        public enum GameplayDurationPolicy
        {
            Instant = 1, //(即刻) 对Attribute中BaseValue立即进行的永久性修改.其不会应用GameplayTag, 哪怕是一帧.
            Duration = 2, //(持续) 对Attribute中CurrentValue的临时修改和当GameplayEffect过期或手动移除时, 应用将要被移除的GameplayTag.持续时间在GE中配置

            Infinite =
                3, //(无限) 对Attribute中CurrentValue的临时修改和当GameplayEffect移除时, 应用将要被移除的GameplayTag.该类型自身永不过期且必须由某个Ability或ASC手动移除.
        }

        public enum GameplayModifierOp
        {
            Add = 1, //将Modifier指定的Attribute加上计算结果.使用负数以实现减法操作.
            Multiply = 2, //将Modifier指定的Attribute乘以计算结果.
            Divide = 3, //将Modifier指定的Attribute除以计算结果.
            Override = 4, //使用计算结果覆盖Modifier指定的Attribute.
        }

        public enum GameplayMagnitudeType
        {
            ScalableFloat = 1,
            AttributeBased = 2,
        }

        public enum GameplayTargetType
        {
            Source,
            Target,
        }

        [Serializable]
        public class GameplayModifier
        {
            [Serializable]
            public class ScalableFloat
            {
                public enum ModifierValueType
                {
                    Const = 1,
                    Table = 2
                }

                [Serializable]
                public class Table
                {
                    public float Value;
                }

                public ModifierValueType Type = ModifierValueType.Const;

                public float Scale = 1;

                [ShowIf("Type", Value = ModifierValueType.Const)]
                public float Value;

                [ShowIf("Type", Value = ModifierValueType.Table)]
                public List<float> TableValue;
            }

            [Serializable]
            public class AttributeBased
            {
                GameplayAttribute Attribute;
                public float Scale = 1;
                public GameplayTargetType Based = GameplayTargetType.Source;
                public bool Snapshot = false;
            }

            public GameplayAttribute Attribute;
            public GameplayModifierOp ModifierOp = GameplayModifierOp.Add;
            public GameplayMagnitudeType MagnitudeType = GameplayMagnitudeType.ScalableFloat;

            [ShowIf("MagnitudeType", Value = GameplayMagnitudeType.ScalableFloat)]
            public ScalableFloat ScalableFloatValue = new ScalableFloat();

            [ShowIf("MagnitudeType", Value = GameplayMagnitudeType.AttributeBased)]
            public AttributeBased AttributeBasedValue = new AttributeBased();

            [FoldoutGroup("Tags"), ValueDropdown("GetAllGameplayTags"), CustomValueDrawer("DrawTagLabel"), Searchable]
            public List<int> SourceRequireTags = new List<int>();

            [FoldoutGroup("Tags"), ValueDropdown("GetAllGameplayTags"), CustomValueDrawer("DrawTagLabel"), Searchable]
            public List<int> SourceIgnoreTags = new List<int>();

            [FoldoutGroup("Tags"), ValueDropdown("GetAllGameplayTags"), CustomValueDrawer("DrawTagLabel"), Searchable]
            public List<int> TargetRequireTags = new List<int>();

            [FoldoutGroup("Tags"), ValueDropdown("GetAllGameplayTags"), CustomValueDrawer("DrawTagLabel"), Searchable]
            public List<int> TargetIgnoreTags = new List<int>();

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
                 EditorGUILayout.LabelField(tag.Name);
                return key;
            }
        }

        [HideIfGroup("DurationPolicy", Value = GameplayDurationPolicy.Instant)]
        [FoldoutGroup("DurationPolicy/Period"), LabelText("刷新周期")]
        public float Period;

        [EnableIf("DurationPolicy", Value = GameplayDurationPolicy.Duration)]
        [FoldoutGroup("DurationPolicy/Period"), LabelText("持续时间")]
        public float Duration;

        [FoldoutGroup("DurationPolicy/Period")]
        public PeriodicInhibitionPolicy PeriodPolicy = PeriodicInhibitionPolicy.NeverReset;

        [FoldoutGroup("Application"), LabelText("成功概率"), Range(0, 100)]
        public float ChanceToApplyToTarget;

        [FoldoutGroup("Immunity"), ValueDropdown("GetAllGameplayTags"), CustomValueDrawer("DrawTagLabel"), Searchable]
        public List<int> ImmunityRequireTags = new List<int>();

        [FoldoutGroup("Immunity"), ValueDropdown("GetAllGameplayTags"), CustomValueDrawer("DrawTagLabel"), Searchable]
        public List<int> ImmunityIgnoreTags = new List<int>();

        [FoldoutGroup("Overflow"), Tooltip("超过StackLimitCount数量的Effect被Apply时将会调用该OverflowEffects")]
        public List<GameplayEffect> OverflowEffects = new List<GameplayEffect>();

        [FoldoutGroup("Overflow"), Tooltip("对应于StackDurationRefreshPolicy，如果为True则多余的Apply不会刷新Duration")]
        public bool DenyOverflowApplication = true;

        [FoldoutGroup("Overflow"), Tooltip("当DenyOverflowApplication为True是才有效，当Overflow时是否直接删除所有层数"),
         EnableIf("DenyOverflowApplication")]
        public bool ClearStackOnOverflow = true;

        [FoldoutGroup("Expiration"), ValueDropdown("GetPrematureExpirationEffectClasses"),
         Tooltip("Duration还未结束时该Effect便被强制结束，则调用PrematureExpirationEffectClasses")]
        public string PrematureExpirationEffectClasses;

        [FoldoutGroup("Expiration"), ValueDropdown("GetRoutineExpirationEffectClasses"),
         Tooltip("Duration正常结束时，调用该RoutineExpirationEffectClasses")]
        public string RoutineExpirationEffectClasses;

        [FoldoutGroup("Stacking")] public GameplayStackingType StackingType = GameplayStackingType.None;

        [FoldoutGroup("Stacking"), Min(1)] public int StackLimitCount;

        [FoldoutGroup("Stacking"), Tooltip("每次ApplyEffect时是否刷新Effect的持续时间，即使层数不增加也会刷新Duration")]
        public bool StackDurationRefreshPolicy = true;

        [FoldoutGroup("Stacking"), Tooltip("每次ApplyEffect时是否刷新周期时间（Period），即使层数不增加也会刷新Period")]
        public bool StackPeriodResetPolicy = true;

        [FoldoutGroup("Stacking"), Tooltip("当Effect的Duration结束时,层数的结束方式")]
        public GameplayStackExpirationPolicy StackExpirationPolicy = GameplayStackExpirationPolicy.ClearEntireStack;

        [FoldoutGroup("GameplayEffect")] public GameplayDurationPolicy DurationPolicy = GameplayDurationPolicy.Infinite;

        [FoldoutGroup("GameplayEffect/Modifier")]
        public List<GameplayModifier> Modifiers = new List<GameplayModifier>();

        [ToggleGroup("GameplayEffect/EnabledExecutions")]
        public bool EnabledExecutions;

        [ToggleGroup("GameplayEffect/EnabledExecutions"), ValueDropdown("GetExecutionsClasses")]
        public string ExecutionsClass = null;

        [FoldoutGroup("GrantedAbilities"), ValueDropdown("GetAllGameplayAbilities")]
        public List<GameplayAbility> GrantedAbilities = new List<GameplayAbility>();

        [FoldoutGroup("Tag"), ValueDropdown("GetAllGameplayTags"), CustomValueDrawer("DrawTagLabel"), Searchable]
        public List<int> GameplayEffectAssetTags = new List<int>();

        [FoldoutGroup("Tag"), ValueDropdown("GetAllGameplayTags"), CustomValueDrawer("DrawTagLabel"), Searchable]
        public List<int> GrantedTags = new List<int>();

        [FoldoutGroup("Tag"), ValueDropdown("GetAllGameplayTags"), CustomValueDrawer("DrawTagLabel"), Searchable]
        public List<int> OngoingTagRequirementsRequireTags = new List<int>();

        [FoldoutGroup("Tag"), ValueDropdown("GetAllGameplayTags"), CustomValueDrawer("DrawTagLabel"), Searchable]
        public List<int> OngoingTagRequirementsIgnoreTags = new List<int>();

        [FoldoutGroup("Tag"), ValueDropdown("GetAllGameplayTags"), CustomValueDrawer("DrawTagLabel"), Searchable]
        public List<int> ApplicationTagRequirementsRequireTags = new List<int>();

        [FoldoutGroup("Tag"), ValueDropdown("GetAllGameplayTags"), CustomValueDrawer("DrawTagLabel"), Searchable]
        public List<int> ApplicationTagRequirementsIgnoreTags = new List<int>();

        [FoldoutGroup("Tag"), ValueDropdown("GetAllGameplayTags"), CustomValueDrawer("DrawTagLabel"), Searchable]
        public List<int> RemoveGameplayEffectsWithTags = new List<int>();

        [FoldoutGroup("Display")] public List<string> Cues = new List<string>();

        private IEnumerable GetAllGameplayTags
        {
            get
            {
                var list = new ValueDropdownList<int>();
                foreach (var tag in GameplayTagManager.Instance.Tags)
                {
                    var name = tag.Name.Replace('.', '/');
                    list.Add(name,tag.Key);
                }

                return list;
            }
        }

        private IEnumerable GetPrematureExpirationEffectClasses
        {
            get { return new List<string>(); }
        }

        private IEnumerable GetRoutineExpirationEffectClasses
        {
            get { return new List<string>(); }
        }

        private IEnumerable GetExecutionsClasses
        {
            get { return new List<string>() {"RpgExecutionCalculation"}; }
        }

        private IEnumerable GetAllGameplayAbilities
        {
            get
            {
                return AssetDatabase.FindAssets("t:GameplayAbility")
                    .Select(guid => AssetDatabase.LoadAssetAtPath<GameplayAbility>(AssetDatabase.GUIDToAssetPath(guid)))
                    .ToArray();
            }
        }

        private int DrawTagLabel(int key)
        {
            var tag = GameplayTagManager.Instance.GetGameplayTagByKey(key);
            EditorGUILayout.LabelField(tag.Name);
            return key;
        }
    }
}