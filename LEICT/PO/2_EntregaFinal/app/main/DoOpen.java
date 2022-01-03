package sth.app.main;

import java.io.FileNotFoundException;
import java.io.IOException;
import sth.core.exception.ImportFileException;
import sth.core.exception.NoSuchPersonIdException;
import sth.app.exception.NoSuchPersonException;

import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.DialogException;
import pt.tecnico.po.ui.Input;
import sth.core.SchoolManager;
import sth.core.School;
import sth.core.Person;
import sth.core.Teacher;
import sth.core.Student;

import java.io.*;

/**
 * 4.1.1. Open existing document.
 */
public class DoOpen extends Command<SchoolManager>  {

  Input<String> _file;

  /**
   * @param receiver
   */
  public DoOpen(SchoolManager receiver) {
    super(Label.OPEN, receiver);
    _file = _form.addStringInput(Message.openFile());
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() throws DialogException {
    _form.parse();
    try{
      _receiver.openSchool(_file.value());
    }catch(NoSuchPersonIdException nsp){
      throw new NoSuchPersonException(_receiver.getLoggedUser().getId());
    }

    Person utilizador = _receiver.getLoggedUser();

    if(utilizador instanceof Student){

      Student aluno = (Student) utilizador;


      for(String s : aluno.getNotificationList()){
        _display.addLine(s);
      }

      aluno.removeNotifications();

    }

    if(utilizador instanceof Teacher){

      Teacher professor = (Teacher) utilizador;


      for(String s : professor.getNotificationList()){
        _display.addLine(s);
      }

      professor.removeNotifications();

    }
    
    _display.display();

  }

}
