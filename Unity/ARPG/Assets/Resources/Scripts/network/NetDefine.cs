using System.Collections;
using System.Collections.Generic;
using UnityEngine;


enum NetMoudleID : ushort
{
    kUserMoudle         = 1,        //用户模块
    kMatchingMoudle     = 2,        //匹配模块
    kBattleMoudle       = 3,        //战斗模块
}

// 协议号定义
enum ProtoID : uint
{
    #region 用户模块10000
    kRegAccount         = 10001,        //注册账号
    kLogin              = 10002,        //登入
    kChangeName         = 10003,        //改名
    #endregion

    #region 匹配模块20000
    kMatchingBattle         = 20001,        //匹配战斗
    kEnterSelectHeroRoom    = 20002,        //进入英雄选择
    kUpdateRoomEnemyHero    = 20003,        //更新敌人英雄
    kSelectHero             = 20004,        //选择英雄
    kLoadBattleScentBegin   = 20005,        //开始加载场景
    kLoadBattleScentEnd     = 20006,        //场景加载完毕
    #endregion

    #region 战斗模块30000
    SC_StartBattle          = 30001,        //开始战斗
    CS_SyncHero             = 30002,        //同步自己的英雄信息
    SC_SyncNetHero          = 30003,        //同步
    CS_ReqLaunchSkill       = 30004,        //请求释放技能
    SC_SyncSkill            = 30005,        //创建技能
    SC_SyncHero             = 30006,        // 
    SC_HeroDeath            = 30007,		// 英雄死亡
    SC_HeroRevive           = 30008,        // 英雄复活
    SC_FlyText              = 30009,        // 飘字
    #endregion
}