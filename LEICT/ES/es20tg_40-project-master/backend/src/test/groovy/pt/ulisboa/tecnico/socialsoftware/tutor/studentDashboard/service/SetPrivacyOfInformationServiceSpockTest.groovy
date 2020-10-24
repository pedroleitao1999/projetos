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

    def "make the information public"() {
        // the information from the studentDashboard is now public
        given: "a student and a studentDashboard"
        def studentId = studentRepository.findAll().get(0).getId()
        def studentDashboardId = studentDashboardRepository.findAll().get(0).getId()

        when:
        studentDashboardService.definePrivacyOfInformation(studentDashboardId, true)

        then: "the information from the studentDashboard is now public"
        studentDashboardRepository.findAll().size() == 1
        def studentDashboard = studentDashboardRepository.findAll().get(0)
        studentDashboard.isPublicInformation() == true
    }

    def "make the information private"() {
        // the information from the studentDashboard is now private
        given: "a student and a studentDashboard"
        def studentId = studentRepository.findAll().get(0).getId()
        def studentDashboardId = studentDashboardRepository.findAll().get(0).getId()

        when:
        studentDashboardService.definePrivacyOfInformation(studentDashboardId, false)

        then: "the information from the studentDashboard is now private"
        studentDashboardRepository.findAll().size() == 1
        def studentDashboard = studentDashboardRepository.findAll().get(0)
        studentDashboard.isPublicInformation() == false
    }

}