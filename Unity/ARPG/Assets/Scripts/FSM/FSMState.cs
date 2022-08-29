using UnityEngine;
using System;
using System.Collections.Generic;
using System.Collections;

// 
public abstract class FSMState
{   
    // 关联当前状态过渡到哪个状态
    protected Dictionary<Transition, StateID> map = new Dictionary<Transition, StateID>();
    protected int stateID;


    // 添加转换状态,
    // 不同的状态下可以过渡到不同的状态
    // 减少不同状态类之间的耦合,
    // 具体的状态对象通过FSMSystem获取
    public void AddTransition(Transition trans, StateID id)
    {
        if (trans == Transition.kInvalid)
        {
            Debug.LogError("FSMState ERROR: 无效的状态过渡");
            return;
        }

        if (id == StateID.kInvalid)
        {
            Debug.LogError("FSMState ERROR: 无效的状态ID");
            return;
        }

        if (map.ContainsKey(trans))
        {
            return;
        }

        map.Add(trans, id);
    }

    //删除转换状态
    public void DeleteTransition(Transition trans)
    {
        map.Remove(trans);
    }

    public StateID GetChangeState(Transition trans)
    {
        if (map.ContainsKey(trans))
        {
            return map[trans];
        }
        return StateID.kInvalid;
    }

    // 接收输入
    public abstract void InputHandler(object input);

    // 进入时执行的操作
    public abstract void Enter();

    // 退出时执行的操作
    public abstract void Leave();

    public virtual void Update()
    {

    }

}
