import StatementClarification from '@/models/statement/StatementClarification';
import Question from '@/models/management/Question';
import User from '@/models/user/User';

export default class StatementClarificationRequest {
    id!: number;
    description!: string;
    student!: User;
    question!: Question;
    clarifications: StatementClarification[] = [];

    constructor(jsonObj?: StatementClarificationRequest) {
        if (jsonObj) {
            this.id = jsonObj.id;
            this.description = jsonObj.description;
            this.student = jsonObj.student;
            this.question = jsonObj.question;
            this.clarifications = jsonObj.clarifications.map(clarification => {
                return new StatementClarification(clarification);
            });
        }
    }
}

