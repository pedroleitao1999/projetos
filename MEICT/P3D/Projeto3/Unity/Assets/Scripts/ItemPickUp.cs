using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ItemPickUp : MonoBehaviour
{
    private GameInventory inventory;
    public GameObject sackboy;
    private Sounds sounds;
    public Item item;

    void Start(){
        inventory = sackboy.GetComponent<GameInventory>();
        sounds = sackboy.GetComponent<Sounds>();
    }

    private void OnTriggerEnter(Collider obj){
        if(obj.name == "sackboy"){
      
            bool wasPickedup = GameInventory.instance.Add(item);
            sounds.CollectBubble();
        if(wasPickedup)
            Destroy(gameObject);

        }

    }
}
