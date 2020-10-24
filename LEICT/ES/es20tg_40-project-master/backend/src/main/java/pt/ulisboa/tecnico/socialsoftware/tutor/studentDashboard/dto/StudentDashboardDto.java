package pt.ulisboa.tecnico.socialsoftware.tutor.studentDashboard.dto;

import pt.ulisboa.tecnico.socialsoftware.tutor.clarification.domain.Clarification;
import pt.ulisboa.tecnico.socialsoftware.tutor.clarificationRequest.domain.ClarificationRequest;
import pt.ulisboa.tecnico.socialsoftware.tutor.studentDashboard.domain.StudentDashboard;

import java.util.ArrayList;
import java.util.List;

public class StudentDashboardDto {

    private Integer id;
    private Integer studentId;
    private ArrayList<ClarificationRequest> clarificationRequests;
    private ArrayList<ClarificationRequest> avaliableClarificationRequests;
    private ArrayList<Clarification> clarifications;
    private Integer numberOfClarificationRequests;
    private Integer numberOfAvaliableClarificationRequests;
    private Integer numberOfClarifications;
    private boolean publicInformation;

    public StudentDashboardDto() {

    }

    public StudentDashboardDto(StudentDashboard studentDashboard) {
        this.id = studentDashboard.getId();
        this.studentId = studentDashboard.getStudent().getId();
        this.clarificationRequests = studentDashboard.getClarificationRequests();
        this.avaliableClarificationRequests = studentDashboard.getAvaliableClarificationRequests();
        this.clarifications = studentDashboard.getClarifications();
        this.numberOfClarificationRequests = studentDashboard.getNumberOfClarificationRequests();
        this.numberOfAvaliableClarificationRequests = studentDashboard.getNumberOfAvaliableClarificationRequests();
        this.numberOfClarifications = studentDashboard.getNumberOfClarifications();
        this.publicInformation = studentDashboard.isPublicInformation();
    }

    public Integer getId() {
        return id;
    }

    public void setId(Integer id) {
        this.id = id;
    }

    public Integer getStudentId() {
        return studentId;
    }

    public void setStudentId(Integer studentId) {
        this.studentId = studentId;
    }

    public ArrayList<ClarificationRequest> getClarificationRequests() {
        return clarificationRequests;
    }

    public void setClarificationRequests(ArrayList<ClarificationRequest> clarificationRequests) {
        this.clarificationRequests = clarificationRequests;
    }

    public ArrayList<ClarificationRequest> getAvaliableClarificationRequests() {
        return avaliableClarificationRequests;
    }

    public void setAvaliableClarificationRequests(ArrayList<ClarificationRequest> avaliableClarificationRequests) {
        this.avaliableClarificationRequests = avaliableClarificationRequests;
    }

    public ArrayList<Clarification> getClarifications() {
        return clarifications;
    }

    public void setClarifications(ArrayList<Clarification> clarifications) {
        this.clarifications = clarifications;
    }

    public Integer getNumberOfClarificationRequests() {
        return numberOfClarificationRequests;
    }

    public void setNumberOfClarificationRequests(Integer numberOfClarificationRequests) {
        this.numberOfClarificationRequests = numberOfClarificationRequests;
    }

    public Integer getNumberOfAvaliableClarificationRequests() {
        return numberOfAvaliableClarificationRequests;
    }

    public void setNumberOfAvaliableClarificationRequests(Integer numberOfAvaliableClarificationRequests) {
        this.numberOfAvaliableClarificationRequests = numberOfAvaliableClarificationRequests;
    }

    public Integer getNumberOfClarifications() {
        return numberOfClarifications;
    }

    public void setNumberOfClarifications(Integer numberOfClarifications) {
        this.numberOfClarifications = numberOfClarifications;
    }

    public boolean isPublicInformation() {
        return publicInformation;
    }

    public void setPublicInformation(boolean publicInformation) {
        this.publicInformation = publicInformation;
    }

    @Override
    public String toString() {
        return "StudentDashboardDto{" +
                "id=" + id +
                ", studentId=" + studentId +
                ", numberOfClarificationRequests=" + numberOfClarificationRequests +
                ", numberOfAvaliableClarificationRequests=" + numberOfAvaliableClarificationRequests +
                ", numberOfClarifications=" + numberOfClarifications +
                ", publicInformation=" + publicInformation +
                '}';
    }

}
