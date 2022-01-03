package sth.core;

import java.io.IOException;
import java.io.StreamTokenizer;
import java.io.FileReader;
import java.io.BufferedReader;
import java.io.Reader;

import java.util.Collection;
import java.util.Collections;
import java.util.ArrayList;
import java.util.Iterator;

import sth.core.exception.BadEntryException;

public class Submission implements Comparable<Submission>, java.io.Serializable {

  /** Serial number for serialization. */
  private static final long serialVersionUID = 201810051538L;

  private String _mensagem;
  private Project _projecto;
  private int _studentId;

  public Submission(String mensagem, Project projecto, int alunoId){
    _mensagem = mensagem;
    _projecto = projecto;
    _studentId = alunoId;
  }

  @Override
  public int compareTo(Submission outraSubmissao) {
     if (_studentId < outraSubmissao.getStudentId()) {
          return -1;
     }
     if (_studentId > outraSubmissao.getStudentId()) {
          return 1;
     }
     return 0;
  }

  public String getMessage(){
    return _mensagem;
  }

  public void changeMessage(String mensagem){
    _mensagem = mensagem;
  }

  public int getStudentId(){
    return _studentId;
  }

}
