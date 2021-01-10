using System.Collections;
using System.Collections.Generic;
using System.Reflection;
using UnityEngine;
using System.Runtime.InteropServices;
using System;

public class UnityResult
{
    public object result;
}

public class CPlusPlusBridge : Singleton<CPlusPlusBridge>
{
#if !UNITY_EDITOR
    private static IntPtr nativeLibraryPtr;

    public delegate void InitGame(byte[] data, int size);
    public delegate void DestoryGame();
    public delegate void UpdateGame(float dt);

    public delegate void SetUnityDelegate(UnityDelegate unity_delegate);
    public delegate string UnityDelegate(string func, string json_params);
    public static void CSetUnityDelegate(UnityDelegate unity_delegate)
    {
        Native.Invoke<SetUnityDelegate>(nativeLibraryPtr, unity_delegate);
    }

    public static void CInitGame(byte[] data, int size)
    {
        Native.Invoke<InitGame>(nativeLibraryPtr,data,size);
    }

    public static void CDestoryGame()
    {
        Native.Invoke<DestoryGame>(nativeLibraryPtr);
    }

    public static void CUpdateGame(float dt)
    {
        Native.Invoke<UpdateGame>(nativeLibraryPtr,dt);
    }

    public static void LoadDLL(string name)
    {
        if (nativeLibraryPtr != IntPtr.Zero) return;

        nativeLibraryPtr = Native.LoadLibrary(name);
        if (nativeLibraryPtr == IntPtr.Zero)
        {
            Debug.LogError("Failed to load native library");
        }
    }

    public static void FreeDLL()
    {
        if (nativeLibraryPtr == IntPtr.Zero) return;

        Debug.Log(Native.FreeLibrary(nativeLibraryPtr)
                      ? "Native library successfully unloaded."
                      : "Native library could not be unloaded.");
    }

#else
    public delegate string UnityDelegate(string func,string json_params);
    [DllImport("ClientProxy")]
    public static extern void SetUnityDelegate(UnityDelegate unity_delegate);

    [DllImport("ClientProxy")]
    public static extern void InitGame(byte[] data,int size);

    [DllImport("ClientProxy")]
    public static extern void DestoryGame();

    [DllImport("ClientProxy")]
    public static extern void UpdateGame(float dt);

    [DllImport("ClientProxy")]
    public static extern void LoadDLL();

    [DllImport("ClientProxy")]
    public static extern void FreeDLL();

    [DllImport("ClientProxy")]
    public static extern void GameInput(byte[] data, int size);
#endif

    public static string UnityCallback(string func,string json_params)
    {
        object[] objs = { json_params };
        Type t = typeof(CPlusPlusBridge);
        MethodInfo method = t.GetMethod(func);
        if (method != null)
        {
            object result;
            try
            {
                result = method.Invoke(null, objs);
            } 
            catch(Exception e)
            {
                Debug.Log(func + json_params);
                Debug.LogError(e.ToString());
                result = null;
            }

            if(result != null)
            {
                UnityResult unity_result = new UnityResult();
                unity_result.result = result;
                string json_result = LitJson.JsonMapper.ToJson(unity_result);
                return json_result;
            }
        }
        return "";
    }

    public static int CreateView(string json)
    {
        object[] param = LitJson.JsonMapper.ToObject<object[]>(json);
        string asset = (string)param[0];
        UnityViewServices.Instance.CreateView(asset);
        return 0;
    }

    public static void DestoryView(string json)
    {
        object[] param = LitJson.JsonMapper.ToObject<object[]>(json);
        int id = (int)param[0];
        UnityViewServices.Instance.DestroyView(id);
    }

    public static void UpdatePosition(string json)
    {
        object[] param = LitJson.JsonMapper.ToObject<object[]>(json);
        int id = (int)param[0];
        float x = (float)param[1];
        float y = (float)param[2];
        float z = (float)param[3];

        UnityViewServices.Instance.UpdatePosition(id, x, y, z);
    }


    public static void MovePosition(string json)
    {
        object[] param = LitJson.JsonMapper.ToObject<object[]>(json);
        int id = (int)param[0];
        float x = Convert.ToSingle(param[1]);
        float y = Convert.ToSingle(param[2]);
        float z = Convert.ToSingle(param[3]);
        UnityViewServices.Instance.MovePosition(id, (float)x, (float)y, (float)z);
    }

    public static void UpdateForward(string json)
    {
        object[] param = LitJson.JsonMapper.ToObject<object[]>(json);
        int id = (int)param[0];
        float x = (float)param[1];
        float y = (float)param[2];
        float z = (float)param[3];
        UnityViewServices.Instance.UpdateForward(id, x, y, z);
    }

    public static void MoveForward(string json)
    {
        object[] param = LitJson.JsonMapper.ToObject<object[]>(json);
        int id = (int)param[0];
        float x = Convert.ToSingle(param[1]);
        float y = Convert.ToSingle(param[2]);
        float z = Convert.ToSingle(param[3]);
        UnityViewServices.Instance.MoveForward(id, x, y, z);
    }

    public static void InputHandler(string json)
    {
        BattleController.Instance.InputHandler();
    }

    public static void LogInfo(string json)
    {
        object[] param = LitJson.JsonMapper.ToObject<object[]>(json);
        string message = (string)param[0];
        Debug.Log(message);
    }

    public static void LogWarning(string json)
    {
        object[] param = LitJson.JsonMapper.ToObject<object[]>(json);
        string message = (string)param[0];
        Debug.LogWarning(message);
    }

    public static string OpenFile(string json)
    {
        object[] param = LitJson.JsonMapper.ToObject<object[]>(json);
        string file = (string)param[0];
        file = file.Substring(0, file.LastIndexOf("."));
        var obj = Resources.Load(file);
        if (obj)
        {
            return obj.ToString();
        }
        return "";
    }

    public static void LogError(string json)
    {
        object[] param = LitJson.JsonMapper.ToObject<object[]>(json);
        string message = (string)param[0];
        Debug.LogError(message);
    }

    
}
