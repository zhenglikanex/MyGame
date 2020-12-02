﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UIManager : Singleton<UIManager>
{
    public enum UIType
    {
        kNormalType,
        kDialogType,
    }

    // 类型名字对应UI路径
    private Dictionary<string, string> ui_name_dic_ = new Dictionary<string, string>();
    private Dictionary<string, BaseUI> normal_dic_ = new Dictionary<string, BaseUI>();
    private List<BaseUI> dialogs_ = new List<BaseUI>();


    #region UI挂接点
    public Transform NormalUINode
    {
        get
        {
            GameObject go = GameObject.Find("Canvas/NoramlNode");
            if(go == null)
            {
                Debug.LogError("NoramlNode 未创建无法打开NoramlUI");
                return null;
            }

            return go.transform;
        }
    }

    public Transform DialogUINode
    {
        get
        {
            GameObject go = GameObject.Find("Canvas/DialogNode");
            if(go == null)
            {
                Debug.LogError("NoramlNode 未创建无法打开DialogUI");
                return null;
            }

            return go.transform;
        }
    }
    #endregion

    public bool Initalized()
    {

        RegisterAllUI();

        return true;
    }
    #region 注册界面UI
    public void RegisterAllUI()
    {
        RegisterUI<LoginUI>("LoginUI","");
    }

    // 注册UI，所有UI都需要注册后才能通过UIManager才能使用
    public void RegisterUI<T>(string ui_name,string path) where T : BaseUI
    {
        string key = typeof(T).ToString();
        if(ui_name_dic_.ContainsKey(key))
        {
            return;
        }

        ui_name_dic_.Add(key,path);
    }
    #endregion

    #region 打开界面
    public void OpenUI<T>(params object[] open_params) where T : BaseUI
    {
        string key = typeof(T).ToString();

        // 是否是已经打开的normal界面
        if (normal_dic_.ContainsKey(key))
        {
            return;
        }

        // 是否注册过这个界面
        if (!ui_name_dic_.ContainsKey(key))
        {
            Debug.LogError("界面未注册 ： " + key);
            return;
        }
        
        GameObject original = Resources.Load<GameObject>(ui_name_dic_[key]);
        if(original == null)
        {
            Debug.LogError("没有找到注册的界面资源" + ui_name_dic_[key]);
            return;
        }
        
        GameObject go = Object.Instantiate<GameObject>(original);
        BaseUI ui = go.GetComponent<T>();

        if(ui == null)
        {
            Debug.LogError("界面打开失败，没有UI组件：" + key);
            Object.Destroy(go);
            return;
        }

        if(ui.Open())
        {
            if(ui.GetUIType() == UIType.kNormalType)
            {
                normal_dic_.Add(key, ui);
            }
            else if(ui.GetUIType() == UIType.kDialogType)
            {
                dialogs_.Add(ui);
            }

            AttachUI(ui);

            // 初始化界面(界面无关逻辑)
            ui.Init(open_params);
        }
        else
        {
            Object.Destroy(go);
        }
    }

    public bool AttachUI(BaseUI ui)
    {
        if(ui == null)
        {
            return false;
        }

        bool ret = false;

        if (ui.GetUIType() == UIType.kNormalType)
        {
            ret = AttachNormalUI(ui);
            
        }
        else if(ui.GetUIType() == UIType.kDialogType)
        {
            ret = AttachDialogUI(ui);
        }

        return ret;
    }

    private bool AttachNormalUI(BaseUI ui)
    {
        Transform noraml_node = NormalUINode;
        if (noraml_node == null)
        {
            return false;
        }

        ui.transform.SetParent(noraml_node, false);

        return true;
    }

    private bool AttachDialogUI(BaseUI ui)
    {
        Transform dialog_node = DialogUINode;
        if(dialog_node  == null)
        {
            return false;
        }
        ui.transform.SetParent(dialog_node, false);

        return true;
    }
    #endregion

    #region 关闭界面UI

    // 直接关闭界面
    public void Close(BaseUI ui)
    {
        if(ui == null)
        {
            Debug.LogError("close ui is null");
            return;
        }

        if(ui.GetUIType() == UIType.kNormalType)
        {
            normal_dic_.Remove(ui.GetType().ToString());
        }
        else if(ui.GetUIType() == UIType.kDialogType)
        {
            dialogs_.Remove(ui);
        }

        Object.Destroy(ui.gameObject);
    }

    public void Close<T>() where T : NormalUI
    {
        string key = typeof(T).ToString();
        Debug.Log("close ui : " + key);
        if(!normal_dic_.ContainsKey(key))
        {
            return;
        }

        Close(normal_dic_[key]);
    }
    
    public void CloseAllNormalUI()
    {
        foreach(BaseUI ui in normal_dic_.Values)
        {
            Object.Destroy(ui.gameObject);
        }

        normal_dic_.Clear();
    }

    public void CloseAllDialogUI()
    {
        foreach(BaseUI ui in dialogs_)
        {
            Object.Destroy(ui.gameObject);
        }

        dialogs_.Clear();
    }   

    public void CloseAll()
    {
        CloseAllNormalUI();
        CloseAllDialogUI();
    }
    #endregion
}