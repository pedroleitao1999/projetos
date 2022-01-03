package sth.core;

import java.io.IOException;
import java.io.StreamTokenizer;
import java.io.FileReader;
import java.io.BufferedReader;
import java.io.Reader;

import java.util.Collection;
import java.util.ArrayList;

public class Notification implements java.io.Serializable {

  /** Serial number for serialization. */
  private static final long serialVersionUID = 201810051538L;

  private ArrayList<Person> _pessoas;
  private int _estado;
  private Survey _inquerito;


  public Notification(Survey inquerito){
    _pessoas = new ArrayList<Person>();
    _inquerito = inquerito;
  }

  public int getState(){
    return _estado;
  }

  public void setState(int estado, Survey inquerito) {
    _estado = estado;
    notifyAllObservers(inquerito);
  }

  public void notifyAllObservers(Survey inquerito){

    Discipline disciplina = inquerito.getProject().getDiscipline();
    Course curso = disciplina.getCourse();

    ArrayList<Student> alunosDisciplina = disciplina.getAllStudents();
    ArrayList<Student> delegados = curso.getRepresentativeList();
    ArrayList<Teacher> professores = disciplina.getAllTeachers();
    ArrayList<Student> alunos = new ArrayList<Student>();

    for(Student s : alunosDisciplina){
      alunos.add(s);
    }

    for(Student d : delegados){
      ArrayList<Discipline> disciplinasDelegado = d.getDisciplineList();
      if(!disciplinasDelegado.contains(disciplina)){
        alunos.add(d);
      }
    }

    String nomeProjecto = inquerito.getProject().getName();
    String nomeDisciplina = inquerito.getProject().getDiscipline().getName();

    String str = "";
    if(inquerito.getState() == 1){
      str += "Pode preencher inquérito do projecto "+nomeProjecto+" da disciplina "+nomeDisciplina;
    }

    else if(inquerito.getState() == 3){
      str += "Resultados do inquérito do projecto "+nomeProjecto+" da disciplina "+nomeDisciplina;
    }

    for(Student s : alunos) {
      s.addNotification(str);
    }

    for(Teacher t : professores) {
      t.addNotification(str);
    }

  }

}
