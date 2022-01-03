package sth.core;

import java.io.IOException;
import java.io.StreamTokenizer;
import java.io.FileReader;
import java.io.BufferedReader;
import java.io.Reader;

import java.util.Collection;
import java.util.Collections;
import java.util.ArrayList;
import java.util.Iterator;

public class Survey implements java.io.Serializable {

  /** Serial number for serialization. */
  private static final long serialVersionUID = 201810051538L;

  private int _estado;
  private ArrayList<Answer> _respostas;
  private Project _projecto;

  /*
  _estado = 0 significa inquerito criado
  _estado = 1 significa inquerito aberto
  _estado = 2 significa inquerito fechado
  _estado = 3 significa inquerito finalizado
  */

  public Survey(Project projecto){
    _estado = 0;
    _respostas = new ArrayList<Answer>();
    _projecto = projecto;
  }

  public int getState(){
    return _estado;
  }

  public ArrayList<Answer> getAnswers(){
    return _respostas;
  }

  public Project getProject(){
    return _projecto;
  }

  public void openSurvey(){
    _estado = 1;
    Notification notificacao = new Notification(this);
    notificacao.setState(1, this);
  }

  public void closeSurvey(){
    _estado = 2;
  }

  public void finishSurvey(){
    _estado = 3;
    Notification notificacao = new Notification(this);
    notificacao.setState(3, this);
  }

  public int getAnswersNumber(){
      return _respostas.size();
  }

  public int getAverageHours(){

    Iterator<Answer> iteratorAnswer = _respostas.iterator();
    int numRespostas = this.getAnswersNumber();
    if(numRespostas == 0){
      return 0;
    }
    int horasTotal = 0;

    while(iteratorAnswer.hasNext()){
      Answer resposta = iteratorAnswer.next();
      horasTotal += resposta.getHours();
    }

    return horasTotal/numRespostas;

  }

  public String toString(){

    String s = "";
    s += _projecto.getDisciplineName()+" - "+_projecto.getName();

    if(_estado == 0){
      s += " (por abrir)";
    }

    else if(_estado == 1){
      s += " (aberto)";
    }

    else if(_estado == 2){
      s += " (fechado)";
    }

    else if(_estado == 3){
      s += " - "+this.getAnswersNumber()+" respostas - "+this.getAverageHours()+" horas";
    }

    return s;

  }

  public int getLessHours(){

    Collections.sort(_respostas);
    Iterator<Answer> iteratorAnswer = _respostas.iterator();
    int menorTempo = 0;

    while(iteratorAnswer.hasNext()){
      Answer resposta = iteratorAnswer.next();
      menorTempo = resposta.getHours();
      break;
    }

    return menorTempo;

  }

  public int getMostHours(){

    Collections.sort(_respostas);
    Iterator<Answer> iteratorAnswer = _respostas.iterator();
    int maiorTempo = 0;

    while(iteratorAnswer.hasNext()){
      Answer resposta = iteratorAnswer.next();
      maiorTempo = resposta.getHours();
    }

    return maiorTempo;

  }

  public void addAnswer(int horas, String mensagem){
    Answer resposta = new Answer(horas, mensagem);
    _respostas.add(resposta);
  }

}
