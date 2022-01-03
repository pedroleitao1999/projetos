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

public class Course{
  private String _name;
  private ArrayList<Discipline> _disciplinas;
  private ArrayList<Student> _alunos;
  private ArrayList<Student> _delegados;
  private Discipline _disciplina;

  public Course(String name){
    _name = name;
    _disciplinas = new ArrayList<Discipline>();
    _alunos = new ArrayList<Student>();
    _delegados = new ArrayList<Student>();
  }

  public String getName(){
    return _name;
  }

  Discipline parseDiscipline(String disciplinaNome){
    Discipline _disciplina = null;

    Iterator<Discipline> iterator = _disciplinas.iterator();
    while(iterator.hasNext()){
      _disciplina = iterator.next();
      if(_disciplina.getName().equals(disciplinaNome)){
        return _disciplina;
      }
    }
    _disciplina = new Discipline(disciplinaNome, this);
    _disciplinas.add(_disciplina);
    return _disciplina;
  }

  void addStudent(Student aluno){
    _alunos.add(aluno);
    if(aluno.isRepresentative() == true){
      _delegados.add(aluno);
    }
    aluno.setCourse(this);
  }

  void addRepresentative(Student aluno){
    if((_delegados.size() < 7) && (aluno.isRepresentative() == false)){
      aluno.setRepresentative(true);
      _delegados.add(aluno);
    }
  }

  void removeRepresentative(Student aluno){
    if(aluno.isRepresentative() == true){
      aluno.setRepresentative(false);
      _delegados.remove(aluno);
    }
  }

  public ArrayList<Discipline> getDisciplineList(){
    return _disciplinas;
  }

}
