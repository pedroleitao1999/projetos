package pt.ulisboa.tecnico.socialsoftware.tutor.question.service

import org.springframework.beans.factory.annotation.Autowired
import org.springframework.boot.test.autoconfigure.orm.jpa.DataJpaTest
import pt.ulisboa.tecnico.socialsoftware.tutor.exceptions.TutorException
import pt.ulisboa.tecnico.socialsoftware.tutor.user.User
import pt.ulisboa.tecnico.socialsoftware.tutor.question.domain.Question
import pt.ulisboa.tecnico.socialsoftware.tutor.question.QuestionService
import spock.lang.Specification

import static pt.ulisboa.tecnico.socialsoftware.tutor.exceptions.ErrorMessage.QUESTION_HAS_NO_CLARIFICATION_REQUESTS


@DataJpaTest
class ConsultClarificationRequestsServiceSpockTest extends Specification {
    public static final String QUESTION_TITLE = "Bla"
    public static final String QUESTION_CONTENT = "Bla Bla Bla"

    @Autowired
    QuestionService questionService

    def setup() {
        def student = new User()
        student.setKey(1)
        studentRepository.save(student)
        def question = new Question()
        question.setKey(1)
        questionRepository.save(question)
    }

    def "consult the clarification requests about a question"() {
        // the clarification abrequests are consulted
        given: "a student and a question"
        def studentId = studentRepository.findAll().get(0).getId()
        def questionId = questionRepository.findAll().get(0).getId()
        def question = questionRepository.findAll().get(0)
        question.setTitle(QUESTION_TITLE)
        question.setContent(QUESTION_CONTENT)
        question.setStatus(Question.Status.AVAILABLE.name())

        when:
        def clarificationRequests = questionService.consultClarificationRequests(questionId)

        then: "clarification requests are consulted"
        clarificationRequests.size() > 0
    }

    def "the question doesnt have any clarification request associated"() {
        // an exception is thrown
        given:
        def studentId = studentRepository.findAll().get(0).getId()
        def questionId = questionRepository.findAll().get(0).getId()

        when:
        def clarificationRequests = questionService.consultClarificationRequests(questionId)
        clarificationRequests.size() == 0

        then:
        new TutorException(QUESTION_HAS_NO_CLARIFICATION_REQUESTS, questionId);
    }

}
