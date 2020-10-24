package pt.ulisboa.tecnico.socialsoftware.tutor.clarificationRequest.domain;

import pt.ulisboa.tecnico.socialsoftware.tutor.user.User;
import pt.ulisboa.tecnico.socialsoftware.tutor.question.domain.Question;
import pt.ulisboa.tecnico.socialsoftware.tutor.clarification.domain.Clarification;

import javax.persistence.*;
import java.util.*;

@Entity
@Table(name = "clarificationRequests")
public class ClarificationRequest {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Integer id;

    @ManyToOne
    @JoinColumn(name = "student_id")
    private User student;

    @ManyToOne
    @JoinColumn(name = "question_id")
    private Question question;

    @OneToMany(cascade = CascadeType.ALL, mappedBy = "clarificationReques")
    private List<Clarification> clarifications = new ArrayList<>();

    @Column(unique=true)
    private String description;

    @Column
    private boolean avaliable;

    public ClarificationRequest() {
    }


    public ClarificationRequest(User student, Question question, String description) {
        this.student = student;
        this.question = question;
        this.description = description;
        this.avaliable = false;
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

    public Question getQuestion() {
        return question;
    }

    public void setQuestion(Question question) {
        this.question = question;
    }

    public String getDescription() {
        return description;
    }

    public void setDescription(String description) {
        this.description = description;
    }

    public List<Clarification> getClarifications(){
        return clarifications;
    }

    public boolean isAvaliable() {
        return avaliable;
    }

    public void setAvaliable(boolean avaliable) {
        this.avaliable = avaliable;
    }

}
