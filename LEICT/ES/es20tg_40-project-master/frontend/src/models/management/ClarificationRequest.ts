import Question from '@/models/management/Question';
import User from '@/models/user/User';
import Clarification from '@/models/management/Clarification';

export default class ClarificationRequest {
    id: number | null = null;
    description: string = '';
    question: Question | null = null;
    student: User | null = null;
    clarifications: Clarification[] = [];

    constructor(jsonObj?: ClarificationRequest) {
        if (jsonObj) {
            this.id = jsonObj.id;
            this.description = jsonObj.description;
            this.question = jsonObj.question;
            this.student = jsonObj.student;
            this.clarifications = jsonObj.clarifications;

            this.clarifications = jsonObj.clarifications.map((clarification: Clarification) => new Clarification(clarification));
        }
    }
}
