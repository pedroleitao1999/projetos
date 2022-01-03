using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SpawnCyllinders : MonoBehaviour
{
    // Start is called before the first frame update
    public GameObject sackBoy;
    public FireCyllinder f;
    bool flag = true;
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        if(sackBoy.transform.position.z> 312 && sackBoy.transform.position.z< 550)
        {
            if (flag)
            {
                Instantiate(f, new Vector3(-4.3f, 37.5f, 464.6f), Quaternion.Euler(0,0,90));
                StartCoroutine(changeFlag());
            }
        }
            
    }

    IEnumerator changeFlag()
    {
        flag = false;
        yield return new WaitForSeconds(4f);
        flag = true;
    }
}
