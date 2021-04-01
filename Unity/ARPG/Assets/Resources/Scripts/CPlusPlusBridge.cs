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
//#if !UNITY_EDITOR
//    private static IntPtr nativeLibraryPtr;

//    public delegate void InitGame(byte[] data, int size);
//    public delegate void DestoryGame();
//    public delegate void UpdateGame(float dt);

//    public delegate void SetUnityDelegate(UnityDelegate unity_delegate);
//    public delegate string UnityDelegate(string func, string json_params);
//    public static void CSetUnityDelegate(UnityDelegate unity_delegate)
//    {
//        Native.Invoke<SetUnityDelegate>(nativeLibraryPtr, unity_delegate);
//    }

//    public static void CInitGame(byte[] data, int size)
//    {
//        Native.Invoke<InitGame>(nativeLibraryPtr,data,size);
//    }

//    public static void CDestoryGame()
//    {
//        Native.Invoke<DestoryGame>(nativeLibraryPtr);
//    }

//    public static void CUpdateGame(float dt)
//    {
//        Native.Invoke<UpdateGame>(nativeLibraryPtr,dt);
//    }

//    public static void LoadDLL(string name)
//    {
//        if (nativeLibraryPtr != IntPtr.Zero) return;

//        nativeLibraryPtr = Native.LoadLibrary(name);
//        if (nativeLibraryPtr == IntPtr.Zero)
//        {
//            Debug.LogError("Failed to load native library");
//        }
//    }

//    public static void FreeDLL()
//    {
//        if (nativeLibraryPtr == IntPtr.Zero) return;

//        Debug.Log(Native.FreeLibrary(nativeLibraryPtr)
//                      ? "Native library successfully unloaded."
//                      : "Native library could not be unloaded.");
//    }

//#else
    public delegate string UnityDelegate(string func, string json_params);
    [DllImport("ClientProxy")]
    public static extern void SetUnityDelegate(UnityDelegate unity_delegate);

    [DllImport("ClientProxy")]
    public static extern void InitGame(byte[] data, int size);

    [DllImport("ClientProxy")]
    public static extern void DestoryGame();

    [DllImport("ClientProxy")]
    public static extern void UpdateGame(float dt);

    [DllImport("ClientProxy")]
    public static extern void GameRollback();

    [DllImport("ClientProxy")]
    public static extern void LoadDLL();

    [DllImport("ClientProxy")]
    public static extern void FreeDLL();

    [DllImport("ClientProxy")]
    public static extern void GameInput(byte[] data, int size);
//#endif

    public static string UnityCallback(string func, string json_params)
    {
        object[] objs = null;
        if (json_params != "")
        {
            objs = LitJson.JsonMapper.ToObject<object[]>(json_params);
        }
        Type t = typeof(CPlusPlusBridge);
        MethodInfo method = t.GetMethod(func);
        if (method != null)
        {
            object result;
            try
            {
                //Debug.LogError(string.Format("name : {0} params :{1}", func, json_params));
                result = method.Invoke(null, objs);
            }
            catch (Exception e)
            {
                Debug.LogError(string.Format("name : {0} params :{1}", func,json_params));
                Debug.LogError(e.ToString());
                result = null;
            }

            if (result != null)
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
        var view = UnityViewServices.Instance.CreateView(asset);
        if(view)
        {
            return view.Id;
        }
        return -1;
    }

    public static void DestoryView(int id)
    {
        UnityViewServices.Instance.DestroyView(id);
    }

    public static void UpdatePosition(int id, double x, double y, double z)
    {
        UnityViewServices.Instance.UpdatePosition(id, (float)x, (float)y, (float)z);
    }

    public static void MovePosition(int id, double x, double y, double z)
    {
        UnityViewServices.Instance.MovePosition(id, (float)x, (float)y, (float)z);
    }

    public static void UpdateRotation(int id, double x, double y, double z, double w)
    {
        UnityViewServices.Instance.UpdateRotation(id, (float)x, (float)y, (float)z,(float)w);
    }

    public static void MoveForward(int id, double x, double y, double z,double w)
    {
        UnityViewServices.Instance.MoveRotation(id, (float)x, (float)y, (float)z,(float)w);
    }
    
    public static void PlayAnimation(int id,string str)
    {
        UnityViewServices.Instance.PlayAnim(id, str);
    }

    public static void PlayAnimationToTime(int id, string str,double time)
    {
        UnityViewServices.Instance.PlayAnim(id, str,(float)time);
    }

    public static void InputHandler()
    {
        BattleController.Instance.InputHandler();
    }

    public static void LogInfo(string message)
    {
        Debug.Log(message);
    }

    public static void LogWarning(string message)
    {
        Debug.LogWarning(message);
    }

    public static void LogError(string message)
    {
        Debug.LogError(message);
    }

    public static void DrawCube(
        double pos_x, double pos_y, double pos_z,
        double rot_x, double rot_y, double rot_z, double rot_w,
        double size_x, double size_y, double size_z)
    {
        DebugManager.Instance.DrawCube(
            new Vector3((float)pos_x, (float)pos_y, (float)pos_z),
            new Quaternion((float)rot_x, (float)rot_y, (float)rot_z, (float)rot_w),
            new Vector3((float)size_x, (float)size_y, (float)size_z));
    }

    public static void DrawSphere(
        double pos_x, double pos_y, double pos_z,
        double radius)
    {
        DebugManager.Instance.DrawSphere(
            new Vector3((float)pos_x, (float)pos_y, (float)pos_z),
            (float)radius);
    }

    public static void DrawCapsule(
        double pos_x, double pos_y, double pos_z,
        double a_x,double a_y,double a_z,
        double b_x, double b_y, double b_z,
        double radius)
    {
        DebugManager.Instance.DrawCapsule(
            new Vector3((float)pos_x, (float)pos_y, (float)pos_z),
            new Vector3((float)a_x, (float)a_y, (float)a_z),
            new Vector3((float)b_x, (float)b_y, (float)b_z),
            (float)radius);
    }

    public static string OpenFile(string file)
    {
        file = file.Substring(0, file.LastIndexOf("."));
        var obj = Resources.Load(file);        
        if (obj)
        {
            Debug.Log(obj.ToString());
            return obj.ToString();
        }
        return "";
    }
}
