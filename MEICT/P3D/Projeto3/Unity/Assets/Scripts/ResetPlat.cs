using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ResetPlat : MonoBehaviour
{
    // Start is called before the first frame update

    public GameObject platform;
    public GameObject platform2;
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    private void OnTriggerEnter(Collider other)
    {
        if (other.name == "sackboy")
        {
            platform.SetActive(true);
            platform2.SetActive(true);
        }
    }
}
