using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Bullet : MonoBehaviour
{
    // Start is called before the first frame update
    Rigidbody rb;
    GameObject target;
    Vector3 moveDirection;
    public float movespeed=4f;
    void Start()
    {
      //  rb = this.gameObject.GetComponent<Rigidbody>();
        target = GameObject.Find("sackboy");
        moveDirection = (target.transform.position - transform.position).normalized * movespeed ;
        
        Destroy(this.gameObject, 3f);
    }

    // Update is called once per frame
    void Update()
    {
        transform.position += moveDirection * Time.deltaTime;
    }
}
