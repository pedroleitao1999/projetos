using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraBillboard : MonoBehaviour
{
    private Camera cam;

    void Start(){
      cam = Camera.main;
    }

  //Orient the camera after all movement is completed this frame to avoid jittering
  void LateUpdate()
  {
      transform.LookAt(cam.transform);
  }

}
