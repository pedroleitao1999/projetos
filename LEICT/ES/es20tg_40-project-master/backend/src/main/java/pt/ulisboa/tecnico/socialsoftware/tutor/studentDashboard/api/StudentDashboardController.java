package pt.ulisboa.tecnico.socialsoftware.tutor.studentDashboard.api;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.security.access.annotation.Secured;
import org.springframework.web.bind.annotation.*;
import pt.ulisboa.tecnico.socialsoftware.tutor.clarificationRequest.dto.ClarificationRequestDto;
import pt.ulisboa.tecnico.socialsoftware.tutor.studentDashboard.dto.StudentDashboardDto;
import pt.ulisboa.tecnico.socialsoftware.tutor.studentDashboard.StudentDashboardService;
import pt.ulisboa.tecnico.socialsoftware.tutor.studentDashboard.domain.StudentDashboard;

import java.util.List;

@RestController
@Secured({ "ROLE_ADMIN", "ROLE_STUDENT" })
public class StudentDashboardController {
    private static Logger logger = LoggerFactory.getLogger(StudentDashboardController.class);

    @Autowired
    private StudentDashboardService studentDashboardService;

    @PostMapping(value = "/users/{studentId}")
    public StudentDashboardDto createStudentDashboard(@PathVariable int studentId) {
        return this.studentDashboardService.createStudentDashboard(studentId);
    }

    @PutMapping(value = "/studentDashboards/{studentDashboardId}")
    public StudentDashboardDto updateStudentDashboard(@PathVariable int studentDashboardId, @Valid @RequestBody StudentDashboardDto studentDashboardDto) {
        return this.studentDashboardService.updateStudentDashboard(studentDashboardId, studentDashboardDto);
    }

    @GetMapping("/studentDashboards/{studentDashboardId}")
    public int showNumberOfClarificationRequests(@PathVariable int studentDashboardId) {
        return this.studentDashboardService.showNumberOfClarificationRequests(studentDashboardId);
    }

    @GetMapping("/studentDashboards/{studentDashboardId}")
    public int showNumberOfAvaliableClarificationRequests(@PathVariable int studentDashboardId) {
        return this.studentDashboardService.showNumberOfAvaliableClarificationRequests(studentDashboardId);
    }

    @GetMapping("/studentDashboards/{studentDashboardId}")
    public int showNumberOfClarifications(@PathVariable int studentDashboardId) {
        return this.studentDashboardService.showNumberOfClarifications(studentDashboardId);
    }

    @PostMapping("/studentDashboards/{studentDashboardId}/studentDashboards/{publicInformation}")
    public StudentDashboardDto definePrivacyOfInformation(@PathVariable int studentDashboardId, @PathVariable boolean publicInformation) {
        return this.studentDashboardService.definePrivacyOfInformation(studentDashboardId, publicInformation);
    }

}
