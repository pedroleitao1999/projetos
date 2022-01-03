using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RotatingPlatform : MonoBehaviour
{
    // Start is called before the first frame update
    public float y;
    public float speedY=20;
    bool flag = false;
    bool beingHandled = false;
    public float time;
    public Collider col;
    int counter1;
    int counter2;
    void Start()
    {
        counter1 = 0;
        counter2 = 0;
    }

    // Update is called once per frame
    void Update()
    {
        if (!beingHandled)
        {
            if (y >= 179)
            {
                counter2 = 0;
                flag = true;
                if (counter1 == 0)
                {
                    StartCoroutine(waitPls());
                }
                counter1++;
               
            }
            if (y <= 1)
            {
                counter1=0;
                flag = false;
                if (counter2 == 0)
                {
                    StartCoroutine(waitPls());
                }
                counter2++;
            }

            if (flag)
            {
                y -= Time.deltaTime * speedY;

            }
            else
            {
                y += Time.deltaTime * speedY;
            }

            transform.rotation = Quaternion.Euler(y, 0, 0);
        }
        
    }


    IEnumerator waitPls()
    {
        beingHandled = true;
        yield return new WaitForSeconds(time);
        beingHandled = false;

    }


}