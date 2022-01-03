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

  Input<String> _disciplineName;
  Input<String> _projectName;
  Discipline _discipline;

  /**
   * @param receiver
   */
  public DoCreateProject(SchoolManager receiver) {
    super(Label.CREATE_PROJECT, receiver);
    _disciplineName = _form.addStringInput(Message.requestDisciplineName());
    _projectName = _form.addStringInput(Message.requestProjectName());
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void myExecute() throws DialogException, NoSuchDisciplineException, DuplicateProjectException {
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
      if(disciplina.getName().equals(_disciplineName.value())){
        disciplinaExiste = true;
        _discipline = disciplina;
        break;
      }
    }
    if(disciplinaExiste == false){
      throw new NoSuchDisciplineException(_disciplineName.value());
    }else{
      while(iteratorProject.hasNext()){
        Project projecto = iteratorProject.next();
        String nomeProjecto = projecto.getName();
        if(nomeProjecto.equals(_projectName.value())){
          projectoExiste = true;
          break;
        }
      }
      if(projectoExiste == true){
        throw new DuplicateProjectException(_disciplineName.value(), _projectName.value());
      }else{
        professor.openProject(_projectName.value(), _discipline);
      }
    }
  }

}
