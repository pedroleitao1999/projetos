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
class CreateAdditionalClarificationRequestServiceSpockTest extends Specification {
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
        def firstClarificationRequest = new ClarificationRequest(tudentId, questionId, DESCRIPTION);
        clarificationRequestRepository.save(firstClarificationRequest);
        firstClarificationRequest.setKey(1);
    }

    def "create an additional clarification request"() {
        // an additional clarification request is created
        given: "a student, a question and a clarificationRequest"
        def studentId = studentRepository.findAll().get(0).getId()
        def questionId = questionRepository.findAll().get(0).getId()
        def clarificationRequestId = clarificationRequestRepository.findAll().get(0).getId()

        when:
        clarificationRequestService.createClarificationRequest(studentId, questionId, DESCRIPTION)

        then: "an additional clarification request is created"
        clarificationRequestRepository.findAll().size() == 1
        def additionalClarificationRequest = clarificationRequestRepository.findAll().get(0)
        additionalClarificationRequest.getId() != null
        additionalClarificationRequest.getStudent().getId() == studentId
        additionalClarificationRequest.getQuestion().getId() == questionId
        additionalClarificationRequest.getDescription() == DESCRIPTION
    }

    def "the additional clarification request is empty"() {
        // an exception is thrown
        given:
        def studentId = studentRepository.findAll().get(0).getId()
        def questionId = questionRepository.findAll().get(0).getId()

        when:
        clarificationRequestService.createClarificationRequest(studentId, questionId, "   ")

        then:
        thrown(TutorException)
    }

    def "the additional clarification request is too large"() {
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