package sth.app.main;

import java.io.IOException;
import sth.app.exception.NoSuchPersonException;

import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.Input;
import pt.tecnico.po.ui.Display;
import sth.core.SchoolManager;
import sth.core.School;
import sth.core.Course;

import java.io.*;

import sth.core.Teacher;
import sth.core.Discipline;
import sth.core.Project;
import sth.core.Submission;
import sth.core.School;

import java.util.ArrayList;

/**
 * 4.1.1. Save to file under current name (if unnamed, query for name).
 */
public class DoSave extends Command<SchoolManager>  {
  private Input<String> _file;

  /**
   * @param receiver
   */
  public DoSave(SchoolManager receiver) {
    super(Label.SAVE, receiver);
    _file = _form.addStringInput(Message.newSaveAs());
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() throws NoSuchPersonException {
    if(_receiver.getFile() == null){
       _form.parse();
       _receiver.saveSchool(_file.value());
    }
    else{
      _receiver.saveSchool(_receiver.getFile());
    }
  }
}
