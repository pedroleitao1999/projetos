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

public class Student extends Person {

  private boolean _delegado;
  private Course _course;
  private ArrayList<Discipline> _disciplinas;

  public Student(int id, int phoneNumber, String name, boolean delegado){
      super(id, phoneNumber, name);
      _delegado = delegado;
      _disciplinas = new ArrayList<Discipline>();
  }

  @Override
  void parseContext(String lineContext, School school) throws BadEntryException {
    String components[] =  lineContext.split("\\|");

    if (components.length != 2)
      throw new BadEntryException("Invalid line context " + lineContext);

    if (_course == null) {
      _course = school.parseCourse(components[0]);
      _course.addStudent(this);
    }

    Discipline disciplina = _course.parseDiscipline(components[1]);

    disciplina.enrollStudent(this);
  }

  public void setCourse(Course curso){
      _course = curso;
  }

  Course getCourse(){
      return _course;
  }

  public void addDiscipline(Discipline disciplina){
    Course curso = disciplina.getCourse();
    if((_disciplinas.size() < 6) && (curso.equals(_course))){
        _disciplinas.add(disciplina);
    }
  }

  public ArrayList<Discipline> getDisciplineList(){
    return _disciplinas;
  }

  void setRepresentative(Boolean arg){
      _delegado = arg;
  }

  boolean isRepresentative(){
    return _delegado;
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

  public String toString(){
    String s = "";
    if(_delegado == false){
      s += "ALUNO|"+this.getId()+"|"+this.getPhoneNumber()+"|"+this.getName();
      Iterator<Discipline> iterator = _disciplinas.iterator();
      while(iterator.hasNext()){
        s+="\n";
        Discipline disciplina = iterator.next();
        s += "* "+_course.getName()+" - "+disciplina.getName();
      }
    }
    else{
      s += "DELEGADO|"+this.getId()+"|"+this.getPhoneNumber()+"|"+this.getName();
      Iterator<Discipline> iterator = _disciplinas.iterator();
      while(iterator.hasNext()){
        s+="\n";
        Discipline disciplina = iterator.next();
        s += "* "+_course.getName()+" - "+disciplina.getName();
      }
    }
    return s;
  }

}
