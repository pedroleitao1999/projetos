import StatementClarification from '@/models/statement/StatementClarification';
import StatementClarificationRequest from '@/models/statement/StatementClarificationRequest';
import User from '@/models/user/User';

export default class StatementStudentDashboard {
  id!: number;
  numberOfClarificationRequests!: number;
  numberOfAvaliableClarificationRequests!: number;
  numberOfClarifications!: number;
  student!: User;
  clarificationRequests: StatementClarificationRequest[] = [];
  avaliableClarificationRequests: StatementClarificationRequest[] = [];
  clarifications: StatementClarification[] = [];

  constructor(jsonObj?: StatementStudentDashboard) {
    if (jsonObj) {
      this.id = jsonObj.id;
      this.numberOfClarificationRequests = jsonObj.numberOfClarificationRequests;
      this.numberOfAvaliableClarificationRequests = jsonObj.numberOfAvaliableClarificationRequests;
      this.numberOfClarifications = jsonObj.numberOfClarifications;
      this.student = jsonObj.student;
      this.clarificationRequests = jsonObj.clarificationRequests.map(clarificationRequest => {
        return new StatementClarificationRequest(clarificationRequest);
      });
      this.avaliableClarificationRequests = jsonObj.avaliableClarificationRequests.map(clarificationRequest => {
        return new StatementClarificationRequest(clarificationRequest);
      });
      this.clarifications = jsonObj.clarifications.map(clarification => {
        return new StatementClarification(clarification);
      });
    }
  }
}

