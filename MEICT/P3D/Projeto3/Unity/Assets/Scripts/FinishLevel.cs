using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class FinishLevel : MonoBehaviour
{

    private void OnTriggerEnter(Collider obj){

        if(obj.name == "sackboy"){
          VariablesController.finalScore = GameObject.Find("sackboy").GetComponent<Score>().ScoreNum;
          SceneManager.LoadScene("Completed");

        }

    }

}
