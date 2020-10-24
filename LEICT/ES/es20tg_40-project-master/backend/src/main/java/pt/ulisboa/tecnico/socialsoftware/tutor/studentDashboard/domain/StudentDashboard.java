package pt.ulisboa.tecnico.socialsoftware.tutor.studentDashboard.domain;

import pt.ulisboa.tecnico.socialsoftware.tutor.clarification.domain.Clarification;
import pt.ulisboa.tecnico.socialsoftware.tutor.clarificationRequest.domain.ClarificationRequest;
import pt.ulisboa.tecnico.socialsoftware.tutor.user.User;

import javax.persistence.*;
import java.util.ArrayList;

@Entity
@Table(name = "studentDashboards")
public class StudentDashboard {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Integer id;

    @Column(unique=true)
    private Integer numberOfClarificationRequests;
    private Integer numberOfAvaliableClarificationRequests;
    private Integer numberOfClarifications;

    @OneToOne
    @JoinColumn(name = "student_id")
    private User student;

    @OneToMany
    @JoinColumn(name = "clarificationRequest_id")
    private ArrayList<ClarificationRequest> clarificationRequests = new ArrayList<ClarificationRequest>();

    @OneToMany
    @JoinColumn(name = "clarificationRequest_id")
    private ArrayList<ClarificationRequest> avaliableClarificationRequests = new ArrayList<ClarificationRequest>();

    @OneToMany
    @JoinColumn(name = "clarification_id")
    private ArrayList<Clarification> clarifications = new ArrayList<Clarification>();

    @Column
    private boolean publicInformation;

    public StudentDashboard() {

    }

    public StudentDashboard(User student) {
        this.student = student;
        this.clarificationRequests = student.getClarificationRequests();
        this.clarifications = student.getClarifications();
        this.numberOfClarificationRequests = clarificationRequests.size();
        this.numberOfClarifications = clarifications.size();
        this.publicInformation = false;

        this.avaliableClarificationRequests = new ArrayList<ClarificationRequest>();

        for(ClarificationRequest cR : clarificationRequests)
            if(cR.isAvaliable() == true)
                avaliableClarificationRequests.add(cR);

        this.numberOfAvaliableClarificationRequests = avaliableClarificationRequests.size();

    }

    public Integer getId() {
        return id;
    }

    public void setId(Integer id) {
        this.id = id;
    }

    public User getStudent() {
        return student;
    }

    public void setStudent(User student) {
        this.student = student;
    }

    public ArrayList<ClarificationRequest> getClarificationRequests() {
        return clarificationRequests;
    }

    public void addClarificationRequest(ClarificationRequest clarificationRequest) {
        clarificationRequests.add(clarificationRequest);
        numberOfClarificationRequests++;
    }

    public ArrayList<ClarificationRequest> getAvaliableClarificationRequests() {
        return avaliableClarificationRequests;
    }

    public void addAvaliableClarificationRequest(ClarificationRequest avaliableClarificationRequest) {
        avaliableClarificationRequests.add(avaliableClarificationRequest);
        numberOfAvaliableClarificationRequests++;
    }

    public ArrayList<Clarification> getClarifications() {
        return clarifications;
    }

    public void addClarification(Clarification clarification) {
        clarifications.add(clarification);
        numberOfClarifications++;
    }

    public Integer getNumberOfClarificationRequests() {
        return numberOfClarificationRequests;
    }

    public Integer getNumberOfAvaliableClarificationRequests() {
        return numberOfAvaliableClarificationRequests;
    }

    public Integer getNumberOfClarifications() {
        return numberOfClarifications;
    }

    public void setNumberOfClarificationRequests(Integer numberOfClarificationRequests) {
        this.numberOfClarificationRequests = numberOfClarificationRequests;
    }

    public void setNumberOfAvaliableClarificationRequests(Integer numberOfAvaliableClarificationRequests) {
        this.numberOfAvaliableClarificationRequests = numberOfAvaliableClarificationRequests;
    }

    public void setNumberOfClarifications(Integer numberOfClarifications) {
        this.numberOfClarifications = numberOfClarifications;
    }

    public void setClarificationRequests(ArrayList<ClarificationRequest> clarificationRequests) {
        this.clarificationRequests = clarificationRequests;
    }

    public void setAvaliableClarificationRequests(ArrayList<ClarificationRequest> avaliableClarificationRequests) {
        this.avaliableClarificationRequests = avaliableClarificationRequests;
    }

    public void setClarifications(ArrayList<Clarification> clarifications) {
        this.clarifications = clarifications;
    }

    public boolean isPublicInformation() {
        return publicInformation;
    }

    public void setPublicInformation(boolean publicInformation) {
        this.publicInformation = publicInformation;
    }

}
