package pt.ulisboa.tecnico.socialsoftware.tutor.clarification.api;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.security.access.annotation.Secured;
import org.springframework.web.bind.annotation.*;
import pt.ulisboa.tecnico.socialsoftware.tutor.clarification.ClarificationService;
import pt.ulisboa.tecnico.socialsoftware.tutor.clarification.dto.ClarificationDto;

import javax.validation.Valid;
import java.util.List;
import java.util.logging.Logger;

@RestController
@Secured({ "ROLE_ADMIN", "ROLE_TEACHER" })
public class ClarificationController {
    private static Logger logger = LoggerFactory.getLogger(ClarificationController.class);

    @Autowired
    private ClarificationService clarificationService;

    @GetMapping("/clarification/{clarificationId}")
    public List<ClarificationDto> getClarification(@PathVariable int clarificationId) {
        return this.clarificationService.findClarifications(clarificationId);
    }

    @PostMapping(value = "/users/{teacherId}/clarificationRequests/{clarificationRequestId}/clarifications/{clarificationDescription}")
    public ClarificationDto createClarification(@PathVariable int teacherId, @PathVariable int clarificationRequestId, @PathVariable String clarificationDescription) {
        return this.clarificationService.createClarification(teacherId, clarificationRequestId, clarificationDescription);
    }

    @PutMapping(value = "/clarifications/{clarificationId}")
    public ClarificationDto updateClarification(@PathVariable int clarificationId, @Valid @RequestBody ClarificationDto clarificationDto) {
        return this.clarificationService.updateClarification(clarificationId, clarificationDto);
    }


    @DeleteMapping("/clarifications/{clarificationId}")
    public ResponseEntity removeClarification(@PathVariable Integer clarificationId) {
        clarificationService.removeClarification(clarificationId);

        return ResponseEntity.ok().build();
    }

    @PutMapping(value = "/clarifications/{clarificationId}")
    public ClarificationDto changeClarificationAvaliable(@PathVariable Integer clarificationId) {
        return this.clarificationService.changeClarificationAvaliable(clarificationId);
    }
}
