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

public class Student extends Person implements java.io.Serializable {

  /** Serial number for serialization. */
  private static final long serialVersionUID = 201810051538L;

  private boolean _delegado;
  private Course _course;
  private ArrayList<Discipline> _disciplinas;
  private ArrayList<Project> _projSubmetidos;
  private ArrayList<Survey> _inqueRespondidos;
  protected ArrayList<String> _notificacoes;
  private Notification _notificacao = null;

  public Student(int id, int phoneNumber, String name, boolean delegado){
      super(id, phoneNumber, name);
      _delegado = delegado;
      _disciplinas = new ArrayList<Discipline>();
      _projSubmetidos = new ArrayList<Project>();
      _inqueRespondidos = new ArrayList<Survey>();
      _notificacoes = new ArrayList<String>();
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

  public Course getCourse(){
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

  @Override
  public String toString(){
    Collections.sort(_disciplinas);
    String s = "";
    if(_delegado == false){
      s += "ALUNO|"+this.getId()+"|"+this.getPhoneNumber()+"|"+this.getName();
      for(Discipline d : _disciplinas){
        s += "\n";
        s += "* "+this.getCourse().getName()+" - "+d.getName();
      }
    }
    else{
      s += "DELEGADO|"+this.getId()+"|"+this.getPhoneNumber()+"|"+this.getName();
      for(Discipline d : _disciplinas){
        s += "\n";
        s += "* "+this.getCourse().getName()+" - "+d.getName();
      }
    }
    return s;
  }

  public void deliverProject(String mensagem, Project projecto){
    projecto.createSubmission(mensagem, this);
    _projSubmetidos.add(projecto);
  }

  public ArrayList<Project> getDeliveredProjects(){
    return _projSubmetidos;
  }

  public void answerSurvey(int hours, String message, Project project){
    Survey inquerito = project.getSurvey();
    inquerito.addAnswer(hours, message);
    _inqueRespondidos.add(inquerito);
  }

  public ArrayList<Survey> getAnsweredSurveys(){
    return _inqueRespondidos;
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
