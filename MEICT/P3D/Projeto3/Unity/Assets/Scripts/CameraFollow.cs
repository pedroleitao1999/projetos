using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraFollow : MonoBehaviour
{
    // Start is called before the first frame update
    public Transform target;
    public Vector3 offset;
    public float speed = 10f;
    void Start()
    {
        
    }

    // Update is called once per frame
    void LateUpdate()
    {
       Vector3 desiredPosition = new Vector3(0,target.position.y, target.position.z )+ offset;
       Vector3 smothedPosition = Vector3.Lerp(transform.position, desiredPosition, speed * Time.deltaTime);
       transform.position = smothedPosition;
    }
}
