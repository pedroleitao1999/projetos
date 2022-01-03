package sth.core;

import java.io.IOException;
import java.io.StreamTokenizer;
import java.io.FileReader;
import java.io.BufferedReader;
import java.io.Reader;

import java.util.Collection;
import java.util.Collections;
import java.util.ArrayList;

import sth.core.exception.BadEntryException;

public abstract class Person implements Comparable<Person>, java.io.Serializable {

  /** Serial number for serialization. */
  private static final long serialVersionUID = 201810051538L;

  int _id;
  String _name;
  int _phoneNumber;

  public Person(int id, int phoneNumber, String nome){
    _id = id;
    _name = nome;
    _phoneNumber = phoneNumber;
  }

  @Override
  public int compareTo(Person outraPessoa) {
     if (_id < outraPessoa.getId()) {
          return -1;
     }
     if (_id > outraPessoa.getId()) {
          return 1;
     }
     return 0;
  }

  void parseContext(String context, School school) throws BadEntryException {
    throw new BadEntryException("Should not have extra context: " + context);
  }

  public abstract String toString();

  public String getName(){
    return _name;
  }

  public int getId(){
    return _id;
  }

  public int getPhoneNumber(){
    return _phoneNumber;
  }

  public void setPhoneNumber(int numero){
    _phoneNumber = numero;
  }

}
