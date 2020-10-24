package pt.ulisboa.tecnico.socialsoftware.tutor.clarificationRequest.repository;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;
import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;
import pt.ulisboa.tecnico.socialsoftware.tutor.clarificationRequest.domain.ClarificationRequest;

import java.util.List;
import java.util.Optional;

@Repository
@Transactional
public interface ClarificationRequestRepository extends JpaRepository<ClarificationRequest, Integer> {
    @Query(value = "SELECT * FROM clarificationRequests cR WHERE cR.question_id = :questionId", nativeQuery = true)
    List<ClarificationRequest> findClarificationRequests(int questionId);

    @Query(value = "SELECT * FROM clarificationRequests cR WHERE cR.id = :id", nativeQuery = true)
    ClarificationRequest findById(int id);

    @Query(value = "SELECT * FROM clarificationRequests", nativeQuery = true)
    List<ClarificationRequest> findAll();
}