using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class NormalUI : BaseUI {

	// Use this for initialization
    public override void OnDestroy()
    {
        base.OnDestroy();
    }

    public override UIManager.UIType GetUIType()
    {
        return UIManager.UIType.kNormalType;
    }
}
