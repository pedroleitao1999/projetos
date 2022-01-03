package sth.app.person;

import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.Display;
import sth.core.SchoolManager;
import sth.core.Person;

/**
 * 4.2.1. Show person.
 */
public class DoShowPerson extends Command<SchoolManager> {

  /**
   * @param receiver
   */
  public DoShowPerson(SchoolManager receiver) {
    super(Label.SHOW_PERSON, receiver);
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() {
    Person utilizador = _receiver.getLoggedUser();
    _display.addLine(utilizador.toString());
    _display.display();
  }

}
