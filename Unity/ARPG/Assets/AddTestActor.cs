using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AddTestActor : MonoBehaviour
{
    private Animator animator;
    // Start is called before the first frame update
    void Start()
    {
        animator = GetComponent<Animator>();
        animator.Play("Idle");
        
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetAxis("Vertical") > 0.0f)
        {
            animator.Play("Locomotion");
        }
        else
        {
            animator.Play("Idle");
        }
        float y = (float)System.Math.Round(Input.GetAxis("Vertical"), 1);
        animator.SetFloat("forward", y);
    }
    
    private void OnAnimatorMove()
    {
        transform.localPosition += animator.velocity * Time.fixedDeltaTime;
        float y = (float)System.Math.Round(Input.GetAxis("Vertical"), 1);
        var state = animator.GetCurrentAnimatorStateInfo(0);
        var process = state.normalizedTime - System.Math.Floor(state.normalizedTime);
        if(state.IsName("Locomotion"))
        {
            Debug.Log(string.Format("1 unity {0} time{7:F6} vec x:{1:F6} y:{2:F6} z:{3:F6} delt x:{4:F6} y:{5:F6} z:{6:F6}", y, animator.velocity.x, animator.velocity.y, animator.velocity.z, animator.deltaPosition.x, animator.deltaPosition.y, animator.deltaPosition.z, process * state.length));
        }
    }
}
