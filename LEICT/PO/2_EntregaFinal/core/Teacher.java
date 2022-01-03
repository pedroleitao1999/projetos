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

public class Teacher extends Person implements java.io.Serializable {

  /** Serial number for serialization. */
  private static final long serialVersionUID = 201810051538L;

  private ArrayList<Discipline> _disciplinas;
  private ArrayList<Course> _cursos;
  protected ArrayList<String> _notificacoes;
  private Notification _notificacao = null;

  public Teacher(int id, int phoneNumber, String name){
    super(id, phoneNumber, name);
    _disciplinas = new ArrayList<Discipline>();
    _cursos = new ArrayList<Course>();
    _notificacoes = new ArrayList<String>();
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

  public ArrayList<Course> getCourseListProf(){
    for(Discipline d : _disciplinas){
      boolean cursoExiste = false;
      Course c = d.getCourse();
      for(Course outro : _cursos){
        if(outro.equals(c)){
          cursoExiste = true;
          break;
        }
      }
      if(cursoExiste == false){
        _cursos.add(c);
      }

    }
    return _cursos;
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

  @Override
  public String toString(){
    getCourseListProf();
    Collections.sort(_cursos);
    Collections.sort(_disciplinas);
    String s = "";
    s += "DOCENTE|"+this.getId()+"|"+this.getPhoneNumber()+"|"+this.getName();

    for(Course c : _cursos){
      Collections.sort(c.getDisciplineList());
      for(Discipline d : c.getDisciplineList()){
        for(Discipline dProf : _disciplinas){
          if(d.equals(dProf)){
            s += "\n";
            s += "* "+d.getCourse().getName()+" - "+d.getName();

          }
        }
      }
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

  public void addNotification(String notificacao){
    _notificacoes.add(notificacao);
  }

  public ArrayList<String> getNotificationList(){
    return _notificacoes;
  }

  public void removeNotifications(){
    _notificacoes = new ArrayList<String>();
  }

}
