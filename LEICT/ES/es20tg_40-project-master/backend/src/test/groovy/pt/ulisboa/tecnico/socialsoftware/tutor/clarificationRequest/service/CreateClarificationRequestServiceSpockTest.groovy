package pt.ulisboa.tecnico.socialsoftware.tutor.clarificationRequest.service

import org.springframework.beans.factory.annotation.Autowired
import org.springframework.boot.test.autoconfigure.orm.jpa.DataJpaTest
import pt.ulisboa.tecnico.socialsoftware.tutor.exceptions.TutorException
import pt.ulisboa.tecnico.socialsoftware.tutor.clarificationRequest.ClarificationRequestService
import pt.ulisboa.tecnico.socialsoftware.tutor.clarificationRequest.repository.ClarificationRequestRepository
import pt.ulisboa.tecnico.socialsoftware.tutor.question.repository.QuestionRepository
import pt.ulisboa.tecnico.socialsoftware.tutor.user.UserRepository
import pt.ulisboa.tecnico.socialsoftware.tutor.user.User
import pt.ulisboa.tecnico.socialsoftware.tutor.question.domain.Question
import spock.lang.Specification

import static pt.ulisboa.tecnico.socialsoftware.tutor.exceptions.ErrorMessage.CLARIFICATION_IS_EMPTY
import static pt.ulisboa.tecnico.socialsoftware.tutor.exceptions.ErrorMessage.CLARIFICATION_SIZE_LIMIT_EXCEEDED

@DataJpaTest
class CreateClarificationRequestServiceSpockTest extends Specification {
    public static final String DESCRIPTION = "Bla"
    public static final String DESCRIPTION_TOO_LARGE = "BlaBlaBlaBlaBlaBlaBlaBlaBlaBlaBlaBlaBlaBlaBlaBlaBlaBlaBlaBlaBlaBlaBlaBlaBlaBlaBlaBlaBlaBlaBlaBlaBlaBlaBlaBla"

    @Autowired
    ClarificationRequestService clarificationRequestService

    @Autowired
    ClarificationRequestRepository clarificationRequestRepository

    @Autowired
    QuestionRepository questionRepository

    @Autowired
    UserRepository studentRepository

    def setup() {
        def student = new User()
        student.setKey(1)
        studentRepository.save(student)
        def question = new Question()
        question.setKey(1)
        questionRepository.save(question)
    }

    def "create a clarification request"() {
        // the clarification request is created
	    given: "a student and a question"
        def studentId = studentRepository.findAll().get(0).getId()
        def questionId = questionRepository.findAll().get(0).getId()

        when:
        clarificationRequestService.createClarificationRequest(studentId, questionId, DESCRIPTION)

        then: "clarification request is created"
        clarificationRequestRepository.findAll().size() == 1
        def clarificationRequest = clarificationRequestRepository.findAll().get(0)
        clarificationRequest.getId() != null
        clarificationRequest.getStudent().getId() == studentId
        clarificationRequest.getQuestion().getId() == questionId
        clarificationRequest.getDescription() == DESCRIPTION
    }

    def "the clarification request is empty"() {
        // an exception is thrown
        given:
        def studentId = studentRepository.findAll().get(0).getId()
        def questionId = questionRepository.findAll().get(0).getId()

        when:
        clarificationRequestService.createClarificationRequest(studentId, questionId, "   ")

        then:
        thrown(TutorException)
    }

    def "the clarification request is too large"() {
        // an exception is thrown
        given:
        def studentId = studentRepository.findAll().get(0).getId()
        def questionId = questionRepository.findAll().get(0).getId()

        when:
        clarificationRequestService.createClarificationRequest(studentId, questionId, DESCRIPTION_TOO_LARGE)

        then:
        thrown(TutorException)
    }

}
