package sth.app.teaching;

import pt.tecnico.po.ui.DialogException;
import pt.tecnico.po.ui.Input;
import sth.core.SchoolManager;
import sth.core.Teacher;
import sth.core.Discipline;
import sth.core.Project;
import sth.core.School;

import sth.app.exception.NoSuchProjectException;
import sth.app.exception.NoSuchDisciplineException;

import java.util.Collection;
import java.util.ArrayList;
import java.util.Iterator;

/**
 * 4.4.2. Close project.
 */
public class DoCloseProject extends sth.app.common.ProjectCommand {

  Input<String> _disciplineName;
  Input<String> _projectName;

  /**
   * @param receiver
   */
  public DoCloseProject(SchoolManager receiver) {
    super(Label.CLOSE_PROJECT, receiver);
    _disciplineName = _form.addStringInput(Message.requestDisciplineName());
    _projectName = _form.addStringInput(Message.requestProjectName());
  }

  /** @see sth.app.common.ProjectCommand#myExecute() */
  @Override
  public final void myExecute() throws DialogException, NoSuchDisciplineException, NoSuchProjectException {
    Teacher professor = (Teacher) _receiver.getLoggedUser();
    boolean disciplinaExiste = false;
    boolean projectoExiste = false;
    School universidade = _receiver.getSchool();
    ArrayList<Discipline> disciplinas = universidade.getAllDisciplines();
    ArrayList<Project> projectos = universidade.getAllProjects();
    Iterator<Discipline> iteratorDiscipline = disciplinas.iterator();
    Iterator<Project> iteratorProject = projectos.iterator();
    while(iteratorDiscipline.hasNext()){
      Discipline disciplina = iteratorDiscipline.next();
      if(disciplina.getName() == _disciplineName.value()){
        disciplinaExiste = true;
        break;
      }
    }
    if(disciplinaExiste == false){
      throw new NoSuchDisciplineException(_disciplineName.value());
    }else{
      while(iteratorProject.hasNext()){
        Project projecto = iteratorProject.next();
        if(projecto.getName() == _projectName.value()){
          projectoExiste = true;
          professor.closeProject(projecto);
          break;
        }
      }
      if(projectoExiste == false){
        throw new NoSuchProjectException(_disciplineName.value(), _projectName.value());
      }
    }

  }

}
