using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameInventory: MonoBehaviour
{
  public static GameInventory instance;

  void Awake(){
    if(instance != null){
      return;
    }
    instance = this;
  }

  public int space = 20;

  public delegate void OnItemChanged();
  public OnItemChanged onItemChangedCallback;

  public List<Item> items = new List<Item>();

  public bool Add(Item item){
    if ( !item.isDefaultItem){
      if ( items.Count >= space){
        return false;
      }else{
        items.Add(item);
      }
      if ( onItemChangedCallback !=null)
        onItemChangedCallback.Invoke();
    }
    return true;
  }

  public void Remove(Item item){
    items.Remove(item);
    if ( onItemChangedCallback !=null)
      onItemChangedCallback.Invoke();
  }


}
