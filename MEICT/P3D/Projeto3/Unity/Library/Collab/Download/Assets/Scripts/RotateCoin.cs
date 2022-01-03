using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RotateCoin : MonoBehaviour
{
    public int rotateSpeed;

    void Start(){
      rotateSpeed = 10;
    }

    // Update is called once per frame
    void FixedUpdate()
    {
        transform.Rotate(0, rotateSpeed, 0, Space.World);
    }

    private void OnTriggerEnter(Collider obj){
      if(obj.name == "sackboy"){
        obj.GetComponent<Score>().ScoreNum++;
        Destroy(gameObject);
      }
    }

}
