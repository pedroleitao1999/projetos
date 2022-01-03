using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class StartMenu : MonoBehaviour
{

  public GameObject startInfo;
  public GameObject optionsDisplay;


  public void StartGame(){
      SceneManager.LoadScene(SceneManager.GetActiveScene().buildIndex +1);
  }

  public void GoToOptionsMenu(){
    optionsDisplay.SetActive(true);
    startInfo.SetActive(false);
  }

  public void GoToBackStartMenu(){
    optionsDisplay.SetActive(false);
    startInfo.SetActive(true);
  }

  public void QuitGame(){
    Application.Quit();
  }
}
