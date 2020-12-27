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
    [DllImport("MyGameClient")]
    public static extern void SetUnityDelegate(UnityDelegate unity_delegate);

    [DllImport("MyGameClient")]
    public static extern void InitGame(byte[] data,int size);

    [DllImport("MyGameClient")]
    public static extern void DestoryGame();

    [DllImport("MyGameClient")]
    public static extern void UpdateGame(float dt);

    public static void LoadDLL(string name)
    {
    }

    public static void FreeDLL()
    {
    }
#endif

    public static string UnityCallback(string func,string json_params)
    {
        object[] objs = LitJson.JsonMapper.ToObject<object[]>(json_params);

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

    public static int CreateView(string asset)
    {
        UnityViewServices.Instance.CreateView(asset);
        return 0;
    }

    public static void DestoryView(int id)
    {
        UnityViewServices.Instance.DestroyView(id);
    }

    public static void UpdatePosition(int id,float x,float y,float z)
    {
        //UnityViewServices.Instance.UpdateView(int)
    }
}
