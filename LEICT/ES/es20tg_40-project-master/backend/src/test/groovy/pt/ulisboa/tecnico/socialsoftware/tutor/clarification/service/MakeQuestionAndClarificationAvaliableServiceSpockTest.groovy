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

import static pt.ulisboa.tecnico.socialsoftware.tutor.exceptions.ErrorMessage.CLARIFICATION_NOT_FOUND
import static pt.ulisboa.tecnico.socialsoftware.tutor.exceptions.ErrorMessage.REQUEST_NOT_FOUND


@DataJpaTest
class MakeQuestionAndClarificationAvaliableServiceSpockTest extends Specification {
    static final String DESCRIPTION = "Bla"

    @Autowired
    ClarificationService clarificationService

    @Autowired
    ClarificationRequestService clarificationRequestService

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
        def clarification = new Clarification(teacher, clarificationRequest, DESCRIPTION)
        clarification.setId(1)
        clarificationRepository.save(clarification)
    }

    def "make the clarificationRequest and the respective clarification avaliable for all students"() {
        // the clarification request and the clarification are made avaliable
        given: "a teacher, a clarification request and a clarification"
        def teacherId = teacherRepository.findAll().get(0).getId()
        def clarificationRequestId = clarificationRequestRepository.findAll().get(0).getId()
        def clarificationId = clarificationRepository.findAll().get(0).getId()

        when:
        clarificationRequestService.changeClarificationAvaliable(clarificationRequestId)
        clarificationService.changeClarificationAvaliable(clarificationId)

        then: "clarificationRequest and clarification are made avaliable"
        clarificationRequestRepository.findAll().size() == 1
        def clarificationRequest = clarificationRequestRepository.findAll().get(0)
        def clarification = clarificationRepository.findAll().get(0)
        clarificationRequest.isAvaliable() == true
        clarification.isAvaliable() == true
    }

    def "the clarificationRequest doesnt exist"() {
        // an exception is thrown
        given:
        def teacherId = teacherRepository.findAll().get(0).getId()
        def clarificationId = clarificationRepository.findAll().get(0).getId()

        when:
        def clarificationRequestId = 0
        def clarificationRequest = clarificationRequestRepository.findById(clarificationRequestId)
        clarificationRequest == null
        clarificationRequestService.changeAvaliable(clarificationRequestId)

        then:
        new TutorException(REQUEST_NOT_FOUND, clarificationRequestId);
    }

    def "the clarification doesnt exist"() {
        // an exception is thrown
        given:
        def teacherId = teacherRepository.findAll().get(0).getId()
        def clarificationRequestId = clarificationRequestRepository.findAll().get(0).getId()

        when:
        def clarificationId = 0
        def clarification = clarificationRepository.findById(clarificationId)
        clarification == null
        clarificationService.changeClarificationAvaliable(clarificationId)

        then:
        new TutorException(CLARIFICATION_NOT_FOUND, clarificationId);
    }
}
