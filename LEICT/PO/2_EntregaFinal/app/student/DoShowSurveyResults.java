package sth.app.student;

import pt.tecnico.po.ui.DialogException;
import pt.tecnico.po.ui.Input;
import sth.core.SchoolManager;
import sth.core.Student;
import sth.core.Discipline;
import sth.core.Project;
import sth.core.Submission;
import sth.core.Survey;
import sth.core.School;

import sth.app.exception.NoSuchProjectException;
import sth.app.exception.NoSuchDisciplineException;
import sth.app.exception.NoSurveyException;

import java.util.Collection;
import java.util.Collections;
import java.util.ArrayList;
import java.util.Iterator;


/**
 * 4.5.3. Show survey results.
 */
public class DoShowSurveyResults extends sth.app.common.ProjectCommand {

  Discipline _disciplinaProj;
  Project _projecto;

  /**
   * @param receiver
   */
  public DoShowSurveyResults(SchoolManager receiver) {
    super(Label.SHOW_SURVEY_RESULTS, receiver);
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void myExecute() throws NoSuchProjectException, NoSuchDisciplineException, NoSurveyException {

    _disciplinaProj = null;
    _projecto = null;

    Student aluno = (Student) _receiver.getLoggedUser();

    boolean disciplinaExiste = false;
    boolean projectoExiste = false;

    ArrayList<Discipline> disciplinas = aluno.getDisciplineList();
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
          Survey inquerito = projecto.getSurvey();

          if(inquerito != null){

            if(inquerito.getState() != 3){
              _display.addLine(inquerito.toString());
            }

            else{
              String s = "";
              s += _discipline.value()+" - "+_project.value()+"\n";
              s += " * Número de respostas: "+inquerito.getAnswersNumber()+"\n";
              s += " * Tempo médio (horas): "+inquerito.getAverageHours();
              _display.addLine(s);
            }

            _display.display();

          }

          else{
            throw new NoSurveyException(_discipline.value(), _project.value());
          }

          break;

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
