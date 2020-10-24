import StatementCorrectAnswer from '@/models/statement/StatementCorrectAnswer';
import RemoteServices from '@/services/RemoteServices';
import StatementQuiz from '@/models/statement/StatementQuiz';
import StatementClarificationRequest from '@/models/statement/StatementClarificationRequest';
import StatementClarification from '@/models/statement/StatementClarification';
import Question from '@/models/management/Question';
import ClarificationRequest from '@/models/management/ClarificationRequest';

export default class StatementManager {
  questionType: string = 'all';
  assessment: string = 'all';
  numberOfQuestions: string = '5';
  statementQuiz: StatementQuiz | null = null;
  statementClarificationRequest: StatementClarificationRequest | null = null;
  correctAnswers: StatementCorrectAnswer[] = [];
  description: string = '';
  question: Question | null = null;
  clarificationRequest: ClarificationRequest | null = null;

  private static _quiz: StatementManager = new StatementManager();

  static get getInstance(): StatementManager {
    return this._quiz;
  }

  async getQuizStatement() {
    let params = {
      // topic: this.topic,
      questionType: this.questionType,
      assessment: this.assessment,
      numberOfQuestions: +this.numberOfQuestions
    };

    this.statementQuiz = await RemoteServices.generateStatementQuiz(params);
  }

  async getClarificationRequestStatement() {
    let params = {
      // topic: this.topic,
      descriprion: this.description,
      assessment: this.assessment,
      question: this.question
    };

    this.statementClarificationRequest = await RemoteServices.generateStatementClarificationRequest(params);
  }

  async getClarificationStatement() {
    let params = {
      // topic: this.topic,
      description: this.description,
      assessment: this.assessment,
      clarificationRequest: this.clarificationRequest
    };

    this.statementClarificationRequest = await RemoteServices.generateStatementClarificationRequest(params);
  }

  async concludeQuiz() {
    if (this.statementQuiz) {
      let answers = await RemoteServices.concludeQuiz(this.statementQuiz.id);
      if (answers) {
        this.correctAnswers = answers;
      }
    } else {
      throw Error('No quiz');
    }
  }

  reset() {
    this.statementQuiz = null;
    this.correctAnswers = [];
  }

  isEmpty(): boolean {
    return this.statementQuiz == null;
  }
}
