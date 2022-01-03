using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Teleport : MonoBehaviour
{
    // Start is called before the first frame update
    public GameObject Character;
    private PlayerMovement script;
    public GameObject TP;
    private int count = 0;
    public Material checkpointMaterial;
    void Start()
    {
        script = Character.GetComponent<PlayerMovement>();
    }

    // Update is called once per frame
    void Update()
    {
        if(TP.transform.position.z < Character.transform.position.z && count ==0)
        {
           
            script.setTeleport(new Vector3(0, TP.transform.position.y, TP.transform.position.z));
            TP.GetComponent<MeshRenderer>().material = checkpointMaterial;
            count++;
        }
    }
}
