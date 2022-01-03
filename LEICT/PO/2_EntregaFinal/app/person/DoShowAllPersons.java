package sth.app.person;

import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.Display;
import sth.core.SchoolManager;
import sth.core.Person;

import java.util.Collection;
import java.util.Collections;
import java.util.ArrayList;
import java.util.Iterator;

/**
 * 4.2.3. Show all persons.
 */
public class DoShowAllPersons extends Command<SchoolManager> {

  /**
   * @param receiver
   */
  public DoShowAllPersons(SchoolManager receiver) {
    super(Label.SHOW_ALL_PERSONS, receiver);
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() {
    ArrayList<Person> pessoas = _receiver.getAllUsers();
    Collections.sort(pessoas);
    Iterator<Person> iterator = pessoas.iterator();
    Person pessoaEncontrada;
    while(iterator.hasNext()){
      pessoaEncontrada = iterator.next();
      _display.addLine(pessoaEncontrada.toString());
    }
    _display.display();
  }

}
