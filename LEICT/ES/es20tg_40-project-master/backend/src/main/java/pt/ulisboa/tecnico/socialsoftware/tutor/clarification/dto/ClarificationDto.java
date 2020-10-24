package pt.ulisboa.tecnico.socialsoftware.tutor.clarification.dto;

import pt.ulisboa.tecnico.socialsoftware.tutor.clarification.domain.Clarification;

import java.io.Serializable;

public class ClarificationDto implements Serializable {
    private Integer id;
    private Integer teacherId;
    private Integer requestId;
    private String description;
    private boolean avaliable;

    public ClarificationDto() {

    }

    public ClarificationDto(Clarification clarification) {
        this.id = clarification.getId();
        this.teacherId = clarification.getTeacher().getId();
        this.requestId = clarification.getRequest().getId();
        this.description = clarification.getDescription();
        this.avaliable = clarification.isAvaliable();
    }

    public Integer getId() {
        return id;
    }

    public void setId(Integer id) {
        this.id = id;
    }

    public Integer getTeacherId() {
        return teacherId;
    }

    public void setTeacherId(Integer teacherId) {
        this.teacherId = teacherId;
    }

    public Integer getRequestId() {
        return requestId;
    }

    public void setRequestId(Integer requestId) {
        this.requestId = requestId;
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

    @Override
    public String toString() {
        return "ClarificationDto{" +
                "id=" + id +
                ", teacherId=" + teacherId +
                ", requestId=" + requestId +
                ", description=" + description +
                ", avaliable=" + avaliable +
                '}';
    }

}