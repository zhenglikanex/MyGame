using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Scheduler : GlobalGameObjectSingleton<Scheduler>
{
    public void DelayCall(System.Action action,float time)
    {
        StartCoroutine(Delay(action,time));
    }
    
    private IEnumerator Delay(System.Action action, float time)
    {
        yield return new WaitForSeconds(time);
        if(action != null)
        {
            action();
        }
    }
}
