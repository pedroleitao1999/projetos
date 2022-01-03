package sth.core;

import java.io.IOException;
import java.io.StreamTokenizer;
import java.io.FileReader;
import java.io.BufferedReader;
import java.io.Reader;

import java.util.Collection;
import java.util.ArrayList;
import java.util.Iterator;

import sth.core.exception.BadEntryException;

public class Answer implements Comparable<Answer>, java.io.Serializable {

  /** Serial number for serialization. */
  private static final long serialVersionUID = 201810051538L;

  private int _hours;
  private String _message;

  public Answer(int hours, String message){
    _hours = hours;
    _message = message;
  }

  @Override
  public int compareTo(Answer outraResposta) {
     if (_hours < outraResposta.getHours()) {
          return -1;
     }
     if (_hours > outraResposta.getHours()) {
          return 1;
     }
     return 0;
  }

  public int getHours(){
    return _hours;
  }

  public String getMessage(){
    return _message;
  }

}
