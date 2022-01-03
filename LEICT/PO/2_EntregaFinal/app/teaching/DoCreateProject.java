package sth.app.teaching;

import pt.tecnico.po.ui.DialogException;
import pt.tecnico.po.ui.Input;
import sth.core.SchoolManager;
import sth.core.Teacher;
import sth.core.Discipline;
import sth.core.Project;
import sth.core.School;

import sth.app.exception.NoSuchDisciplineException;
import sth.app.exception.DuplicateProjectException;

import java.util.Collection;
import java.util.ArrayList;
import java.util.Iterator;

/**
 * 4.4.1. Create project.
 */
public class DoCreateProject extends sth.app.common.ProjectCommand {

  Discipline _disciplinaProj;

  /**
   * @param receiver
   */
  public DoCreateProject(SchoolManager receiver) {
    super(Label.CREATE_PROJECT, receiver);
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void myExecute() throws DialogException, NoSuchDisciplineException, DuplicateProjectException {

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
          break;
        }

      }

      if(projectoExiste == false){
        professor.openProject(_project.value(), _disciplinaProj);
      }

      else{
        throw new DuplicateProjectException(_discipline.value(), _project.value());
      }
    }

    else{
      throw new NoSuchDisciplineException(_discipline.value());
    }

  }

}
