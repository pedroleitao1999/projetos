using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
using TMPro;


public class GameOver : MonoBehaviour
{
    public int totalScore = 0;
    public TextMeshProUGUI ScoreText;
    public Score score;
    public GameObject GameObject;
    // Start is called before the first frame update
    void Start()
    {
        score = GameObject.Find("Sackboy").GetComponent<Score>();
        totalScore = score.ReturnScoreNum();
        ScoreText.text = "Score: " + totalScore;
    }

    public void RestartGame(){
        SceneManager.LoadScene("SampleScene");
    }
    public void MainMenu(){
        SceneManager.LoadScene("StartScene");
    }


}
