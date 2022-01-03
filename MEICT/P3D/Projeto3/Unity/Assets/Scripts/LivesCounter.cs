using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;


public class LivesCounter : MonoBehaviour
{
    public RawImage[] lives;
    public int livesRemaining;
    public GameOver GameOver;

    // 5 lives = 5 images (0,1,2,3,4)
    // 4 lives = 4 images (0,1,2,3,[4]) [4] = hidden
    // 3 lives = 3 images (0,1,[2],[3],[4])
    // 2 lives = 2 images (0,1,[2],[3],[4])
    // 1 lives = 1 images (0,1,[2],[3],[4])
    // 0 lives = 0 images (0,1,[2],[3],[4]) -> loses

    public void LoseLife()
    {
      livesRemaining--;
      lives[livesRemaining].enabled = false; // hides life image
      if ( livesRemaining == 0){
        VariablesController.finalScore = GameObject.Find("sackboy").GetComponent<Score>().ScoreNum;
        SceneManager.LoadScene("EndScene");
      }

    }

    public int getLifeNumber()
    {
        return livesRemaining;
    }

    // Update is called once per frame

}
