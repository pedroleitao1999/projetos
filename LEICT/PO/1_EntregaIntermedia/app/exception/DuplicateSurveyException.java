package sth.app.exception;

/**
 *
 */
public class DuplicateSurveyException extends ProjectException {

  /** Serial number for serialization. */
  private static final long serialVersionUID = 201810051538L;
  
  /**
   * @param discipline 
   * @param project 
   */
  public DuplicateSurveyException(String discipline, String project) {
    super(discipline, project);
  }

  /** @see pt.tecnico.po.ui.DialogException#getMessage() */
  @Override
  public String getMessage() {
    return Message.duplicateSurvey(_discipline, _project);
  }

}
