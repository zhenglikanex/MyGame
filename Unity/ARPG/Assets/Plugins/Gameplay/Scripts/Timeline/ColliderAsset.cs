using System.Collections;
using System.Collections.Generic;
using System.Threading;
using UnityEngine;
using UnityEngine.Playables;
using Sirenix.OdinInspector;
using Sirenix.Serialization;
using UnityEngine.Timeline;

namespace GameplayEditor
{
    [System.Serializable]
    public class ColliderAsset : PlayableAsset, ITimelineExport
    {

        public enum ColliderType
        {
            Box,
            Sphere,
            Capsule
        }

        [System.Serializable]
        public struct BoxCollider
        {
            public Vector3 Size;
        }

        [System.Serializable]
        public struct SphereCollider
        {
            public float Radius;
        }

        [System.Serializable]
        public struct CapsuleCollider
        {
            public float Radius;
            public float Height;
        }

        [Tooltip("勾上表示只触发一次，否则持续到Clip结束")] public bool Once = true;

        [Tooltip("跟随自身")] public bool Follow = false;

        public Vector3 Position;
        public Vector3 Rotation;

        public ColliderType Type = ColliderType.Box;

        [ShowIf("Type", Value = ColliderType.Box)]
        public BoxCollider Box = new BoxCollider() {Size = Vector3.one};

        [ShowIf("Type", Value = ColliderType.Sphere)]
        public SphereCollider Sphere = new SphereCollider() {Radius = 1};

        [ShowIf("Type", Value = ColliderType.Capsule)]
        public CapsuleCollider Capsule = new CapsuleCollider() {Radius = 0.5f, Height = 2};

        // Factory method that generates a playable based on this asset
        public override Playable CreatePlayable(PlayableGraph graph, GameObject go)
        {

#if UNITY_EDITOR
            var playable = ScriptPlayable<ColliderBehaviour>.Create(graph);
            ColliderBehaviour behaviour = playable.GetBehaviour();
            behaviour.Go = go;
            behaviour.Once = Once;
            behaviour.Follow = Follow;
            behaviour.Position = Position;
            behaviour.Rotation = Rotation;
            behaviour.Type = Type;
            behaviour.Box = Box;
            behaviour.Sphere = Sphere;
            behaviour.Capsule = Capsule;

            return playable;
#else
            return Playable.Create(graph);
#endif

        }

#if UNITY_EDITOR

        public TimelineExportManager.Event Export(TimelineClip clip)
        {
            if (Type == ColliderType.Box)
            {
                var colliderEvent = new TimelineExportManager.BoxColliderEvent();
                colliderEvent.StartFrame = (uint) (clip.start * TimelineExportManager.FrameRate);
                colliderEvent.EndFrame = (uint)(clip.end * TimelineExportManager.FrameRate);
                colliderEvent.Once = Once;
                colliderEvent.Follow = Follow;
                colliderEvent.Transform = Matrix4x4.TRS(Position, Quaternion.Euler(Rotation), Vector3.one);
                colliderEvent.Size = Box.Size;
                return colliderEvent;
            }
            else if (Type == ColliderType.Sphere)
            {
                var colliderEvent = new TimelineExportManager.SphereColliderEvent();
                colliderEvent.StartFrame = (uint)(clip.start * TimelineExportManager.FrameRate);
                colliderEvent.EndFrame = (uint)(clip.end * TimelineExportManager.FrameRate);
                colliderEvent.Once = Once;
                colliderEvent.Follow = Follow;
                colliderEvent.Transform = Matrix4x4.TRS(Position, Quaternion.Euler(Rotation), Vector3.one);
                colliderEvent.Radius = Sphere.Radius;
                return colliderEvent;
            }
            else
            {
                var colliderEvent = new TimelineExportManager.CapsuleColliderEvent();
                colliderEvent.StartFrame = (uint)(clip.start * TimelineExportManager.FrameRate);
                colliderEvent.EndFrame = (uint)(clip.end * TimelineExportManager.FrameRate);
                colliderEvent.Once = Once;
                colliderEvent.Follow = Follow;
                colliderEvent.Transform = Matrix4x4.TRS(Position, Quaternion.Euler(Rotation), Vector3.one);
                colliderEvent.Radius = Capsule.Radius;
                colliderEvent.HalfHeight = Capsule.Height / 2;
                return colliderEvent;
            }
        }
#endif
    }
}