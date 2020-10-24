package pt.ulisboa.tecnico.socialsoftware.tutor.clarificationRequest;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.retry.annotation.Backoff;
import org.springframework.retry.annotation.Retryable;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Isolation;
import org.springframework.transaction.annotation.Transactional;
import pt.ulisboa.tecnico.socialsoftware.tutor.clarification.domain.Clarification;
import pt.ulisboa.tecnico.socialsoftware.tutor.clarification.dto.ClarificationDto;
import pt.ulisboa.tecnico.socialsoftware.tutor.exceptions.TutorException;
import pt.ulisboa.tecnico.socialsoftware.tutor.question.domain.Question;
import pt.ulisboa.tecnico.socialsoftware.tutor.question.repository.QuestionRepository;
import pt.ulisboa.tecnico.socialsoftware.tutor.clarificationRequest.repository.ClarificationRequestRepository;
import pt.ulisboa.tecnico.socialsoftware.tutor.clarificationRequest.dto.ClarificationRequestDto;
import pt.ulisboa.tecnico.socialsoftware.tutor.clarificationRequest.domain.ClarificationRequest;
import pt.ulisboa.tecnico.socialsoftware.tutor.user.UserRepository;
import pt.ulisboa.tecnico.socialsoftware.tutor.user.User;

import static pt.ulisboa.tecnico.socialsoftware.tutor.exceptions.ErrorMessage.QUESTION_NOT_FOUND;
import static pt.ulisboa.tecnico.socialsoftware.tutor.exceptions.ErrorMessage.STUDENT_NOT_FOUND;

import javax.persistence.EntityManager;
import javax.persistence.PersistenceContext;
import java.sql.SQLException;
import java.util.List;
import java.util.stream.Collectors;

import static pt.ulisboa.tecnico.socialsoftware.tutor.exceptions.ErrorMessage.*;

@Service
public class ClarificationRequestService {

    @Autowired
    private ClarificationRequestRepository clarificationRequestRepository;

    @Autowired
    private QuestionRepository questionRepository;

    @Autowired
    private UserRepository studentRepository;

    @PersistenceContext
    EntityManager entityManager;

    @Retryable(
            value = { SQLException.class },
            backoff = @Backoff(delay = 5000))
    @Transactional(isolation = Isolation.REPEATABLE_READ)
    public ClarificationRequestDto createClarificationRequest(Integer studentId, Integer questionId, String description) {
        User student = studentRepository.findById(studentId).orElseThrow(() -> new TutorException(STUDENT_NOT_FOUND, studentId));

        Question question = questionRepository.findById(questionId).orElseThrow(() -> new TutorException(QUESTION_NOT_FOUND, questionId));

        ClarificationRequest clarificationRequest = new ClarificationRequest(student, question, description);
        student.addClarificationRequest(clarificationRequest);
        entityManager.persist(clarificationRequest);

        return new ClarificationRequestDto(clarificationRequest);
    }

    @Retryable(
            value = { SQLException.class },
            backoff = @Backoff(delay = 5000))
    @Transactional(isolation = Isolation.REPEATABLE_READ)
    public ClarificationRequestDto findClarificationRequestById(Integer clarificationRequestId) {
        return clarificationRequestRepository.findById(clarificationRequestId).map(ClarificationRequestDto::new)
                .orElseThrow(() -> new TutorException(QUESTION_NOT_FOUND, clarificationRequestId));
    }

    @Retryable(
            value = { SQLException.class },
            backoff = @Backoff(delay = 5000))
    @Transactional(isolation = Isolation.REPEATABLE_READ)
    public List<ClarificationRequestDto> findClarificationRequests(int clarificationRequestId) {
        return clarificationRequestRepository.findClarificationRequests(clarificationRequestId).stream().map(ClarificationRequestDto::new).collect(Collectors.toList());
    }

    @Retryable(
            value = { SQLException.class },
            backoff = @Backoff(delay = 5000))
    @Transactional(isolation = Isolation.REPEATABLE_READ)
    public ClarificationRequestDto updateClarificationRequest(Integer clarificationRequestId, ClarificationRequestDto clarificationRequestDto) {
        ClarificationRequest clarificationRequest = clarificationRequestRepository.findById(clarificationRequestId).orElseThrow(() -> new TutorException(REQUEST_NOT_FOUND, clarificationRequestId));

        clarificationRequest.setDescription(clarificationRequestDto.getDescription());
        return new ClarificationRequestDto(clarificationRequest);
    }

    @Retryable(
            value = { SQLException.class },
            backoff = @Backoff(delay = 5000))
    @Transactional(isolation = Isolation.REPEATABLE_READ)
    public void removeClarificationRequest(Integer clarificationRequestId) {
        ClarificationRequest clarificationRequest = clarificationRequestRepository.findById(clarificationRequestId).orElseThrow(() -> new TutorException(REQUEST_NOT_FOUND, clarificationRequestId));

        entityManager.remove(clarificationRequest);
    }

    @Retryable(
            value = { SQLException.class },
            backoff = @Backoff(delay = 5000))
    @Transactional(isolation = Isolation.REPEATABLE_READ)
    public ClarificationRequestDto changeClarificationRequestAvaliable(Integer clarificationRequestId) {
        ClarificationRequest clarificationRequest = clarificationRequestRepository.findById(clarificationRequestId).orElseThrow(() -> new TutorException(REQUEST_NOT_FOUND, clarificationRequestId));
        if(clarificationRequest.isAvaliable() == false)
            clarificationRequest.setAvaliable(true);
        else
            clarificationRequest.setAvaliable(false);
        return new ClarificationRequestDto(clarificationRequest);
    }

}
