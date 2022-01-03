using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FireCyllinder : MonoBehaviour
{
    // Start is called before the first frame update

    public GameObject FireVFX;
    GameObject explosion;
    GameObject explosion1;
    GameObject explosion2;
    Rigidbody rb;
    void Start()
    {
        explosion = Instantiate(FireVFX, this.transform.position, Quaternion.identity);
        StartCoroutine(destroyThis());
        rb = this.GetComponent<Rigidbody>();
    }

    // Update is called once per frame
    void Update()
    {
        explosion.transform.position = this.transform.position;
        
        rb.velocity = new Vector3(this.GetComponent<Rigidbody>().velocity.x , -40, -20);
    }

    private void OnTriggerEnter(Collider other)
    {
        if (other.name == "sackboy")
        {
            other.gameObject.GetComponent<PlayerMovement>().Death();
        }
    }

    IEnumerator destroyThis()
    {
        yield return new WaitForSeconds(10f);
        Destroy(explosion);
        Destroy(this.gameObject);
    }
}
