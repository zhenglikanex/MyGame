using System.Collections;
using System.Collections.Generic;
using UnityEngine;



public class FSMSystem
{
    // 状态工厂委托
    public delegate FSMState FSMStateFactory();

    // 状态工厂
    private Dictionary<StateID, FSMStateFactory> state_factory_map_ = new Dictionary<StateID, FSMStateFactory>();
    private FSMState cur_state_;
    private StateID cur_state_id_;

    public FSMState CurState
    {
        get
        {
            return cur_state_;
        }
        set
        {
            cur_state_ = value;
        }
    }

    public StateID CurStateId
    {
        get
        {
            return cur_state_id_;
        }
        set
        {
            cur_state_id_ = value;
        }
    }

    public bool InitCurState(StateID id)
    {
        if(!state_factory_map_.ContainsKey(id))
        {
            return false;
        }

        CurStateId = id;

        cur_state_ = state_factory_map_[id]();
        cur_state_.Enter();

        return true;
    }

    // 注册状态工厂
    public void RegisterStateFactory(StateID id, FSMStateFactory factory)
    {
        if(id == StateID.kInvalid)
        {
            Debug.LogError("FSMSystem ERROR : 无效的状ID");
            return;
        }

        if(state_factory_map_.ContainsKey(id))
        {
            return;
        }

        state_factory_map_.Add(id, factory);
    }

    public void DeleteStateFactory(StateID id)
    {
        state_factory_map_.Remove(id);
    }

    public FSMState GetFSMState(StateID id)
    {
        if (!state_factory_map_.ContainsKey(id))
        {
            Debug.LogError("FSMSystem Error : 不存在此状态");
            return null;
        }
        return state_factory_map_[id]();
    }

    public void DoTransition(Transition trans)
    {
        if(trans == Transition.kInvalid)
        {
            Debug.LogError("FSMSystem Error : 无效的过度类型");
            return;
        }

        StateID id = cur_state_.GetChangeState(trans);

        if(id  == StateID.kInvalid)
        {
            Debug.LogError("FSMSystem Error : 当前状态没有此过度类型, " + "Transition : " + trans + "CurState : " + cur_state_);
            return;
        }

        FSMState next_state = state_factory_map_[id]();
        if (next_state != null)
        {
            cur_state_.Leave();
            cur_state_ = next_state;
            cur_state_.Enter();

            CurStateId = id;
        }
    }
}