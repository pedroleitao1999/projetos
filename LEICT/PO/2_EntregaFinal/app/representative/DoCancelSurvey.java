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
import sth.app.exception.NoSurveyException;
import sth.app.exception.NonEmptySurveyException;
import sth.app.exception.SurveyFinishedException;

import java.util.Collection;
import java.util.ArrayList;
import java.util.Iterator;

/**
 * 4.5.2. Cancel survey.
 */
public class DoCancelSurvey extends sth.app.common.ProjectCommand {

  Discipline _disciplinaProj;

  /**
   * @param receiver
   */
  public DoCancelSurvey(SchoolManager receiver) {
    super(Label.CANCEL_SURVEY, receiver);
  }

  /** @see sth.app.common.ProjectCommand#myExecute() */
  @Override
  public final void myExecute() throws NoSuchProjectException, NoSuchDisciplineException, DialogException {

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

          if(projecto.getSurvey() == null){
            throw new NoSurveyException(_discipline.value(), _project.value());
          }

          else{

            Survey inquerito = projecto.getSurvey();

            if((inquerito.getState() == 0) || (inquerito.getState() == 1)){

              if(inquerito.getAnswers().size() == 0){
                projecto.cancelSurvey();
              }

              else{
                throw new NonEmptySurveyException(_discipline.value(), _project.value());
              }

            }

            else if(inquerito.getState() == 2){
              inquerito.openSurvey();
            }

            else{
              throw new SurveyFinishedException(_discipline.value(), _project.value());
            }

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