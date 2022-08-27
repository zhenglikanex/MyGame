using GameplayEditor;
using LitJson;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using UnityEditor;
using UnityEngine;
using UnityEngine.Playables;
using UnityEngine.Timeline;

namespace GameplayEditor
{
    public class TimelineExportManager
    {
        public static readonly double FrameRate = 30;

#if UNITY_EDITOR
        public class Event
        {
            public uint StartFrame;
            public uint EndFrame;
        }

        public class ColliderEvent : Event
        {
            public bool Once;
            public bool Follow;
            public Matrix4x4 Transform;
        }

        public class BoxColliderEvent : ColliderEvent
        {
            public string Type = "box";
            public Vector3 Size;
        }

        public class SphereColliderEvent : ColliderEvent
        {
            public string Type = "sphere";
            public float Radius;
        }

        public class CapsuleColliderEvent : ColliderEvent
        {
            public string Type = "capsule";
            public float Radius;
            public float HalfHeight;
        }

        public class AnimationEvent : Event
        {
            public string Name;
        }

        public class TagEvent : Event
        {
            public List<GameplayTagManager.Tag> GrantedTags = new List<GameplayTagManager.Tag>();
        }

        public class TimelineInfo
        {
            public int FrameCount;
            public List<AnimationEvent> AnimationEvents = new List<AnimationEvent>();
            public List<ColliderEvent> ColliderEvents = new List<ColliderEvent>();
            public List<TagEvent> TagEvents = new List<TagEvent>();
        }
#endif

        [MenuItem("Gameplay/ExportTimeline")]
        private static void ExportTimeline()
        {
            var playableAssets = AssetDatabase.FindAssets("t:PlayableAsset")
                .Select(guid => AssetDatabase.LoadAssetAtPath<PlayableAsset>(AssetDatabase.GUIDToAssetPath(guid)))
                .ToArray();
            foreach (var playableAsset in playableAssets)
            {
                TimelineInfo timelineInfo = new TimelineInfo();
                Debug.Log("111111111111");
                Debug.Log(playableAsset.duration * FrameRate);
                timelineInfo.FrameCount = Mathf.CeilToInt((float) (playableAsset.duration * FrameRate));

                foreach (var playableAssetOutput in playableAsset.outputs)
                {
                    var track = playableAssetOutput.sourceObject as TrackAsset;
                    if (track)
                    {
                        foreach (var timelineClip in track.GetClips())
                        {
                            var asset = timelineClip.asset as PlayableAsset;
                            if (asset is AnimationPlayableAsset animationAsset)
                            {
                                var animationEvent = new AnimationEvent();
                                animationEvent.Name = timelineClip.displayName;
                                animationEvent.StartFrame = (uint) (timelineClip.start * FrameRate);
                                animationEvent.EndFrame = (uint) (timelineClip.end * FrameRate);
                                timelineInfo.AnimationEvents.Add(animationEvent);
                            }

                            ITimelineExport timelineExport = asset as ITimelineExport;
                            var timelineEvent = timelineExport?.Export(timelineClip);
                            if (timelineEvent != null)
                            {
                                if (timelineEvent is ColliderEvent colliderEvent)
                                {
                                    timelineInfo.ColliderEvents.Add(colliderEvent);
                                }

                                if (timelineEvent is TagEvent tagEvent)
                                {
                                    timelineInfo.TagEvents.Add(tagEvent);
                                }
                            }
                        }
                    }
                }

                
                var json = LitJson.JsonMapper.ToJson(timelineInfo);
                FileStream f = new FileStream($"Assets/Resources/Config/Timeline/{playableAsset.name}.json", FileMode.Create, FileAccess.Write);
                StreamWriter stream = new StreamWriter(f);
                stream.Write(json);
                stream.Close();
                AssetDatabase.Refresh();
                
            }
        }
    }
}