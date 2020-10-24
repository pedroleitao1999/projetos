package pt.ulisboa.tecnico.socialsoftware.tutor.clarification.repository;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;
import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;
import pt.ulisboa.tecnico.socialsoftware.tutor.clarification.domain.Clarification;
import pt.ulisboa.tecnico.socialsoftware.tutor.clarificationRequest.domain.ClarificationRequest;

import java.util.List;

@Repository
@Transactional
public interface ClarificationRepository extends JpaRepository<Clarification, Integer> {
    @Query(value = "SELECT * FROM clarifications c WHERE c.clarification_request_id = :clarificationRequestId", nativeQuery = true)
    List<Clarification> findClarifications(int clarificationRequestId);

    @Query(value = "SELECT * FROM clarifications c WHERE c.id = :id", nativeQuery = true)
    Clarification findById(int id);

    @Query(value = "SELECT * FROM clarifications", nativeQuery = true)
    List<Clarification> findAll();

}

