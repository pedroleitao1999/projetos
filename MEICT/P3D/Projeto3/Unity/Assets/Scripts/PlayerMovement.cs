using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerMovement : MonoBehaviour

{

    public GameObject Player;
    public GameObject UI;
    public LivesCounter livesCounter;
    CharacterController characterController;
    public Animator anim;
    public float speed = 6.0f;
    public float jumpSpeed = 20.0f;
    public float gravity = 20f;
    public float pushPower = 2.0F;
    bool death = false;
    bool jumpflag = false;
    Sounds sounds;
    private Vector3 moveDirection = Vector3.zero;

    float VerticalMovement;
    float HorizontalMovement;
    public Transform cam;
    float angle;
    bool check;
    Quaternion targetRotation;
    public float turnSpeed = 10.0f;
    public Vector3 currentTeleport;
    // Start is called before the first frame update
    void Start()
    {
        characterController = Player.GetComponent<CharacterController>();
        float correctHeight = characterController.center.y + characterController.skinWidth;
       // characterController.center = new Vector3(0, correctHeight, 0);
        anim = GetComponent<Animator>();
        sounds = Player.GetComponent<Sounds>();
        livesCounter = Player.GetComponent<LivesCounter>();
    }

    // Update is called once per frame
    void Update()
    {
        if (!death)
        {
            VerticalMovement = Input.GetAxis("Vertical");
            HorizontalMovement = Input.GetAxis("Horizontal");
        }

        if (!Input.GetButton("Jump"))
        {
            jumpflag = true;
        }
        
        //= checkGrounded();

        bool groundedPlayer = checkGrounded();
        // Debug.Log(groundedPlayer);
        if (Input.GetButton("Jump") && jumpflag && groundedPlayer && !death)
        {
            jumpflag = false;
            anim.SetBool("Jump", true);
            moveDirection.y = jumpSpeed;
        }


        if (groundedPlayer && moveDirection.y <= 0)
        {
            
            moveDirection.y = 0;
            anim.SetBool("Jump", false);
        }
        else
        {
            moveDirection.y -= gravity * Time.deltaTime;
        }
        Vector3 movement = (cam.right * HorizontalMovement + cam.forward * VerticalMovement);
        if (movement.magnitude >= 1)
        {
            anim.SetFloat("Speed", 1);
        }
        else
        {
            anim.SetFloat("Speed", movement.magnitude);
        }



        if (Mathf.Abs(HorizontalMovement) > 0.2 || Mathf.Abs(VerticalMovement) > 0.2)
        {
            CalculateDirection();
            Rotate();
        }
        //Physics.SyncTransforms();
        if(transform.position.x<= -9)
        {
            if (movement.x > 0)
            {
                characterController.Move((moveDirection + movement * speed) * Time.deltaTime);
            }
            else
            {
                movement = new Vector3(0, movement.y, movement.z);
                characterController.Move((moveDirection + movement * speed) * Time.deltaTime);
            }
            
        }
        else if(transform.position.x >= 6)
        {
            if (movement.x < 0)
            {
                
                characterController.Move((moveDirection + movement * speed) * Time.deltaTime);
            }
            else
            {
                movement = new Vector3(0, movement.y, movement.z);
                characterController.Move((moveDirection + movement * speed) * Time.deltaTime);
            }
        }
        else
        {
            characterController.Move((moveDirection + movement * speed) * Time.deltaTime);
        }
            
    }

    void Rotate()
    {
        targetRotation = Quaternion.Euler(0, angle, 0);


        transform.rotation = Quaternion.Slerp(transform.rotation, targetRotation, turnSpeed * Time.deltaTime);
    }

    void CalculateDirection()
    {
        angle = Mathf.Atan2(HorizontalMovement, VerticalMovement);
        angle = Mathf.Rad2Deg * angle;
        angle += cam.eulerAngles.y;
    }

    bool checkGrounded()
    {
        Vector3 right = new Vector3(0.4f, 0, -0.5f);
        Vector3 left = new Vector3(-0.4f, 0, -0.5f);
        Vector3 right1 = new Vector3(0.4f, 0, 0.5f);
        Vector3 left1 = new Vector3(-0.4f, 0, 0.5f);
        
        check = Physics.Raycast(characterController.bounds.center, Vector3.down, 1.3f);
        if (!check)
        {
            check = Physics.Raycast(characterController.bounds.center + right, Vector3.down, 1.3f);
        }
        if (!check)
        {
            check = Physics.Raycast(characterController.bounds.center + left, Vector3.down, 1.3f);
        }
        if (!check)
        {
            check = Physics.Raycast(characterController.bounds.center + left1, Vector3.down, 1.3f);
        }
        if (!check)
        {
            check = Physics.Raycast(characterController.bounds.center + right1, Vector3.down, 1.3f);
        }
       // Debug.Log(characterController.bounds.extents.y);
        Debug.DrawRay(characterController.bounds.center, Vector3.down * 1.3f, Color.green);
        Debug.DrawRay(characterController.bounds.center + left, Vector3.down * 1.3f, Color.red);
        Debug.DrawRay(characterController.bounds.center + right, Vector3.down * 1.3f, Color.blue);
        Debug.DrawRay(characterController.bounds.center + left1, Vector3.down * 1.3f, Color.yellow);
        Debug.DrawRay(characterController.bounds.center + right1, Vector3.down * 1.3f, Color.black);
        return check;

    }

    void OnControllerColliderHit(ControllerColliderHit hit)
    {
        if (hit.transform.tag == "ExplosionButton")
        {
            hit.transform.SendMessage("Explosion", SendMessageOptions.DontRequireReceiver);
        }

        Rigidbody body = hit.collider.attachedRigidbody;
        if (body == null || body.isKinematic)
            return;
        if(hit.transform.tag != "NotMovable")
        {
            Vector3 pushDir = hit.moveDirection;
            body.velocity = pushDir * pushPower;
        }
        

    }
    public void Respawn()
    {
       
        if (livesCounter.getLifeNumber() > 0)
        {
            TeleportToCheckPoint();
            anim.SetBool("Death", false);
            death = false;
            if (livesCounter.getLifeNumber() == 1)
            {
                sounds.StartCoroutine("LastLife");
            }
            else
            {
                sounds.TpRespawn();
            }
            
        }

    }

    public void Death()
    {
      //  this.transform.parent = null;
      //  this.transform.localScale = new Vector3(1, 1, 1);
        anim.SetBool("Death", true);
        VerticalMovement = 0;
        HorizontalMovement = 0;
        death = true;
        livesCounter.LoseLife();
        
    }

    public void TeleportToCheckPoint()
    {
      //  Debug.Log(Player.transform.position);
        Player.transform.position = currentTeleport;
      //  Debug.Log(Player.transform.position);
    }

    public void setTeleport(Vector3 tp)
    {
        currentTeleport = tp;
    }

    private void OnTriggerEnter(Collider other)
    {
        if (other.transform.tag == "MovingPlatform")
        {
            this.transform.SetParent(other.transform, true);
        }
        if (other.gameObject.layer == 10)
        {
            sounds.Spike();
            Death();

        }
        if (other.gameObject.tag == "Bullet" && !death)
        {
            Death();
        }

        if (other.gameObject.layer == 8 && !death)
        {
            Death();

        }

    }
    private void OnTriggerExit(Collider other)
    {
        if (other.transform.tag == "MovingPlatform")
        {
            this.transform.parent = null;
            this.transform.localScale = new Vector3(1, 1, 1);
        }
    }
}
