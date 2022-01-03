package sth.app.person;

import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.Input;
import sth.core.SchoolManager;
import sth.core.Person;

import java.util.Collection;
import java.util.Collections;
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

    _form.parse();

    ArrayList<Person> pessoas = _receiver.getAllUsers();
    Iterator<Person> iteratorPerson = pessoas.iterator();

    ArrayList<String> nomesEncontrados = new ArrayList<String>();

    while(iteratorPerson.hasNext()){

      Person pessoa = iteratorPerson.next();
      String nomePessoa = pessoa.getName().toLowerCase();
      String pesquisa = _name.value().toLowerCase();

      if(nomePessoa.contains(pesquisa) == true){
        nomesEncontrados.add(pessoa.getName());
      }

    }

    Collections.sort(nomesEncontrados);
    Iterator<String> iteratorName = nomesEncontrados.iterator();

    while(iteratorName.hasNext()){

      ArrayList<Person> pessoasNovo = _receiver.getAllUsers();
      Iterator<Person> iteratorPersonNovo = pessoasNovo.iterator();
      String nome = iteratorName.next();

      while(iteratorPersonNovo.hasNext()){

        Person pessoa = iteratorPersonNovo.next();

        if(nome.equals(pessoa.getName())){
          _display.addLine(pessoa.toString());
        }
      }

    }

    _display.display();

  }

}
