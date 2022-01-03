package sth.app.teaching;

import pt.tecnico.po.ui.DialogException;
import pt.tecnico.po.ui.Input;
import sth.core.SchoolManager;
import sth.core.Teacher;
import sth.core.Discipline;
import sth.core.Project;
import sth.core.Submission;
import sth.core.School;

import sth.app.exception.NoSuchProjectException;
import sth.app.exception.NoSuchDisciplineException;

import java.util.Collection;
import java.util.Collections;
import java.util.ArrayList;
import java.util.Iterator;

/**
 * 4.4.3. Show project submissions.
 */
public class DoShowProjectSubmissions extends sth.app.common.ProjectCommand {
  /**
   * @param receiver
   */

  Discipline _disciplinaProj;

  public DoShowProjectSubmissions(SchoolManager receiver) {
    super(Label.SHOW_PROJECT_SUBMISSIONS, receiver);
  }

  /** @see sth.app.common.ProjectCommand#myExecute() */
  @Override
  public final void myExecute() throws DialogException, NoSuchDisciplineException, NoSuchProjectException {

    _disciplinaProj = null;

    Teacher professor = (Teacher) _receiver.getLoggedUser();

    boolean disciplinaExiste = false;
    boolean projectoExiste = false;

    ArrayList<Discipline> disciplinas = professor.getDisciplineListProf();
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
          _display.addLine(projecto.toString());
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

    _display.display();

  }

}
