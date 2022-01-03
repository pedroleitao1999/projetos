package sth.core;

import java.io.IOException;
import java.io.StreamTokenizer;
import java.io.FileReader;
import java.io.BufferedReader;
import java.io.Reader;

import java.util.Collection;
import java.util.ArrayList;

import sth.core.exception.BadEntryException;

public abstract class Person{

  int _id;
  String _name;
  int _phoneNumber;

  public Person(int id, int phoneNumber, String nome){
    _id = id;
    _name = nome;
    _phoneNumber = phoneNumber;
  }

  void parseContext(String context, School school) throws BadEntryException {
    throw new BadEntryException("Should not have extra context: " + context);
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

  public void setPhoneNumber(int numero){
    _phoneNumber = numero;
  }

}
