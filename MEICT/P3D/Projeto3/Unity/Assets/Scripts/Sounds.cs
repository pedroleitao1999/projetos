using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Sounds : MonoBehaviour
{
    // Start is called before the first frame update
    public AudioClip jumpClip;
    public AudioClip CollectBubbleClip;
    public AudioClip WearCostumeClip;
    public AudioClip LastLifeClip;
    public AudioClip CoinClip;
    public AudioClip CheckpointClip;
    public AudioClip RespawnClip;
    public AudioClip spikeClip;
    public AudioClip explosionClip;
    AudioSource audioSource;
    void Awake()
    {
        audioSource = GetComponent<AudioSource>();
    }

    // Update is called once per frame
    void Update()
    {
        
    }
    public void Jump()
    {
        audioSource.PlayOneShot(jumpClip);
    }
    public void Spike()
    {
        audioSource.PlayOneShot(spikeClip);
    }

    public void CollectBubble()
    {
        audioSource.PlayOneShot(CollectBubbleClip);
    }

    public void FireDeath()
    {
        audioSource.PlayOneShot(CollectBubbleClip);
    }

    public void TpRespawn()
    {
        
        audioSource.PlayOneShot(RespawnClip);
    }

    public void TpChekpoint()
    {
        audioSource.PlayOneShot(CheckpointClip);
    }

    public void ChangeClothes()
    {
        audioSource.PlayOneShot(WearCostumeClip);
    }

    IEnumerator LastLife()
    {
        
        for (int i = 0; i < 4; i++)
        {
            audioSource.PlayOneShot(LastLifeClip);
            yield return new WaitForSeconds(LastLifeClip.length-0.4f);
        }
        
    }

    public void CatchCoin()
    {
        audioSource.PlayOneShot(CoinClip);
    }
    public void Explosion()
    {
        audioSource.PlayOneShot(explosionClip);
    }


}
