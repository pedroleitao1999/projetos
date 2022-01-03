package sth.app.teaching;

import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.DialogException;
import pt.tecnico.po.ui.Input;
import sth.core.SchoolManager;
import sth.core.Teacher;
import sth.core.Discipline;
import sth.core.Course;
import sth.core.Student;
import sth.app.exception.NoSuchDisciplineException;

import java.util.Collection;
import java.util.Collections;
import java.util.ArrayList;
import java.util.Iterator;
import java.io.IOException;

//FIXME import other classes if needed

/**
 * 4.4.4. Show course students.
 */
public class DoShowDisciplineStudents extends Command<SchoolManager> {

  Input<String> _name;
  Discipline _discipline;

  /**
   * @param receiver
   */
  public DoShowDisciplineStudents(SchoolManager receiver) {
    super(Label.SHOW_COURSE_STUDENTS, receiver);
    _name = _form.addStringInput(Message.requestDisciplineName());
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() throws DialogException {

    _form.parse();
    _discipline = null;

    Teacher professor = (Teacher) _receiver.getLoggedUser();

    ArrayList<Discipline> disciplinas = professor.getDisciplineListProf();
    Iterator<Discipline> iteratorDiscipline = disciplinas.iterator();

    boolean disciplinaExiste = false;

    while(iteratorDiscipline.hasNext()){

      Discipline disciplina = iteratorDiscipline.next();

      if(disciplina.getName().equals(_name.value())){
        _discipline = disciplina;
        disciplinaExiste = true;
        break;
      }

    }

    if(disciplinaExiste == false){
      throw new NoSuchDisciplineException(_name.value());
    }

    else{

      ArrayList<Student> alunos = professor.getStudentsOfDiscipline(_discipline);
      Collections.sort(alunos);
      Iterator<Student> iteratorStudent = alunos.iterator();


      while(iteratorStudent.hasNext()){
        Student aluno = iteratorStudent.next();
        _display.addLine(aluno.toString());
      }

    }

    _display.display();

  }

}
