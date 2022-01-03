package sth.core.exception;

/** Exception thrown when the requested project does not exist. */
public class NoSuchDisciplineIdException extends Exception {

  /** Serial number for serialization. */
  private static final long serialVersionUID = 201809021324L;

  /** Discipline id. */
  private String _disciplineName;

  /**
   * @param id
   */
  public NoSuchDisciplineIdException(String id) {
    _disciplineName = id;
  }

  /** @return id */
  public String getId() {
    return _disciplineName;
  }

}
