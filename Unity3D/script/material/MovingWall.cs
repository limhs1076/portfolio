using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MovingWall : MonoBehaviour
{
    float timer;
    
    bool flag;
    Vector3 start;

    [SerializeField]
    Vector3 end = new Vector3(0,0,3);
    public float speed = 3.0f;
    public int waitingTime = 2;



    void Start()
    {
        start = this.transform.position;
        end += this.transform.position;
        flag = true;
        timer = 0;
    }

    // Update is called once per frame
    void Update()
    {
        timer += Time.deltaTime;
        if (flag == true)
        {
            transform.position = Vector3.MoveTowards(transform.position, end, speed * Time.deltaTime);
        }
        else
        {
            transform.position = Vector3.MoveTowards(transform.position, start, speed * Time.deltaTime);
        }

        if (timer > waitingTime)
        {
            timer = 0;
            flag = !flag;
        }
    }
}
