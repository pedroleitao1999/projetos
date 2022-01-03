package sth.app;

import static pt.tecnico.po.ui.Dialog.IO;

import pt.tecnico.po.ui.DialogException;
import pt.tecnico.po.ui.Menu;
import sth.core.SchoolManager;
import sth.app.main.MainMenu;
import sth.app.person.DoLogin;
import sth.core.exception.BadEntryException;
import sth.core.exception.ImportFileException;


import java.io.IOException;

/**
 * Main driver for the travel management application.
 */
public class App {

  public static void main(String[] args) {
    SchoolManager school = new SchoolManager();

    String datafile = System.getProperty("import"); //$NON-NLS-1$
    if (datafile != null) {
      try {
        school.importFile(datafile);
      } catch (ImportFileException bde) {
        // file input should always be correct: just present the problem
        // no behavior described: just present the problem
        System.err.println("Error in parsing: " + bde.getMessage());
        bde.printStackTrace();
      }
    }

    try {
      DoLogin loginCmd = new DoLogin(school);
      loginCmd.execute();
      Menu menu = new MainMenu(school);
      menu.open();
    } catch (DialogException de) {
      // DO NOTHING -- just exit
      de.printStackTrace();
    } finally {
      IO.close();
    }
  }

}
