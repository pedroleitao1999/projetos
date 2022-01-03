using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class InstantiateFire : MonoBehaviour
{
    public GameObject FireVFX;
    private Collider col;
    private Vector3 _min;
    private Vector3 _max;
    private Vector3 diff;
    public int numberOfVFX = 20;
    // Start is called before the first frame update
    void Start()
    {
        col = this.GetComponent<BoxCollider>();
        _min = col.bounds.min;
        _max = col.bounds.max;
        diff = _max - _min;

      //  Debug.Log(transform.position);
        // Debug.Log(_max);

          for(int i=0; i <= numberOfVFX; i++)
              {
              for(int j=0; j <= numberOfVFX; j++)
              {
                  Instantiate(FireVFX, new Vector3(_min.x + i* (diff.x/ numberOfVFX), transform.position.y, _min.z+ j * (diff.z / numberOfVFX)), Quaternion.identity);
              }
          }

    }

    // Update is called once per frame
    void Update()
    {
       
    }
}
