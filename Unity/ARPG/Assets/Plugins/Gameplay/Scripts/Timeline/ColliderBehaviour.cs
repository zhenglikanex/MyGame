#if UNITY_EDITOR
using System.Collections;
using System.Collections.Generic;
using Sirenix.OdinInspector;
using UnityEditor;
using UnityEngine;
using UnityEngine.Playables;
using UnityEngine.UI;

namespace GameplayEditor
{
// A behaviour that is attached to a playable
    public class ColliderBehaviour : PlayableBehaviour
    {
        public GameObject Go;
        public bool Once;
        public bool Follow;
        public Vector3 Position;
        public Vector3 Rotation;

        public ColliderAsset.ColliderType Type = ColliderAsset.ColliderType.Box;

        public ColliderAsset.BoxCollider Box = new ColliderAsset.BoxCollider() {Size = Vector3.one};

        public ColliderAsset.SphereCollider Sphere = new ColliderAsset.SphereCollider() {Radius = 1};

        public ColliderAsset.CapsuleCollider Capsule = new ColliderAsset.CapsuleCollider() {Radius = 0.5f, Height = 2};

        private Material _material;

        private bool _finish = false;
        private Vector3 _lastPosition;

        private Material Mat
        {
            get
            {
                if (_material == null)
                {
                    _material = AssetDatabase.LoadAssetAtPath<Material>("Assets/Plugins/Gameplay/ColliderMat.mat");
                }

                return _material;
            }
        }


        // Called when the owning graph starts playing
        public override void OnGraphStart(Playable playable)
        {

        }

        // Called when the owning graph stops playing
        public override void OnGraphStop(Playable playable)
        {

        }

        // Called when the state of the playable is set to Play
        public override void OnBehaviourPlay(Playable playable, FrameData info)
        {
            _finish = false;
            _lastPosition = Go.transform.position;
        }

        // Called when the state of the playable is set to Paused
        public override void OnBehaviourPause(Playable playable, FrameData info)
        {

        }

        // Called each frame while the state is set to Play
        public override void PrepareFrame(Playable playable, FrameData info)
        {

        }

        public override void ProcessFrame(Playable playable, FrameData info, object playerData)
        {
            Vector3 offset = Go.transform.position - _lastPosition;
            Debug.Log(offset);
            Debug.Log(Go.transform.position);
            if (!Follow)
            {
                offset = Vector3.zero;
            }

            if (Type == ColliderAsset.ColliderType.Box && !_finish)
            {
                Mesh mesh = Resources.GetBuiltinResource<Mesh>("Cube.fbx");
                Matrix4x4 matrix = Matrix4x4.TRS(Position + offset, Quaternion.Euler(Rotation), Box.Size);
                Graphics.DrawMesh(mesh, matrix, Mat, 0);
            }

            if (Type == ColliderAsset.ColliderType.Sphere && !_finish)
            {
                Mesh mesh = Resources.GetBuiltinResource<Mesh>("Sphere.fbx");
                Matrix4x4 matrix = Matrix4x4.TRS(Position + offset, Quaternion.Euler(Rotation),
                    new Vector3(Sphere.Radius, Sphere.Radius, Sphere.Radius));
                Graphics.DrawMesh(mesh, matrix, Mat, 0);
            }

            if (Type == ColliderAsset.ColliderType.Capsule && !_finish)
            {
                Mesh mesh = Resources.GetBuiltinResource<Mesh>("Capsule.fbx");
                Matrix4x4 matrix = Matrix4x4.TRS(Position + offset, Quaternion.Euler(Rotation),
                    new Vector3(Capsule.Radius, Capsule.Height, Capsule.Radius));
                Graphics.DrawMesh(mesh, matrix, Mat, 0);
            }

            if (Once)
            {
                _finish = true;
            }
        }

    }
}

#endif