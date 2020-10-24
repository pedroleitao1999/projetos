package pt.ulisboa.tecnico.socialsoftware.tutor.clarification.domain;

import pt.ulisboa.tecnico.socialsoftware.tutor.clarificationRequest.domain.ClarificationRequest;
import pt.ulisboa.tecnico.socialsoftware.tutor.user.User;

import javax.persistence.*;
import javax.swing.*;

@Entity
@Table(name = "clarifications")
public class Clarification {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Integer id;

    @ManyToOne
    @JoinColumn(name = "teacher_id")
    private User teacher;

    @ManyToOne(fetch=FetchType.LAZY)
    @JoinColumn(name = "clarification_request_id")
    private ClarificationRequest request;

    @Column
    private String description;

    @Column
    private boolean avaliable;

    public Clarification() {
    }

    public Clarification(User teacher, ClarificationRequest request, String description){
        this.teacher = teacher;
        this.request = request;
        this.description = description;
        this.avaliable = false;
    }

    public Integer getId() {
        return id;
    }

    public void setId(Integer id) {
        this.id = id;
    }

    public User getTeacher() {
        return teacher;
    }

    public void setTeacher(User teacher) {
        this.teacher = teacher;
    }

    public ClarificationRequest getRequest() {
        return request;
    }

    public void setRequest(ClarificationRequest request) {
        this.request = request;
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

}
