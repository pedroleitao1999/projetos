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
import sth.core.exception.NoSuchPersonIdException;

/**
 * School implementation.
 */
public class School implements java.io.Serializable {

  /** Serial number for serialization. */
  private static final long serialVersionUID = 201810051538L;

  private String _nome;
  private ArrayList<Person> _pessoas;
  private ArrayList<Course> _cursos;
  private Course _curso;
  private Person _pessoa;
  private Parser _parser;

  /**
   * This method creates a new school.
   * @param nome is the name of the school
   */
  public School(String nome){
    _nome = nome;
    _pessoas = new ArrayList<Person>();
    _cursos = new ArrayList<Course>();
    _curso = null;
    _pessoa = null;
    _parser = new Parser(this);
  }

  /**
    * This method returns the person with the given id.
    * @param id is the id of the Person
    * @return _pessoa is the person with the respective id
    */
  Person getPerson(int id){
    Iterator<Person> iterator = _pessoas.iterator();
    while(iterator.hasNext()){
      Person pessoa = iterator.next();
      if(pessoa.getId() == id){
        _pessoa = pessoa;
        break;
      }
    }
    return _pessoa;
  }

  /**
    * This method adds a new person to this school.
    * @param pessoa is the person to add
    */
  void addPerson(Person pessoa){
    _pessoas.add(pessoa);
  }

  /**
    * If the couse with that name already exists, then this method returns
    * that course. If the course with that name doesnt exist yet, we create a
    * new one based on his name.
    * @param cursoNome is the name of the course we want to create
    * @return _curso is the course
    */
  Course parseCourse(String cursoNome){
    for(Course c: _cursos){
      if(c.getName().equals(cursoNome)){
        return c;
      }
    }
    _curso = new Course(cursoNome);
    _cursos.add(_curso);
    return _curso;
  }

  /**
    * This method returns all the people in this school.
    * @return _pessoas is the array list with all the persons
    */
  ArrayList<Person> getPersonList(){
    return _pessoas;
  }

  /**
    * This method returns all the courses in this school.
    * @return _cursos is the array list with all the courses
    */
  public ArrayList<Course> getCourseList(){
    return _cursos;
  }

  /**
    * First, this method searches all the courses, and then the disciplines
    * from each one. In the end returns all the disciplines in this school.
    * @return disciplinas is the array list with all the disciplines
    */
  public ArrayList<Discipline> getAllDisciplines(){
    ArrayList<Discipline> disciplinas = new ArrayList<Discipline>();
    Iterator<Course> iteratorCourse = _cursos.iterator();
    while(iteratorCourse.hasNext()){
      Course curso = iteratorCourse.next();
      ArrayList<Discipline> disciplinasCurso = curso.getDisciplineList();
      Iterator<Discipline> iteratorDiscipline = disciplinasCurso.iterator();
      while(iteratorDiscipline.hasNext()){
        Discipline disciplina = iteratorDiscipline.next();
        disciplinas.add(disciplina);
      }
    }
    return disciplinas;
  }

  /**
    * From all the disciplines, this method searches all the projects from each
    * discipline. In the end returns all the projects.
    * @return projectos is the array list with all the projects
    */
  public ArrayList<Project> getAllProjects(){
    ArrayList<Project> projectos = new ArrayList<Project>();
    ArrayList<Discipline> disciplinas = getAllDisciplines();
    Iterator<Discipline> iteratorDiscipline = disciplinas.iterator();
    while(iteratorDiscipline.hasNext()){
      Discipline disciplina = iteratorDiscipline.next();
      ArrayList<Project> projectosDisciplina = disciplina.getProjectList();
      Iterator<Project> iteratorProject = projectosDisciplina.iterator();
      while(iteratorProject.hasNext()){
        Project projecto = iteratorProject.next();
        projectos.add(projecto);
      }
    }
    return projectos;
  }


  /**
   * @param filename
   * @throws BadEntryException
   * @throws IOException
   */
  void importFile(String filename) throws IOException, BadEntryException {
    _parser.parseFile(filename);
  }

}
