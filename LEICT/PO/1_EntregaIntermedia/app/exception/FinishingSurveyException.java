package sth.app.exception;

/**
 *
 */
public class FinishingSurveyException extends ProjectException {

  /** Serial number for serialization. */
  private static final long serialVersionUID = 201810051538L;
  
  /**
   * @param discipline 
   * @param project 
   */
  public FinishingSurveyException(String discipline, String project) {
    super(discipline, project);
  }

  /** @see pt.tecnico.po.ui.DialogException#getMessage() */
  @Override
  public String getMessage() {
    return Message.finishingSurvey(_discipline, _project);
  }

}
