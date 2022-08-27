using System.Collections;
using System.Collections.Generic;
using System.Linq;
using Sirenix.OdinInspector;
using UnityEditor;
using UnityEngine;
using UnityEngine.Playables;
using UnityEngine.Timeline;

namespace GameplayEditor
{
    [System.Serializable]
    public class GameplayTagAsset : PlayableAsset,ITimelineExport
    {
        [ValueDropdown("GetAllGameplayTags"), CustomValueDrawer("DrawTagLabel"), Searchable]
        public List<int> GrantedTags = new List<int>();

        // Factory method that generates a playable based on this asset
        public override Playable CreatePlayable(PlayableGraph graph, GameObject go)
        {
            return Playable.Create(graph);
        }
#if UNITY_EDITOR
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

        public TimelineExportManager.Event Export(TimelineClip clip)
        {
            TimelineExportManager.TagEvent tagEvent = new TimelineExportManager.TagEvent();
            tagEvent.StartFrame = (uint)(clip.start * TimelineExportManager.FrameRate);
            tagEvent.EndFrame = (uint)(clip.end * TimelineExportManager.FrameRate);
            foreach (var grantedTag in GrantedTags)
            { 
                 tagEvent.GrantedTags.Add(GameplayTagManager.GetTagByName(GameplayTagManager.Instance.GetGameplayTagByKey(grantedTag).Name));
            }

            return tagEvent;
        }
#endif
    }
}