using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MagicPlane : MonoBehaviour
{
    // Start is called before the first frame update

    public GameObject toReveal;
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    private void OnTriggerEnter(Collider other)
    {
        if(other.name == "sackboy")
        {
            toReveal.SetActive(true);
            StartCoroutine(setInactive());
        }
    }

    IEnumerator setInactive()
    {
        yield return new WaitForSeconds(2f);
        this.gameObject.SetActive(false);
        yield return new WaitForSeconds(4f);
        toReveal.SetActive(false);
    }
}
