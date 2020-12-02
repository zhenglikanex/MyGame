using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DialogUI : BaseUI
{
    public override void OnDestroy()
    {
        base.OnDestroy();
    }

    public override UIManager.UIType GetUIType()
    {
        return UIManager.UIType.kDialogType;
    }
}
