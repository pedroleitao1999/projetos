using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ObjectExplosion : MonoBehaviour
{
    // Start is called before the first frame update

    public float radius;
    public float power;
    Rigidbody rb;
    public GameObject explosionVFX;
    GameObject explosion;
    public GameObject button;
    public GameObject player;
    private PlayerMovement playerMovement;
    ButtonExplosion script;
    float timer = 5f;
    public Sounds sound;
    void Start()
    {
        rb = this.GetComponent<Rigidbody>();
        StartCoroutine("ExplosionTimer");
        button = this.transform.parent.gameObject;
        script = button.GetComponent<ButtonExplosion>();
        player = GameObject.Find("sackboy");
        playerMovement = player.GetComponent<PlayerMovement>();
        sound = player.GetComponent<Sounds>();
    }

    public void setTimer(float time)
    {
        timer = time;
    }

    // Update is called once per frame
    void Update()
    {
        
    }
    IEnumerator ExplosionTimer()
    {
        yield return new WaitForSeconds(timer);
        Explode();
        Destroy(explosion,2);
    }

    public void Explode()
    {
        Vector3 explosionPos = this.transform.GetChild(0).gameObject.transform.position;
        Collider[] colliders = Physics.OverlapSphere(explosionPos, radius);
        explosion= Instantiate(explosionVFX, explosionPos, Quaternion.identity);
        script.resetCount();
        sound.Explosion();
      //  Debug.Log(explosionPos);
        foreach (Collider hit in colliders)
        {
            Rigidbody rb = hit.GetComponent<Rigidbody>();
            if(rb!= null)
            {
                rb.AddExplosionForce(power, explosionPos, radius, 3.0f);
            }

            //Debug.Log(hit);
            if(hit.gameObject.name == "sackboy")
            {
               
                playerMovement.Death();
            }
            
        }
        Destroy(this.gameObject);
    }

    private void OnDrawGizmos() //Draw the area of potential rocket damage
    {
        Gizmos.color = Color.red;
        Gizmos.DrawWireSphere(this.transform.GetChild(0).gameObject.transform.position, radius);
    }

    void OnCollisionEnter(Collision collision)
    {
        collision.transform.SendMessage("resetCount", SendMessageOptions.DontRequireReceiver);
    }
}
