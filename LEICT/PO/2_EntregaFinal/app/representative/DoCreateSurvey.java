package sth.app.representative;

import pt.tecnico.po.ui.DialogException;
import pt.tecnico.po.ui.Input;
import sth.core.SchoolManager;
import sth.core.Student;
import sth.core.Course;
import sth.core.Discipline;
import sth.core.Project;
import sth.core.Survey;

import sth.app.exception.NoSuchDisciplineException;
import sth.app.exception.NoSuchProjectException;
import sth.app.exception.DuplicateSurveyException;

import java.util.Collection;
import java.util.ArrayList;
import java.util.Iterator;

/**
 * 4.5.1. Create survey.
 */
public class DoCreateSurvey extends sth.app.common.ProjectCommand {

  Discipline _disciplinaProj;

  /**
   * @param receiver
   */
  public DoCreateSurvey(SchoolManager receiver) {
    super(Label.CREATE_SURVEY, receiver);
  }

  /** @see sth.app.common.ProjectCommand#myExecute() */
  @Override
  public final void myExecute() throws DialogException, NoSuchDisciplineException, NoSuchProjectException {

    _disciplinaProj = null;

    Student delegado = (Student) _receiver.getLoggedUser();
    Course curso = delegado.getCourse();

    boolean disciplinaExiste = false;
    boolean projectoExiste = false;

    ArrayList<Discipline> disciplinas = curso.getDisciplineList();
    Iterator<Discipline> iteratorDiscipline = disciplinas.iterator();

    while(iteratorDiscipline.hasNext()){

      Discipline disciplina = iteratorDiscipline.next();

      if(_discipline.value().equals(disciplina.getName())){
        disciplinaExiste = true;
        _disciplinaProj = disciplina;
        break;
      }

    }

    if(disciplinaExiste == true){

      ArrayList<Project> projectos = _disciplinaProj.getProjectList();
      Iterator<Project> iteratorProject = projectos.iterator();

      while(iteratorProject.hasNext()){

        Project projecto = iteratorProject.next();

        if(_project.value().equals(projecto.getName())){
          projectoExiste = true;
          if((projecto.getSurvey() == null) && projecto.getClosed() == false){
            projecto.createSurvey();           
          }
          else if(projecto.getClosed() == true){
            throw new NoSuchProjectException(_discipline.value(), _project.value());
          }
          else{
            throw new DuplicateSurveyException(_discipline.value(), _project.value());
          }

        }

      }

      if(projectoExiste == false){
        throw new NoSuchProjectException(_discipline.value(), _project.value());
      }

    }

    else{
      throw new NoSuchDisciplineException(_discipline.value());
    }

  }

}
