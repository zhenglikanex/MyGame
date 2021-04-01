using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

public class ShowAnimtorCtroller
{
    [MenuItem("Assets/Set to HideFlags.None")]
    static void SetHideFlags()
    {
        //选中AnimatorController的状态下弹出菜单
        var path = AssetDatabase.GetAssetPath(Selection.activeObject);

        //获取SubAsset里面的所有东西
        foreach (var item in AssetDatabase.LoadAllAssetsAtPath(path))
        {
            //把全部的标志位设置为None，解除隐藏状态
            item.hideFlags = HideFlags.None;
        }
        //用Import进行刷新
        AssetDatabase.ImportAsset(path);
    }
}
