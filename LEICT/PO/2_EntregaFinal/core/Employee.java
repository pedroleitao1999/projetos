package sth.core;

import java.io.IOException;
import java.io.StreamTokenizer;
import java.io.FileReader;
import java.io.BufferedReader;
import java.io.Reader;

import java.util.Collection;
import java.util.ArrayList;

import sth.core.exception.BadEntryException;

public class Employee extends Person implements java.io.Serializable {

  /** Serial number for serialization. */
  private static final long serialVersionUID = 201810051538L;

  public Employee(int id, int phoneNumber, String name){
      super(id, phoneNumber, name);
  }

  public String getName(){
      return _name;
  }

  public int getId(){
          return _id;
  }

  public int getPhoneNumber(){
      return _phoneNumber;
  }

  @Override
  public String toString(){
    String s = "";
    s += "FUNCIONÁRIO|"+this.getId()+"|"+this.getPhoneNumber()+"|"+this.getName();
    return s;
  }
}
