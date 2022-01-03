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

public class Discipline implements Comparable<Discipline>, java.io.Serializable {

  /** Serial number for serialization. */
  private static final long serialVersionUID = 201810051538L;

  private String _name;
  private int _capacidade;
  private Course _curso;
  private ArrayList<Teacher> _professores;
  private ArrayList<Student> _alunos;
  private ArrayList<Project> _projectos;
  private ArrayList<Survey> _inqueritos;

  public Discipline(String name, Course curso){
    _name = name;
    _professores = new ArrayList<Teacher>();
    _alunos = new ArrayList<Student>();
    _projectos = new ArrayList<Project>();
    _inqueritos = new ArrayList<Survey>();
    _curso = curso;
  }

  @Override
  public int compareTo(Discipline outraDisciplina) {
    return _name.compareTo(outraDisciplina.getName());
  }

  void addTeacher(Teacher professor){
      _professores.add(professor);
      professor.addDiscipline(this);
  }

  void enrollStudent(Student aluno){
    if((aluno.getDisciplineList().size() < 6) && (aluno.getCourse().equals(_curso))){
      _alunos.add(aluno);
      aluno.getDisciplineList().add(this);
    }
  }

  public ArrayList<Student> getAllStudents(){
    return _alunos;
  }

  public ArrayList<Teacher> getAllTeachers(){
    return _professores;
  }

  public String getName(){
    return _name;
  }

  void setCourse(Course curso){
    _curso = curso;
  }

  Course getCourse(){
    return _curso;
  }

  void createProject(String name, String descricao){
    Project projecto = new Project(name, descricao, false, this);
    _projectos.add(projecto);
  }

  void createProject(String name){
    Project projecto = new Project(name, false, this);
    _projectos.add(projecto);
  }

  public ArrayList<Project> getProjectList(){
    return _projectos;
  }

  public int getSurveyNumber(){
    for(Project p : _projectos){
      if(p.getSurvey() != null){
        _inqueritos.add(p.getSurvey());
      }
    }
    return _inqueritos.size();
  }

}
