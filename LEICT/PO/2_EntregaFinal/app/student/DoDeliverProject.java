package sth.app.student;

import pt.tecnico.po.ui.DialogException;
import pt.tecnico.po.ui.Input;
import sth.core.SchoolManager;
import sth.core.Student;
import sth.core.Discipline;
import sth.core.Project;
import sth.core.Submission;
import sth.core.School;

import sth.app.exception.NoSuchProjectException;
import sth.app.exception.NoSuchDisciplineException;

import java.util.Collection;
import java.util.ArrayList;
import java.util.Iterator;

/**
 * 4.5.1. Deliver project.
 */
public class DoDeliverProject extends sth.app.common.ProjectCommand {

  Input<String> _message;
  Discipline _disciplinaProj;

  /**
   * @param receiver
   */
  public DoDeliverProject(SchoolManager receiver) {
    super(Label.DELIVER_PROJECT, receiver);
    _message = _form.addStringInput(Message.requestDeliveryMessage());
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void myExecute() throws NoSuchProjectException, NoSuchDisciplineException, DialogException {

    _disciplinaProj = null;

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

        if((_project.value().equals(projecto.getName())) && (projecto.getClosed() == false)){
          projectoExiste = true;
          aluno.deliverProject(_message.value(), projecto);
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
