using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class OptionsMenu : MonoBehaviour
{

  public void SetVolume(float volume){

  }

  public void GoToStartScene(){
      SceneManager.LoadScene("StartScene");
  }
}
