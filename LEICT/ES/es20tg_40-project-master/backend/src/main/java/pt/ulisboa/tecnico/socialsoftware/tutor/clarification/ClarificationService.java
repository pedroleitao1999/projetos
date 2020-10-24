package pt.ulisboa.tecnico.socialsoftware.tutor.clarification;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.retry.annotation.Backoff;
import org.springframework.retry.annotation.Retryable;
import org.springframework.transaction.annotation.Isolation;
import javax.persistence.EntityManager;
import javax.persistence.PersistenceContext;
import pt.ulisboa.tecnico.socialsoftware.tutor.exceptions.TutorException;
import pt.ulisboa.tecnico.socialsoftware.tutor.user.User;
import pt.ulisboa.tecnico.socialsoftware.tutor.user.UserRepository;
import pt.ulisboa.tecnico.socialsoftware.tutor.clarificationRequest.domain.ClarificationRequest;
import pt.ulisboa.tecnico.socialsoftware.tutor.clarificationRequest.repository.ClarificationRequestRepository;
import pt.ulisboa.tecnico.socialsoftware.tutor.clarification.domain.Clarification;
import pt.ulisboa.tecnico.socialsoftware.tutor.clarification.repository.ClarificationRepository;
import pt.ulisboa.tecnico.socialsoftware.tutor.clarification.dto.ClarificationDto;

import java.sql.SQLException;
import java.util.List;
import java.util.stream.Collectors;

import static pt.ulisboa.tecnico.socialsoftware.tutor.exceptions.ErrorMessage.*;

@Service
public class ClarificationService {
    private ClarificationRepository clarificationRepository;

    private UserRepository userRepository;

    private ClarificationRequestRepository clarificationRequestRepository;

    @PersistenceContext
    EntityManager entityManager;

    @Transactional
    public ClarificationDto createClarification(Integer userId, Integer requestId, String description) {
        User user = userRepository.findById(userId).orElseThrow(() -> new TutorException(USER_NOT_FOUND, userId));

        ClarificationRequest request = clarificationRequestRepository.findById(requestId).orElseThrow(() -> new TutorException(REQUEST_NOT_FOUND, requestId));

        Clarification clarification = new Clarification(user, request, description);
        entityManager.persist(clarification);

        return new ClarificationDto(clarification);
    }

    @Retryable(
            value = { SQLException.class },
            backoff = @Backoff(delay = 5000))
    @Transactional(isolation = Isolation.REPEATABLE_READ)
    public List<ClarificationDto> findClarifications(int clarificationId) {
        return clarificationRepository.findClarifications(clarificationId).stream().map(ClarificationDto::new).collect(Collectors.toList());
    }

    @Retryable(
            value = { SQLException.class },
            backoff = @Backoff(delay = 5000))
    @Transactional(isolation = Isolation.REPEATABLE_READ)
    public ClarificationDto updateClarification(Integer clarificationId, ClarificationDto clarificationDto) {
        Clarification clarification = clarificationRepository.findById(clarificationId).orElseThrow(() -> new TutorException(CLARIFICATION_NOT_FOUND, clarificationId));

        clarification.setDescription(clarificationDto.getDescription());
        return new ClarificationDto(clarification);
    }

    @Retryable(
            value = { SQLException.class },
            backoff = @Backoff(delay = 5000))
    @Transactional(isolation = Isolation.REPEATABLE_READ)
    public void removeClarification(Integer clarificationId) {
        Clarification clarification = clarificationRepository.findById(clarificationId).orElseThrow(() -> new TutorException(CLARIFICATION_NOT_FOUND, clarificationId));

        entityManager.remove(clarification);
    }

    @Retryable(
            value = { SQLException.class },
            backoff = @Backoff(delay = 5000))
    @Transactional(isolation = Isolation.REPEATABLE_READ)
    public ClarificationDto changeClarificationAvaliable(Integer clarificationId) {
        Clarification clarification = clarificationRepository.findById(clarificationId).orElseThrow(() -> new TutorException(CLARIFICATION_NOT_FOUND, clarificationId));

        if(clarification.isAvaliable() == false)
            clarification.setAvaliable(true);
        else
            clarification.setAvaliable(false);
        return new ClarificationDto(clarification);
    }

}