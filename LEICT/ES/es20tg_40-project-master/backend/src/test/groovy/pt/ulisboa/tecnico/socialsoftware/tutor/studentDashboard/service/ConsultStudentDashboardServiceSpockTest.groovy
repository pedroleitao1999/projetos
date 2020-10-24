package pt.ulisboa.tecnico.socialsoftware.tutor.studentDashboard.service

import org.springframework.beans.factory.annotation.Autowired
import org.springframework.boot.test.autoconfigure.orm.jpa.DataJpaTest
import pt.ulisboa.tecnico.socialsoftware.tutor.studentDashboard.StudentDashboardService
import pt.ulisboa.tecnico.socialsoftware.tutor.studentDashboard.domain.StudentDashboard
import pt.ulisboa.tecnico.socialsoftware.tutor.studentDashboard.repository.StudentDashboardRepository
import pt.ulisboa.tecnico.socialsoftware.tutor.user.User
import pt.ulisboa.tecnico.socialsoftware.tutor.user.UserRepository

@DataJpaTest
class ConsultStudentDashboardServiceSpockTest extends Specification {

    @Autowired
    UserRepository studentRepository

    @Autowired
    StudentDashboardRepository studentDashboardRepository

    @Autowired
    StudentDashboardService studentDashboardService

    def setup() {
        def student = new User()
        student.setKey(1)
        studentRepository.save(student)
        def studentDashboard = new StudentDashboard(student)
        studentDashboard.setKey(1)
        studentDashboardRepository.save(studentDashboard)
    }

    def "consult number of clarificationRequests"() {
        // the number of clarificationRequests is shown
        given: "a student and a studentDashboard"
        def studentId = studentRepository.findAll().get(0).getId()
        def studentDashboardId = studentDashboardRepository.findAll().get(0).getId()

        when:
        int numberOfClarificationRequests = studentDashboardService.showNumberOfClarificationRequests(studentId)

        then: "the number of clarificationRequests is shown"
        numberOfClarificationRequests >= 0
    }

    def "consult number of avaliableClarificationRequests"() {
        // the number of avaliableClarificationRequests is shown
        given: "a student and a studentDashboard"
        def studentId = studentRepository.findAll().get(0).getId()
        def studentDashboardId = studentDashboardRepository.findAll().get(0).getId()

        when:
        int numberOfAvaliableClarificationRequests = studentDashboardService.showNumberOfAvaliableClarificationRequests(studentId)

        then: "the number of avaliableClarificationRequests is shown"
        numberOfAvaliableClarificationRequests >= 0
    }

    def "consult number of clarifications"() {
        // the number of clarifications is shown
        given: "a student and a studentDashboard"
        def studentId = studentRepository.findAll().get(0).getId()
        def studentDashboardId = studentDashboardRepository.findAll().get(0).getId()

        when:
        int numberOfClarifications = studentDashboardService.showNumberOfClarifications(studentId)

        then: "the number of clarifications is shown"
        numberOfClarifications >= 0
    }

}