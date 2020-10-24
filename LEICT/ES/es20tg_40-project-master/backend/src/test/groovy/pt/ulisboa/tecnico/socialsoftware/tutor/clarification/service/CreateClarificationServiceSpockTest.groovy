package pt.ulisboa.tecnico.socialsoftware.tutor.clarification.service

import org.springframework.beans.factory.annotation.Autowired
import org.springframework.boot.test.autoconfigure.orm.jpa.DataJpaTest
import pt.ulisboa.tecnico.socialsoftware.tutor.clarification.ClarificationService
import pt.ulisboa.tecnico.socialsoftware.tutor.clarification.repository.ClarificationRepository
import pt.ulisboa.tecnico.socialsoftware.tutor.clarificationRequest.repository.ClarificationRequestRepository
import pt.ulisboa.tecnico.socialsoftware.tutor.clarificationRequest.domain.ClarificationRequest
import pt.ulisboa.tecnico.socialsoftware.tutor.exceptions.TutorException
import pt.ulisboa.tecnico.socialsoftware.tutor.user.User
import pt.ulisboa.tecnico.socialsoftware.tutor.user.UserRepository
import spock.lang.Specification

import static pt.ulisboa.tecnico.socialsoftware.tutor.exceptions.ErrorMessage.CLARIFICATION_IS_EMPTY
import static pt.ulisboa.tecnico.socialsoftware.tutor.exceptions.ErrorMessage.CLARIFICATION_SIZE_LIMIT_EXCEEDED


@DataJpaTest
class CreateClarificationServiceSpockTest extends Specification {
    static final String TEACHER = "TeacherOne"
    static final String REQUEST = "R1"
    static final String DESCRIPTION = "Bla"
    static final String EMPTY_DESCRIPTION = ""
    static final String LONG_DESCRIPTION = "Bla Bla Bla Bla Bla Bla Bla Bla Bla Bla Bla Bla Bla Bla Bla Bla Bla Bla Bla Bla Bla Bla Bla Bla Bla Bla Bla Bla Bla Bla Bla Bla Bla Bla Bla Bla Bla Bla"

    @Autowired
    ClarificationService clarificationService

    @Autowired
    ClarificationRepository clarificationRepository

    @Autowired
    ClarificationRequestRepository clarificationRequestRepository

    @Autowired
    UserRepository teacherRepository

    def setup() {
        def teacher = new User()
        teacher.setKey(1)
        teacherRepository.save(teacher)
        def clarificationRequest = new ClarificationRequest()
        clarificationRequest.setId(1)
        clarificationRequestRepository.save(clarificationRequest)
    }

    def "create a clarification"() {
        // the clarification is created
        given: "a teacher and a clarification request"
        def teacherId = teacherRepository.findAll().get(0).getId()
        def clarificationRequestId = clarificationRequestRepository.findAll().get(0).getId()

        when:
        clarificationService.createClarification(teacherId, clarificationRequestId, DESCRIPTION)

        then: "clarification is created"
        clarificationRepository.findAll().size() == 1
        def clarification = clarificationRepository.findAll().get(0)
        clarification.getId() != null
        clarification.getTeacher().getId() == teacherId
        clarification.getRequest().getId() == clarificationRequestId
        clarification.getDescription() == DESCRIPTION
    }

    def "the clarification is empty"() {
        // an exception is thrown
        given:
        def teacherId = teacherRepository.findAll().get(0).getId()
        def clarificationRequestId = clarificationRequestRepository.findAll().get(0).getId()

        when:
        clarificationService.createClarification(teacherId, clarificationRequestId, EMPTY_DESCRIPTION)

        then:
        thrown(TutorException)
    }

    def "the clarification is too large"() {
        // an exception is thrown
        given:
        def teacherId = teacherRepository.findAll().get(0).getId()
        def clarificationRequestId = clarificationRequestRepository.findAll().get(0).getId()

        when:
        clarificationService.createClarification(teacherId, clarificationRequestId, LONG_DESCRIPTION)

        then:
        thrown(TutorException)
    }
}
