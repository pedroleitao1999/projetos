package pt.ulisboa.tecnico.socialsoftware.tutor.studentDashboard;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.retry.annotation.Backoff;
import org.springframework.retry.annotation.Retryable;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Isolation;
import org.springframework.transaction.annotation.Transactional;
import pt.ulisboa.tecnico.socialsoftware.tutor.exceptions.TutorException;
import pt.ulisboa.tecnico.socialsoftware.tutor.studentDashboard.domain.StudentDashboard;
import pt.ulisboa.tecnico.socialsoftware.tutor.studentDashboard.dto.StudentDashboardDto;
import pt.ulisboa.tecnico.socialsoftware.tutor.studentDashboard.repository.StudentDashboardRepository;
import pt.ulisboa.tecnico.socialsoftware.tutor.user.User;
import pt.ulisboa.tecnico.socialsoftware.tutor.user.UserRepository;

import javax.persistence.EntityManager;
import javax.persistence.PersistenceContext;
import java.sql.SQLException;

import static pt.ulisboa.tecnico.socialsoftware.tutor.exceptions.ErrorMessage.DASHBOARD_NOT_FOUND;
import static pt.ulisboa.tecnico.socialsoftware.tutor.exceptions.ErrorMessage.STUDENT_NOT_FOUND;

public class StudentDashboardService {

    @Autowired
    private UserRepository studentRepository;

    @Autowired
    private StudentDashboardRepository studentDashboardRepository;

    @PersistenceContext
    EntityManager entityManager;

    @Retryable(
            value = { SQLException.class },
            backoff = @Backoff(delay = 5000))
    @Transactional(isolation = Isolation.REPEATABLE_READ)
    public StudentDashboardDto createStudentDashboard(Integer studentId) {
        User student = studentRepository.findByKey(studentId).orElseThrow(() -> new TutorException(STUDENT_NOT_FOUND, studentId));

        StudentDashboard studentDashboard = new StudentDashboard(student);
        student.setStudentDashboard(studentDashboard);
        entityManager.persist(studentDashboard);

        return new StudentDashboardDto(studentDashboard);
    }

    @Retryable(
            value = { SQLException.class },
            backoff = @Backoff(delay = 5000))
    @Transactional(isolation = Isolation.REPEATABLE_READ)
    public StudentDashboardDto updateStudentDashboard(Integer studentDashboardId, StudentDashboardDto studentDashboardDto) {
        StudentDashboard studentDashboard = studentDashboardRepository.findById(studentDashboardId).orElseThrow(() -> new TutorException(DASHBOARD_NOT_FOUND, studentDashboardId));

        studentDashboard.setClarificationRequests(studentDashboardDto.getClarificationRequests());
        studentDashboard.setAvaliableClarificationRequests(studentDashboardDto.getAvaliableClarificationRequests());
        studentDashboard.setClarifications(studentDashboardDto.getClarifications());
        studentDashboard.setNumberOfClarificationRequests(studentDashboardDto.getNumberOfClarificationRequests());
        studentDashboard.setNumberOfAvaliableClarificationRequests(studentDashboardDto.getNumberOfAvaliableClarificationRequests());
        studentDashboard.setNumberOfClarifications(studentDashboardDto.getNumberOfClarifications());

        return new StudentDashboardDto(studentDashboard);
    }

    @Retryable(
            value = { SQLException.class },
            backoff = @Backoff(delay = 5000))
    @Transactional(isolation = Isolation.REPEATABLE_READ)
    public int showNumberOfClarificationRequests(int studentDashboardId) {
        StudentDashboard studentDashboard = studentDashboardRepository.findById(studentDashboardId);
        int numberOfClarificationRequests = studentDashboard.getNumberOfClarificationRequests();
        return numberOfClarificationRequests;
    }

    @Retryable(
            value = { SQLException.class },
            backoff = @Backoff(delay = 5000))
    @Transactional(isolation = Isolation.REPEATABLE_READ)
    public int showNumberOfAvaliableClarificationRequests(int studentDashboardId) {
        StudentDashboard studentDashboard = studentDashboardRepository.findById(studentDashboardId);
        int numberOfAvaliableClarificationRequests = studentDashboard.getNumberOfAvaliableClarificationRequests();
        return numberOfAvaliableClarificationRequests;
    }

    @Retryable(
            value = { SQLException.class },
            backoff = @Backoff(delay = 5000))
    @Transactional(isolation = Isolation.REPEATABLE_READ)
    public int showNumberOfClarifications(int studentDashboardId) {
        StudentDashboard studentDashboard = studentDashboardRepository.findById(studentDashboardId);
        int numberOfClarifications = studentDashboard.getNumberOfClarifications();
        return numberOfClarifications;
    }

    @Retryable(
            value = { SQLException.class },
            backoff = @Backoff(delay = 5000))
    @Transactional(isolation = Isolation.REPEATABLE_READ)
    public StudentDashboardDto definePrivacyOfInformation(int studentDashboardId, boolean publicInformation) {
        StudentDashboard studentDashboard = studentDashboardRepository.findById(studentDashboardId);
        studentDashboard.setPublicInformation(publicInformation);
        return new StudentDashboardDto(studentDashboard);
    }

}
