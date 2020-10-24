import ClarificationRequest from '@/models/management/ClarificationRequest';
import User from '@/models/user/User';
import Clarification from '@/models/management/Clarification';
import Question from '@/models/management/Question';

export default class StudentDashboard {
  id: number | null = null;
  numberOfClarificationRequests: number | null = null;
  numberOfAvaliableClarificationRequests: number | null = null;
  numberOfClarifications: number | null = null;
  student: User | null = null;
  clarificationRequests: ClarificationRequest[] = [];
  avaliableClarificationRequests: ClarificationRequest[] = [];
  clarifications: Clarification[] = [];

  constructor(jsonObj?: StudentDashboard) {
    if (jsonObj) {
      this.id = jsonObj.id;
      this.numberOfClarificationRequests = jsonObj.numberOfClarificationRequests;
      this.numberOfAvaliableClarificationRequests = jsonObj.numberOfAvaliableClarificationRequests;
      this.numberOfClarifications = jsonObj.numberOfClarifications;
      this.student = jsonObj.student;
      this.clarificationRequests = jsonObj.clarificationRequests;
      this.clarificationRequests = jsonObj.clarificationRequests.map((clarificationRequest: ClarificationRequest) => new ClarificationRequest(clarificationRequest));
      this.avaliableClarificationRequests = jsonObj.avaliableClarificationRequests;
      this.avaliableClarificationRequests = jsonObj.avaliableClarificationRequests.map((clarificationRequest: ClarificationRequest) => new ClarificationRequest(clarificationRequest));
      this.clarifications = jsonObj.clarifications;
      this.clarifications = jsonObj.clarifications.map((clarification: Clarification) => new Clarification(clarification));
    }
  }
}