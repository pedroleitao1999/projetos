using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;

public class Score : MonoBehaviour
{
    public TextMeshProUGUI ScoreText;
    public int ScoreNum = 0;

    // Start is called before the first frame update
    void Start()
    {
      ScoreText.text = "Score: " + ScoreNum;
    }

    // Update is called once per frame
    void Update()
    {
      ScoreText.text = "Score: " + ScoreNum;
    }

}
