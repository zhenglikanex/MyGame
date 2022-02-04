using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FlyText : MonoBehaviour
{
    public TextMesh text_;
    public int value { get; set; }

    private float time = 0;

    // Start is called before the first frame update
    void Start()
    {
        text_ = GetComponent<TextMesh>();
        text_.text = value.ToString();
    }

    // Update is called once per frame
    void Update()
    {
        transform.position = new Vector3(transform.position.x, transform.position.y + 5.0f * Time.deltaTime, transform.position.z);

        time += Time.deltaTime;
        if (time >= 10.0f)
        {
            Destroy(gameObject);
        }
    }
}
