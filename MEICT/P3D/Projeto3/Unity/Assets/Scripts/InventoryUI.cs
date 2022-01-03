using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class InventoryUI : MonoBehaviour
{
    public Transform itemsParent;
    GameInventory inventory;

    InventorySlot[] slots;

    void Start(){
      inventory = GameInventory.instance;
      inventory.onItemChangedCallback += UpdateUI;

      slots = itemsParent.GetComponentsInChildren<InventorySlot>();

    }

    void Update(){

    }
    void UpdateUI(){

      for (int i = 0; i < slots.Length; i++){
         if(i < inventory.items.Count){ // item can be added
          slots[i].AddItem(inventory.items[i]);
        }else {
          slots[i].ClearSlot();
        }
      }

    }

}
