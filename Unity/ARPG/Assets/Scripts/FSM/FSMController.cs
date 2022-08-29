using System.Collections;
using System.Collections.Generic;
using UnityEngine;

// 在这里定义状态的过度类型，
public enum Transition
{
    kInvalid, // 无效的过度

    #region Game Trans定义
    kGameToStartTrans,      // 
    kGameToLoginTrans,      // 转换到登入
    kGameToMainTrans,       // 进入主游戏界面
    kGameToBattleTrans,     // 进入战斗场景
    #endregion

    #region HeroTrans
    kHeroToIdleTrans,
    kHeroToRunTrans,
    kHeroToHurtTrans,
    kHeroToDeathTrans,
    kHeroToAttackTrans,
    kHeroToSkill1Trans,
    kHeroToSkill2Trans,
    #endregion
}

// 在这里定义状态ID

public enum StateID
{
    kInvalid, // 无效的状态

    #region Game State定义
    kGameStartState,
    kGameLoginState,
    kGameMainState,
    kGameBattleState,
    #endregion

    #region Hero状态定义
    kHeroIdleState,
    kHeroRunState,
    kHeroHurtState,
    kHeroDeathState,
    kHeroAttackState,
    kHeroSkill1State,
    kHeroSkill2State,
    #endregion
}

public abstract class FSMController
{
    public FSMSystem FSMSystem_
    {
        get;
        set;
    }

    public abstract void InitFSMSystem();

    public abstract void InputHandler(object input);

    public void DoTransition(Transition trans)
    {
        FSMSystem_.DoTransition(trans);
    }
}
