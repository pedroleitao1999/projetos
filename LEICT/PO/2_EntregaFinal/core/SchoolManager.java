package sth.core;

import java.io.IOException;
import sth.core.exception.NoSuchPersonIdException;
import java.io.StreamTokenizer;
import java.io.FileReader;
import java.io.BufferedReader;
import java.io.Reader;
import java.io.*;

import java.util.Collection;
import java.util.ArrayList;
import java.util.Iterator;

import sth.core.exception.BadEntryException;
import sth.core.exception.ImportFileException;
import sth.core.exception.NoSuchPersonIdException;

import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.Display;
import pt.tecnico.po.ui.Input;


/**
 * The facade class.
 */
public class SchoolManager {

  private School _school;
  private Person _utilizadorRegistado;
  private ArrayList<Person> _pessoasInscritas;
  private String _file;

  public SchoolManager(){
    _school = new School("Universidade");
    _pessoasInscritas = new ArrayList<Person>();
    _pessoasInscritas = _school.getPersonList();
    _file = null;
  }

  /**
   * @param datafile
   * @throws ImportFileException
   * @throws InvalidCourseSelectionException
   */
  public void importFile(String datafile) throws ImportFileException {
    try {
      _school.importFile(datafile);
    } catch (IOException | BadEntryException e) {
      throw new ImportFileException(e);
    }
  }

  /**
   * Do the login of the user with the given identifier.

   * @param id identifier of the user to login
   * @throws NoSuchPersonIdException if there is no uers with the given identifier
   */
  public void login(int id) throws NoSuchPersonIdException {
    Iterator<Person> iterator = _pessoasInscritas.iterator();
    Person pessoaInscrita;
    boolean utilizadorExiste = false;
    while(iterator.hasNext()){
      pessoaInscrita = iterator.next();
      if(pessoaInscrita.getId() == id){
        _utilizadorRegistado = pessoaInscrita;
        utilizadorExiste = true;
      }
    }
    if(utilizadorExiste == false){
      throw new NoSuchPersonIdException(id);
    }
  }

  /**
   * @return true when the currently logged in person is an administrative
   */
  public boolean isLoggedUserAdministrative() {
    if(_utilizadorRegistado instanceof Employee){
      return true;
    }else{
      return false;
    }
  }

  /**
   * @return true when the currently logged in person is a professor
   */
  public boolean isLoggedUserProfessor() {
    if(_utilizadorRegistado instanceof Teacher){
      return true;
    }else{
      return false;
    }
  }

  /**
   * @return true when the currently logged in person is a student
   */
  public boolean isLoggedUserStudent() {
    if(_utilizadorRegistado instanceof Student){
      return true;
    }else{
      return false;
    }
  }

  /**
   * @return true when the currently logged in person is a representative
   */
  public boolean isLoggedUserRepresentative() {
    if(_utilizadorRegistado instanceof Student){
      Student aluno = (Student) _utilizadorRegistado;
      if(aluno.isRepresentative() == true){
        return true;
      }else{
        return false;
      }
    }else{
      return false;
    }
  }

  public Person getLoggedUser(){
    return _utilizadorRegistado;
  }

  public ArrayList<Person> getAllUsers(){
    return _pessoasInscritas;
  }

  public School getSchool(){
    return _school;
  }

  public String getFile(){
    return _file;
  }

  public void saveSchool(String file){
    ObjectOutputStream obOut = null;
    try{
      _file = file;
      FileOutputStream fpout = new FileOutputStream(file);
      obOut = new ObjectOutputStream(fpout);
      obOut.writeObject(_school);
    }catch(IOException io){
      System.err.println("Error: " + io);
    }finally{
      try{
        if(obOut != null){
          obOut.close();
        }
      }catch(IOException io){
        System.err.println("Error: " + io);
      }
    }
  }

  public void openSchool(String file) throws NoSuchPersonIdException{
    _file = file;
    ObjectInputStream obIn = null;
    try{
      FileInputStream fpin = new FileInputStream(file);
      obIn = new ObjectInputStream(fpin);
      School school = (School) obIn.readObject();
      Person pessoa = school.getPerson(_utilizadorRegistado.getId());
      if(!school.getPersonList().contains(pessoa)){
        throw new NoSuchPersonIdException(this.getLoggedUser().getId());
      }
      _school = school;
      _utilizadorRegistado = pessoa;
    }catch(IOException io){
      System.err.println("Error: " + io);
    }catch(ClassNotFoundException cnf){
        System.err.println("Error: " + cnf);
    }finally{
      try{
        if(obIn != null){
          obIn.close();
        }
      }catch(IOException io){
        System.err.println("Error: " + io);
      }
    }
  }
}
