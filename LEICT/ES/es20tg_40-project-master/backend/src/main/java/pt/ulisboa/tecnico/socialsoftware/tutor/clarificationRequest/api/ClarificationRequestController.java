package pt.ulisboa.tecnico.socialsoftware.tutor.clarificationRequest.api;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.security.access.annotation.Secured;
import org.springframework.web.bind.annotation.*;
import pt.ulisboa.tecnico.socialsoftware.tutor.clarification.dto.ClarificationDto;
import pt.ulisboa.tecnico.socialsoftware.tutor.clarificationRequest.domain.ClarificationRequest;
import pt.ulisboa.tecnico.socialsoftware.tutor.clarificationRequest.dto.ClarificationRequestDto;
import pt.ulisboa.tecnico.socialsoftware.tutor.clarificationRequest.ClarificationRequestService;
import pt.ulisboa.tecnico.socialsoftware.tutor.question.dto.TopicDto;
import pt.ulisboa.tecnico.socialsoftware.tutor.user.User;
import pt.ulisboa.tecnico.socialsoftware.tutor.question.domain.Question;
import javax.validation.Valid;
import java.util.List;

@RestController
@Secured({ "ROLE_ADMIN", "ROLE_STUDENT" })
public class ClarificationRequestController {
    private static Logger logger = LoggerFactory.getLogger(ClarificationRequestController.class);

    @Autowired
    private ClarificationRequestService clarificationRequestService;

    @GetMapping("/clarificationRequests/{clarificationRequestId}")
    public List<ClarificationRequestDto> getClarificationRequests(@PathVariable int clarificationRequestId) {
        return this.clarificationRequestService.findClarificationRequests(clarificationRequestId);
    }

    @PostMapping(value = "/users/{studentId}/questions/{questionId}/clarificationRequests/{clarificationRequestDescription}")
    public ClarificationRequestDto createClarificationRequest(@PathVariable int studentId, @PathVariable int questionId, @PathVariable String clarificationRequestDescription) {
        return this.clarificationRequestService.createClarificationRequest(studentId, questionId, clarificationRequestDescription);
    }

    @PutMapping(value = "/clarificationRequests/{clarificationRequestId}")
    public ClarificationRequestDto updateClarificationRequest(@PathVariable int clarificationRequestId, @Valid @RequestBody ClarificationRequestDto clarificationRequestDto) {
        return this.clarificationRequestService.updateClarificationRequest(clarificationRequestId, clarificationRequestDto);
    }


    @DeleteMapping("/clarificationRequests/{clarificationRequestId}")
    public ResponseEntity removeClarificationRequest(@PathVariable Integer clarificationRequestId) {
        clarificationRequestService.removeClarificationRequest(clarificationRequestId);

        return ResponseEntity.ok().build();
    }

    @PutMapping(value = "/clarificationRequests/{clarificationRequestId}")
    public ClarificationRequestDto changeClarificationRequestAvaliable(@PathVariable Integer clarificationRequestId) {
        return this.clarificationRequestService.changeClarificationRequestAvaliable(clarificationRequestId);
    }

}
