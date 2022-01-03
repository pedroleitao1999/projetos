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
 * 4.5.2. Answer survey.
 */
public class DoAnswerSurvey extends sth.app.common.ProjectCommand {

  Input<Integer> _horas;
  Input<String> _mensagem;
  Discipline _disciplinaProj;
  Project _projecto;

  /**
   * @param receiver
   */
  public DoAnswerSurvey(SchoolManager receiver) {
    super(Label.ANSWER_SURVEY, receiver);
    _horas = _form.addIntegerInput(Message.requestProjectHours());
    _mensagem = _form.addStringInput(Message.requestComment());
  }

  /** @see sth.app.common.ProjectCommand#myExecute() */
  @Override
  public final void myExecute() throws NoSuchProjectException, NoSuchDisciplineException, DialogException {

    _disciplinaProj = null;
    _projecto = null;

    Student aluno = (Student) _receiver.getLoggedUser();

    boolean disciplinaExiste = false;
    boolean projectoExiste = false;
    boolean submeteu = false;
    boolean respondeuInquerito = false;
    boolean submeteuProjeto = false;

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
          _projecto = projecto;
          for(Submission submissao : projecto.getSubmissions()){
            if(submissao.getStudentId() == _receiver.getLoggedUser().getId()){
              submeteuProjeto = true;
            }
          }
          break;
        }

      }

      if(projectoExiste == false || submeteuProjeto == false){
        throw new NoSuchProjectException(_discipline.value(), _project.value());
      }

    }

    else{
      throw new NoSuchDisciplineException(_discipline.value());
    }

    ArrayList<Project> projectosSubmetidos = aluno.getDeliveredProjects();
    Iterator<Project> iteratorSubProj = projectosSubmetidos.iterator();

    while(iteratorSubProj.hasNext()){

      Project projSubmetido = iteratorSubProj.next();

      if(_project.value().equals(projSubmetido.getName())){

        Survey inquerito =  _projecto.getSurvey();

        if((inquerito != null) && (inquerito.getState() == 1)){

          ArrayList<Survey> inqueritosRespondidos = aluno.getAnsweredSurveys();
          Iterator<Survey> iteratorSurvey = inqueritosRespondidos.iterator();

          while(iteratorSurvey.hasNext()){

            Survey inqueritoRespondido = iteratorSurvey.next();

            if(inqueritoRespondido.equals(inquerito)){
              respondeuInquerito = true;
              break;
            }

          }

          if(respondeuInquerito == false){
            aluno.answerSurvey(_horas.value(), _mensagem.value(), _projecto);
          }

        }

        else{
          throw new NoSurveyException(_discipline.value(), _project.value());
        }

      }

    }

  }

}
