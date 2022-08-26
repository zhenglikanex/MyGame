using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Playables;
using UnityEngine.Timeline;

namespace GameplayEditor
{
    internal interface ITimelineExport
    {
        TimelineExportManager.Event Export(TimelineClip clip);
    }
}