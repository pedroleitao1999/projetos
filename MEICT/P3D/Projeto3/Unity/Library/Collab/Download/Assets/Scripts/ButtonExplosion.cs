using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ButtonExplosion : MonoBehaviour
{
    // Start is called before the first frame update
    public GameObject button;
    public ObjectExplosion objectToExplode;
    public GameObject SackBoy;
    public float ObjectHeight;
    Vector3 height;
    private int count = 0;
    void Start()
    {
        height = new Vector3(0, ObjectHeight, 0);
        
    }

    
    public void Explosion()
    {
        count++;
        if (count == 1)
        {
            Instantiate(objectToExplode, transform.position + height, transform.rotation, this.transform);
        }
    }

    public void resetCount()
    {
        count = 0;
    }

    

    
}
