using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class PauseMenu : MonoBehaviour
{
    public bool IsPaused = false;
    public GameObject pauseMenuUI;
    public GameObject miniMap;
    public GameObject inventory;

    // Update is called once per frame
    void Update()
    {
      if (Input.GetKeyDown(KeyCode.Return)) // FIX
      {
        if(!IsPaused){
            Pause();
        }
        else
        {
            Resume();
        }
      }
    }

    public void Resume()
    {
      Time.timeScale = 1;
      pauseMenuUI.SetActive(false);
      miniMap.SetActive(true);
      IsPaused = false;
    }

     void Pause()
    {
      Time.timeScale = 0;
      pauseMenuUI.SetActive(true);
      miniMap.SetActive(false);
      IsPaused = true;
    }

    public void GoToInventory()
    {
      inventory.SetActive(true);
      pauseMenuUI.SetActive(false);
    }

    public void GoToPause(){
      inventory.SetActive(false);
      pauseMenuUI.SetActive(true);
    }

    public void RestartLevel()
    {
      Resume();
      SceneManager.LoadScene("SampleScene"); // FIX : Change to current level & checkpoint
    }

    public void QuitGame()
    {
      Resume();
      SceneManager.LoadScene("StartScene"); // FIX : Change to current level & checkpoint
    }


}
