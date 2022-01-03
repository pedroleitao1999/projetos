using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MovingPlatform : MonoBehaviour
{
    // Start is called before the first frame update

    public Vector3 min;
    public Vector3 max;
    private Vector3 diff;
    public float speed;
    bool movingToPositive = true;
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        if (Mathf.Abs(max.x - min.x)>0) {
            if (transform.position.x > max.x)
            {
                movingToPositive = false;

            }
            if (transform.position.x < min.x)
            {
                movingToPositive = true;

            }

            if (movingToPositive)
            {
                transform.position = new Vector3(transform.position.x + speed * Time.deltaTime, transform.position.y, transform.position.z);
            }
            else
            {
                transform.position = new Vector3(transform.position.x - speed * Time.deltaTime, transform.position.y, transform.position.z);
            }
        }else if(Mathf.Abs(max.y - min.y) > 0)
        {
            if (transform.position.y > max.y)
            {
                movingToPositive = false;

            }
            if (transform.position.y < min.y)
            {
                movingToPositive = true;

            }

            if (movingToPositive)
            {
                transform.position = new Vector3(transform.position.x, transform.position.y + speed * Time.deltaTime, transform.position.z);
            }
            else
            {
                transform.position = new Vector3(transform.position.x, transform.position.y - speed * Time.deltaTime, transform.position.z);
            }
        }
        else if (Mathf.Abs(max.z - min.z) > 0)
        {
            if (transform.position.z > max.z)
            {
                movingToPositive = false;

            }
            if (transform.position.z < min.z)
            {
                movingToPositive = true;

            }

            if (movingToPositive)
            {
                transform.position = new Vector3(transform.position.x, transform.position.y , transform.position.z + speed * Time.deltaTime);
            }
            else
            {
                transform.position = new Vector3(transform.position.x, transform.position.y , transform.position.z - speed * Time.deltaTime);
            }
        }


    }
}
