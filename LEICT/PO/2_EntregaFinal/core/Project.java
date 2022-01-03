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

import sth.core.exception.BadEntryException;

public class Project implements Comparable<Project>, java.io.Serializable {

  /** Serial number for serialization. */
  private static final long serialVersionUID = 201810051538L;

	private String _name;
	private String _description;
	private boolean _closed;
	private String _disciplineName;
  private Discipline _discipline;
	private ArrayList<Submission> _submissoes;
	private Survey _inquerito;

	public Project(String name, String description, boolean closed, Discipline discipline){
		_name = name;
		_description = description;
		_closed = closed;
		_disciplineName = discipline.getName();
    _discipline = discipline;
		_submissoes = new ArrayList<Submission>();
		_inquerito = null;
	}

	public Project(String name, boolean closed, Discipline discipline){
		_name = name;
		_closed = closed;
		_disciplineName = discipline.getName();
    _discipline = discipline;
		_submissoes = new ArrayList<Submission>();
		_inquerito = null;
	}

	@Override
  public int compareTo(Project outroProjecto) {
    return _name.compareTo(outroProjecto.getName());
  }

	public String getName(){
		return _name;
	}

	public boolean getClosed(){
		return _closed;
	}

	public String getDisciplineName(){
		return _disciplineName;
	}

  public Discipline getDiscipline(){
    return _discipline;
  }

	public void close(){
		_closed = true;
		if(_inquerito != null){
			if(_inquerito.getState() == 0){
				_inquerito.openSurvey();
			}
		}
	}

	public void createSubmission(String mensagem, Student aluno){

		int alunoId = aluno.getId();
		Submission submissao = new Submission(mensagem, this, alunoId);
    boolean alunoSubmeteu = false;

    if(_submissoes.size() > 0){
      for(Submission s : _submissoes){
        if(s.getStudentId() == alunoId){
          s.changeMessage(mensagem);
          alunoSubmeteu = true;
        }
      }
    }

    if(alunoSubmeteu == false){
		    _submissoes.add(submissao);
    }

	}

	public ArrayList<Submission> getSubmissions(){
		return _submissoes;
	}

	public int getSubmissionsNumber(){
		return _submissoes.size();
	}

	public String toString(){

		String s = "";
		s += this.getDisciplineName()+" - "+this.getName();

		if(_submissoes.size() > 0){
			Collections.sort(_submissoes);
			for(Submission sub : _submissoes){
				s += "\n";
				s += "* "+sub.getStudentId()+" - "+sub.getMessage();
			}
		}

		return s;

	}

	public Survey getSurvey(){
		return _inquerito;
	}

  public void createSurvey(){
    _inquerito = new Survey(this);
  }

  public void cancelSurvey(){
    _inquerito = null;
  }

}
