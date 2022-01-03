package sth.app.representative;

import pt.tecnico.po.ui.DialogException;
import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.Display;
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
import java.util.Collections;
import java.util.ArrayList;
import java.util.Iterator;

/**
 * 4.6.6. Show discipline surveys.
 */
public class DoShowDisciplineSurveys extends Command<SchoolManager> {

  Input<String> _discipline;

  /**
   * @param receiver
   */
  public DoShowDisciplineSurveys(SchoolManager receiver) {
    super(Label.SHOW_DISCIPLINE_SURVEYS, receiver);
    _discipline = _form.addStringInput(Message.requestDisciplineName());
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() throws DialogException {

    _form.parse();

    Student delegado = (Student) _receiver.getLoggedUser();
    Course curso = delegado.getCourse();

    boolean disciplinaExiste = false;

    ArrayList<Discipline> disciplinas = curso.getDisciplineList();
    Iterator<Discipline> iteratorDiscipline = disciplinas.iterator();

    while(iteratorDiscipline.hasNext()){

      Discipline disciplina = iteratorDiscipline.next();

      if(_discipline.value().equals(disciplina.getName())){

        disciplinaExiste = true;
        ArrayList<Project> projectos = disciplina.getProjectList();
        Collections.sort(projectos);

        if(projectos.size() > 0){

          if(disciplina.getSurveyNumber() > 0){

            Iterator<Project> iteratorProject = projectos.iterator();

            while(iteratorProject.hasNext()){

              Project projecto = iteratorProject.next();
              Survey inquerito = projecto.getSurvey();

              if(inquerito != null){
                _display.addLine(inquerito.toString());
              }

            }

            _display.display();

          }

        }

      }

    }

  }

}
