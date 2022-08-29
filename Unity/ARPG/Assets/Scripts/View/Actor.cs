using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Playables;
using UnityEngine.Timeline;

public class Actor : UnityView
{
    private Animator animator;
    private PlayableDirector playableDirector;
    private Quaternion TargetRotation { get; set; }
    public Transform weapon;

    private void Awake()
    {
        animator = GetComponent<Animator>();
        playableDirector = GetComponent<PlayableDirector>();
    }

    // Start is called before the first frame update
    void Start()
    {
        TargetRotation = transform.rotation;
    }

    // Update is called once per frame
    void Update()
    {

    }

    public override void OnInit()
    {
        base.OnInit();
    }

    public override void OnClear()
    {
        base.OnClear();
    }

    public override void UpdatePosition(float x, float y, float z)
    {
        Vector3 pos = new Vector3(x, y, z);
        transform.position = pos;
    }

    public override void UpdateRotation(float x, float y, float z,float w)
    {
        transform.rotation = new Quaternion(x, y, z, w);
    }

    public override void MovePosition(float x, float y, float z)
    {
        Vector3 pos = new Vector3(x, y, z);
        transform.position = pos;
    }

    public override void MoveRotation(float x, float y, float z,float w)
    {
        transform.rotation = new Quaternion(x, y, z, w);
    }

    public override void PlayAnim(string str,bool is_timeline)
    {
        playableDirector.playableAsset = null;
        playableDirector.Stop();

        if (is_timeline)
        {
            var playable = Resources.Load<PlayableAsset>($"Timeline/GameplayAbility/{str}");

            if (playable)
            {
                foreach (var binding in playable.outputs)
                {
                    var track = binding.sourceObject as TrackAsset;
                    if (track != null && track.name == "Player")
                    {
                        playableDirector.SetGenericBinding(track, animator);
                    }
                }

                playableDirector.time = 0;
                playableDirector.playableAsset = playable;
                playableDirector.Play();
            }

            return;
        }

        string[] anim_params = str.Split('|');
        if (anim_params.Length == 2)
        {
            string animName = anim_params[0];
            string animParam = anim_params[1];
            if (animName == "Locomotion")
            {
                if (!animator.GetCurrentAnimatorStateInfo(0).IsName(animName))
                {
                    animator.Play(animName);
                }
                animator.SetFloat("forward", float.Parse(animParam));
            }
            else
            {
                animator.Play(name);
            }
        }
        else
        {
            string animName = str;
            animator.Play(animName);
        }
    }

    public override void PlayAnim(string str,float time,bool is_timeline)
    {
        playableDirector.playableAsset = null;
        playableDirector.Stop();

        if (is_timeline)
        {
            var playable = Resources.Load<PlayableAsset>($"Timeline/GameplayAbility/{str}");
            
            if (playable)
            {
                foreach (var binding in playable.outputs)
                {
                    var track = binding.sourceObject as TrackAsset;
                    if (track != null && track.name == "Player")
                    {
                        playableDirector.SetGenericBinding(track, animator);
                    }
                }

                playableDirector.time = time * playable.duration;
                playableDirector.playableAsset = playable;
                playableDirector.Play();
            }

            return;
        }

        string[] anim_params = str.Split('|');
        if (anim_params.Length == 2)
        {
            string animName = anim_params[0];
            string animParam = anim_params[1];
            if (animName == "Locomotion")
            {
                if (!animator.GetCurrentAnimatorStateInfo(0).IsName(animName))
                {
                    animator.Play(animName,0,time);
                }

                animator.SetFloat("forward", float.Parse(animParam));
            }
            else
            {
                animator.Play(name,0,time);
            }
        }
        else
        {
            string animName = str;
            animator.Play(animName,0,time);
        }
    }

    public override void ApplyCue(string name)
    {
        var cue = PoolManager.Instance.Create(name);
        cue.transform.SetParent(transform);
    }

}
