using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MoveToPlatform : MonoBehaviour
{
    Vector3 start;

    [SerializeField]
    Vector3 end;
    float speed = 5.0f;
    bool trigger;

    void Start()
    {
        start = this.transform.position;
        end += this.transform.position;
        trigger = false;
    }

    // Update is called once per frame
    void Update()
    {
        if (trigger)
        {
            transform.position = Vector3.MoveTowards(transform.position, end, speed * Time.deltaTime);
        }
        else
        {
            transform.position = Vector3.MoveTowards(transform.position, start, speed * Time.deltaTime);
        }
    }

    public void MoveToDest()
    {
        trigger = true;
    }

    public void MoveToStart()
    {
        trigger = false;
    }
}
