using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SkyboxChanger : MonoBehaviour
{
    public Material daySkyBox;
    public Material nightSkyBox;
    GameObject[] allLightsOn;
    public bool isDay = false;
    public GameObject WorldLight;
    public GameObject WorldLight2;


    private void Start()
    {
        allLightsOn = GameObject.FindGameObjectsWithTag("streetLights");
    }


    private void OnTriggerEnter(Collider obj){

      
    GameObject[] allLightsOff= GameObject.FindGameObjectsWithTag("streetLightsOff");


    if(obj.name == "sackboy"){


        if (isDay)
        {
            isDay = false;
            RenderSettings.skybox = nightSkyBox;
            foreach (GameObject i in allLightsOn)
            {
                i.SetActive(true);
            }
            WorldLight.SetActive(false);
            WorldLight2.SetActive(false);

        }
        else
        {
            isDay = true;
            RenderSettings.skybox = daySkyBox;
            foreach (GameObject i in allLightsOn)
            {
                i.SetActive(false);
            }
            WorldLight.SetActive(true);
            WorldLight2.SetActive(true);

        }


        Destroy(gameObject);
      }
    }




    void turnNight()
    {
      RenderSettings.skybox = nightSkyBox;
    }
}
