using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class InventorySlot : MonoBehaviour
{
    public GameObject player;
    public Image icon;
    public Button removeButton;
    Item item;

    public void AddItem(Item newItem){
      item = newItem;
      icon.sprite = item.icon;
      icon.enabled = true;
      removeButton.interactable = true;
    }

    public void ClearSlot(){
      item = null;
      icon.sprite = null;
      icon.enabled = false;
      removeButton.interactable = false;
    }

    public void OnSelectButtonn(){
      GameObject childMat;
      childMat = GameObject.Find("rdmobj00.001");
        player.GetComponent<Sounds>().ChangeClothes();  
      if(item.name == "WorkOut"){
        Texture text = Resources.Load<Texture>("ExtraTextures/workout");
        childMat.GetComponent<SkinnedMeshRenderer>().material.mainTexture = text;
        //GameInventory.instance.Remove(item);
      }
      else if (item.name == "Skeleton"){
          Texture text = Resources.Load<Texture>("ExtraTextures/skeleton");
          childMat.GetComponent<SkinnedMeshRenderer>().material.mainTexture = text;
          //GameInventory.instance.Remove(item);
      }
      else if (item.name == "Monkey"){
        Texture text = Resources.Load<Texture>("ExtraTextures/data.farc.799_1");
        childMat.GetComponent<SkinnedMeshRenderer>().material.mainTexture = text;
        //GameInventory.instance.Remove(item);
      }
      else if (item.name == "Wire"){
        Texture text = Resources.Load<Texture>("ExtraTextures/data.farc.877_1");
        childMat.GetComponent<SkinnedMeshRenderer>().material.mainTexture = text;
        //GameInventory.instance.Remove(item);
      }
      else if (item.name == "GreenPurple"){
        Texture text = Resources.Load<Texture>("ExtraTextures/data.farc.531_1");
        childMat.GetComponent<SkinnedMeshRenderer>().material.mainTexture = text;
        //GameInventory.instance.Remove(item);
      }
      else if (item.name == "RedYellow"){
        Texture text = Resources.Load<Texture>("ExtraTextures/data.farc.840_1");
        childMat.GetComponent<SkinnedMeshRenderer>().material.mainTexture = text;
        //GameInventory.instance.Remove(item);
      }
      else {
        return;
      }

    }

    public void OnRemoveButton(){
      GameInventory.instance.Remove(item);
    }



}
