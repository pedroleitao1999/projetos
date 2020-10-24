package pt.ulisboa.tecnico.socialsoftware.tutor.studentDashboard.repository;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;
import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;
import pt.ulisboa.tecnico.socialsoftware.tutor.studentDashboard.domain.StudentDashboard;

import java.util.List;

@Repository
@Transactional
public interface StudentDashboardRepository extends JpaRepository<StudentDashboard, Integer> {
    @Query(value = "SELECT * FROM studentDashboards sD WHERE sD.id = :id", nativeQuery = true)
    StudentDashboard findById(int id);

    @Query(value = "SELECT * FROM studentDashboards", nativeQuery = true)
    List<StudentDashboard> findAll();
}
