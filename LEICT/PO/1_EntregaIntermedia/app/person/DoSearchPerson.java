package sth.app.person;

import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.Input;
import sth.core.SchoolManager;
import sth.core.Person;

import java.util.Collection;
import java.util.ArrayList;
import java.util.Iterator;

/**
 * 4.2.4. Search person.
 */
public class DoSearchPerson extends Command<SchoolManager> {

  Input<String> _name;

  /**
   * @param receiver
   */
  public DoSearchPerson(SchoolManager receiver) {
    super(Label.SEARCH_PERSON, receiver);
    _name = _form.addStringInput(Message.requestPersonName());
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() {
    ArrayList<Person> pessoas = _receiver.getAllUsers();
    Iterator<Person> iterator = pessoas.iterator();
    Person pessoaEncontrada = null;
    while(iterator.hasNext()){
      pessoaEncontrada = iterator.next();
      String nomePessoa = pessoaEncontrada.getName().toLowerCase();
      String searchName = _name.value().toLowerCase();
      if(nomePessoa.contains(searchName) == true){
        _display.addLine(pessoaEncontrada.toString());
        _display.display();
      }
    }
  }

}
