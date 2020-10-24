package pt.ulisboa.tecnico.socialsoftware.tutor.clarificationRequest.dto;

import pt.ulisboa.tecnico.socialsoftware.tutor.clarificationRequest.domain.ClarificationRequest;
import pt.ulisboa.tecnico.socialsoftware.tutor.clarification.domain.Clarification;

import java.io.Serializable;
import java.util.*;


public class ClarificationRequestDto implements Serializable {

    private Integer id;
    private Integer studentId;
    private Integer questionId;
    private List<Clarification> clarifications;
    private String description;
    private boolean avaliable;

    public ClarificationRequestDto() {

    }

    public ClarificationRequestDto(ClarificationRequest clarificationRequest) {
        this.id = clarificationRequest.getId();
        this.studentId = clarificationRequest.getStudent().getId();
        this.questionId = clarificationRequest.getQuestion().getId();
        this.clarifications = clarificationRequest.getClarifications();
        this.description = clarificationRequest.getDescription();
        this.avaliable = clarificationRequest.isAvaliable();
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

    public Integer getQuestionId() {
        return questionId;
    }

    public void setQuestionId(Integer questionId) {
        this.questionId = questionId;
    }

    public List<Clarification> getClarifications() {
        return clarifications;
    }

    public void setClarifications(List<Clarification> clarifications) {
        this.clarifications = clarifications;
    }

    public String getDescription() {
        return description;
    }

    public void setDescription(String description) {
        this.description = description;
    }

    public boolean isAvaliable() {
        return avaliable;
    }

    public void setAvaliable(boolean avaliable) {
        this.avaliable = avaliable;
    }

    @Override
    public String toString() {
        return "ClarificationRequestDto{" +
                "id=" + id +
                ", studentId=" + studentId +
                ", questionId=" + questionId +
                ", description=" + description +
                ", avaliable=" + avaliable +
                '}';
    }

}
