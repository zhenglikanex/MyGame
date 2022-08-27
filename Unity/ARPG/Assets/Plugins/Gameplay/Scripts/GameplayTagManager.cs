using System.Collections;
using System.Collections.Generic;
using Sirenix.OdinInspector;
using Sirenix.Utilities;
using UnityEngine;
using System;
using System.IO;
using System.Text;
using UnityEditor;
using Sirenix.OdinInspector.Editor;
using Sirenix.Utilities.Editor.Expressions;
using static GameplayEditor.GameplayTagManager;

namespace GameplayEditor
{
    
    [GlobalConfig("Editor/GameplayTag")]
    
    public class GameplayTagManager : GlobalConfig<GameplayTagManager>
    {
        public struct Tag
        {
            public long Mask;
            public long Value;
        };

        [Serializable]
        public class GameplayTag
        {
            [HideInInspector]
            public int Key = -1;

            public string Name;
        }

        [HideInInspector]
        public int TagId = 32;
        [NonSerialized] public bool Edit = false;

        [EnableIf("Edit"),TableList(), OnCollectionChanged("After")] public List<GameplayTag> Tags = new List<GameplayTag>();

        [Button(ButtonSizes.Medium), ShowIf("@this.Edit == false")]
        void Editor()
        {
            Edit = true;
        }

        [Button(ButtonSizes.Medium), ShowIf("@this.Edit == true")]
        void Save()
        {
            Edit = false;
            AssetDatabase.SaveAssets();
        }

        public GameplayTag GetGameplayTagByKey(int key)
        {
            foreach (var tag in Tags)
            {
                if (tag.Key == key)
                {
                    return tag;
                }
            }

            return null;
        }

        public void After(CollectionChangeInfo info, object value)
        {
            if (info.ChangeType == CollectionChangeType.Add)
            {
                var tag = info.Value as GameplayTag;
                tag.Key = TagId++;
            }
        }

        static string GenTagName(string[] tokens,int size)
        {
            string name = "k";
            for (int i = 0; i < size; ++i)
            {
                name += tokens[i];
            }

            return name;
        }

        public static Tag GetTagByName(string name)
        {
            Dictionary<string, Tag> tagMap = new Dictionary<string, Tag>();
            List<Dictionary<string, byte>> idMapArray = new List<Dictionary<string, byte>>();
            for (int i = 0; i < 8; ++i)
            {
                idMapArray.Add(new Dictionary<string, byte>());
            }
            foreach (var gameplayTag in GameplayTagManager.Instance.Tags)
            {
                var tokens = gameplayTag.Name.Split('.');

                if (tokens.Length <= 0)
                {
                    continue;
                }

                if (tokens.Length >= 8)
                {
                    Debug.LogError( $"{gameplayTag.Name}  Tag层级超过8层!");
                    continue;
                }

                long value = 0;
                long mask = 0;

                for (var i = 0; i < tokens.Length; ++i)
                {
                    long subValue = 0;
                    var  idMap = idMapArray[i];
                    if (idMap.TryGetValue(tokens[i], out var tryValue))
                    {
                        subValue = tryValue;
                    }
                    else
                    {
                        subValue = idMap.Count + 1;
                        idMap.Add(tokens[i],(byte)subValue);

                        if (subValue > 255)
                        {
                            if (i > 0)
                            {
                                var v = i - 1;
                                //std::cout << line << " " << tokens[v] << ":" << "该层级的大于255" << std::endl;
                            }
                            else
                            {
                                //std::cout << "顶层该层级的大于255" << std::endl;
                            }
                        }
                    }

                    value = (subValue << (7 - i) * 8) | value;
                    const long subMask = 0xFF;
                    mask = (subMask << (7 - i) * 8) | mask;

                    //var genTagName = GenTagName(tokens, i + 1);
                    //if (!tagMap.ContainsKey(genTagName))
                    //{
                    //    tagMap.Add(genTagName, new Tag {Mask = mask, Value = value});
                    //}


                    string tagName = tokens[0];
                    for (int j = 1; j <= i; ++j)
                    {
                        tagName = $"{tagName}.{tokens[j]}";
                    }

                    if (!tagMap.ContainsKey(tagName))
                    {
                        tagMap.Add(tagName, new Tag { Mask = mask, Value = value });
                    }
                }
            }

            if (tagMap.TryGetValue(name, out Tag t))
            {
                return t;
            }

            throw new System.Exception("not tag");
        }

    }
}