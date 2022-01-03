package sth.app.common;

import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.DialogException;
import pt.tecnico.po.ui.Input;
import sth.core.SchoolManager;

import sth.app.exception.NoSuchProjectException;
import sth.app.exception.NoSuchDisciplineException;
import sth.core.exception.NoSuchProjectIdException;
import sth.core.exception.NoSuchDisciplineIdException;

/**
 * Represents the super class Command of all commands that concern a project.
 * It adds two Input's to the form (one for asking the id of discipline and the other for
 * asking the id of the discipline). If needed, other Input's could be added in sub-classes.
 * It takes care of the exceptions NoSuchProjectIdException and NoSuchDisciplineIdException that
 * can happen during the execution of the execute method.
 * The subclasses of this class should implement their specific behavior in the method myExecute.
 */
public abstract class ProjectCommand extends Command<SchoolManager> {

  //FIXME add input fields if needed
  protected final Input<String> _discipline;
  protected final Input<String> _project;

  /**
   * @param receiver
   */
  public ProjectCommand(String label, SchoolManager receiver) {
    super(label, receiver);

    _discipline = _form.addStringInput(Message.requestDisciplineName());
    _project = _form.addStringInput(Message.requestProjectName());
  }

  /**
   * This method represents the specific behavior of each command class
   * that concerns a project.
   **/
  protected abstract void myExecute() throws NoSuchProjectIdException, NoSuchDisciplineIdException, DialogException;

  /**
   * Executes the this command. Parses the form associated with this command and then executes the
   * specific behavior of this command (should be implemented in myExecute method).
   * It handles the chProjectIdException and NoSuchDisciplineIdException.
   *
   * @see pt.tecnico.po.ui.Command#execute()
   **/
  @Override
  public final void execute() throws DialogException {
    _form.parse();

    try {
      myExecute();
    } catch (NoSuchProjectIdException nsp) {
      throw new NoSuchProjectException(_discipline.value(), _project.value());
    } catch (NoSuchDisciplineIdException nsd) {
      throw new NoSuchDisciplineException(_discipline.value());
    }
  }

}
