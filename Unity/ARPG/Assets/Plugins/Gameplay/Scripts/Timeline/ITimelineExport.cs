using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Playables;
using UnityEngine.Timeline;

namespace GameplayEditor
{
    internal interface ITimelineExport
    {
#if UNITY_EDITOR
        TimelineExportManager.Event Export(TimelineClip clip);
#endif
    }
}