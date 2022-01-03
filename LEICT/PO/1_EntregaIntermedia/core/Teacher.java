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

public class Teacher extends Person {

  private Course _curso;
  private ArrayList<Discipline> _disciplinas;

  public Teacher(int id, int phoneNumber, String name){
    super(id, phoneNumber, name);
    _disciplinas = new ArrayList<Discipline>();
  }

  @Override
   void parseContext(String lineContext, School school) throws BadEntryException {
     String components[] =  lineContext.split("\\|");

     if (components.length != 2)
       throw new BadEntryException("Invalid line context " + lineContext);

     Course course = school.parseCourse(components[0]);
     Discipline discipline = course.parseDiscipline(components[1]);

     discipline.addTeacher(this);
   }

  public ArrayList<Student> getStudentsOfDiscipline(Discipline disciplina){
    return disciplina.getAllStudents();
  }

  public ArrayList<Discipline> getDisciplineListProf(){
    return _disciplinas;
  }

  public void addDiscipline(Discipline disciplina){
    _disciplinas.add(disciplina);
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
    s += "DOCENTE|"+this.getId()+"|"+this.getPhoneNumber()+"|"+this.getName();

    for(Discipline d : _disciplinas){
      s+= "\n";
      s += "* "+d.getCourse().getName()+" - "+d.getName();
    }

    return s;
  }

  public void closeProject(Project projecto){
     projecto.close();
  }

  public void openProject(String nome, String descricao, Discipline disciplina){
    disciplina.createProject(nome, descricao);
  }

  public void openProject(String nome, Discipline disciplina){
   disciplina.createProject(nome);
 }

}
